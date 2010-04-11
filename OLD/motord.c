
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#include "amcdrive.h"
#include <ach.h>

#include "amcdrived.h"

#define eprintf(fmt, args...) fprintf(stderr, fmt, ## args)

#ifdef DEBUG
#define dprintf(fmt, args...) eprintf(fmt, ## args)
#else
#define dprintf(fmt, args...)
#endif

#define MAX_DRIVES 256
#define MAX_NAME_LENGTH 64

static int can_bus = -1;
static int drive_count = -1;
static servo_vars_t drives[MAX_DRIVES];
static char drive_names[MAX_DRIVES][MAX_NAME_LENGTH];
static ach_channel_t channels[2*MAX_DRIVES];

static int go = 0; // Used to signal threads to start. Easier than a mutex

static struct option long_options[] = {
    { "can-bus", required_argument, NULL, 'b' },
    { "drive-id", required_argument, NULL, 'd' },
    { "drive-name", required_argument, NULL, 'n' },
    { NULL, 0, NULL, 0 }
};

static void print_usage_and_exit(char *name) {
    eprintf("Usage: %s -b <CAN bus> -d <drive CANopen ID> -n <drive name> ...\n", name);
    exit(1);
}

static void parse_command_line(int argc, char **argv) {
    int drive_index = -1;
    int c;

    memset(drives, 0, sizeof(drives));

    while ((c = getopt_long(argc, argv, "b:d:n:", long_options, NULL)) != -1) {
        switch (c) {
            case -1:
                break;
            case 'b':
                can_bus = atoi(optarg);
                break;
            case 'd':
                drives[++drive_index].canopen_id = strtol(optarg, NULL, 16);
                break;
            case 'n':
                if (drive_index < 0) {
                    eprintf("Must specify drive identifier before name\n");
                    print_usage_and_exit(argv[0]);
                }
                strncpy(drive_names[drive_index], optarg, MAX_NAME_LENGTH);
                drive_names[drive_index][MAX_NAME_LENGTH-1] = '\0';
                break;
            case '?':
            default:
                eprintf("Unknown option: %c\n", c);
                print_usage_and_exit(argv[0]);
                break;
        }
    }
    
    drive_count = drive_index + 1;
    
    if (can_bus < 0) {
        eprintf("Must specify CAN bus!\n");
        print_usage_and_exit(argv[0]);
    }
    if (drive_count < 1) {
        eprintf("Must specify at least one motor!\n");
        print_usage_and_exit(argv[0]);
    }

    for (int i = 0; i < drive_count; i++) {
        if (!strcmp(drive_names[i], "")) {
            eprintf("All drives must have non-empty names.\n");
            print_usage_and_exit(argv[0]);
        }
    }
}

static int open_channels() {
    char channel_name[MAX_NAME_LENGTH+16];

    for (int i = 0; i < drive_count; i++) {
        sprintf(channel_name, "%s_drive_data", drive_names[i]);
        if (ach_publish(&channels[2*i], channel_name, 10, sizeof(drive_data_t)) != ACH_OK)
            goto fail;
        sprintf(channel_name, "%s_drive_control", drive_names[i]);
        if (ach_publish(&channels[2*i+1], channel_name, 10, sizeof(drive_control_t)) != ACH_OK)
            goto fail;
    }

fail:
    // TODO: Clean up open channels here in the future

    return -1;
}

static int open_drives() {
    int identifiers[MAX_DRIVES];

    for (int i = 0; i < drive_count; i++)
        identifiers[i] = drives[i].canopen_id;

    int result = amcdrive_open_drives(can_bus, identifiers, drive_count, REQUEST_TPDO_CURRENT|REQUEST_TPDO_VELOCITY|REQUEST_TPDO_POSITION|ENABLE_RPDO_CURRENT, 1000, drives);
    if (result < 0)
        return result;

    return 0;
}

static void clear_drive(servo_vars_t *d) {
    d->fresh_position = 0;
    d->fresh_velocity = 0;
    d->fresh_current = 0;
}

static int drive_ready(servo_vars_t *d) {
    return d->fresh_position && d->fresh_velocity && d->fresh_current;
}

static double now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

static void publish_update(servo_vars_t *drive, int drive_index) {
    ach_channel_t *channel = &channels[2*drive_index];
    drive_data_t data;

    data.position = drive->raw_position;
    data.velocity = drive->vel_cps;
    data.current = drive->i_act;
    data.ts = now();

    int ach_status = ach_put(channel, &data, sizeof(data));
    if (ach_status != ACH_OK) {
        // TODO handle ach put error
    }
}

static void *drive_listener(void *ignored) {
    while (!go) sched_yield(); // Spinlock to ensure successful startup

    for (int i = 0; i < drive_count; i++)
        clear_drive(&drives[i]);

    while (go) {
        if (amcdrive_update_drives(drives, drive_count) < 0) {
            // TODO handle drive update error
        }
        for (int i = 0; i < drive_count; i++) {
            if (drive_ready(&drives[i])) {
                publish_update(&drives[i], i);
                clear_drive(&drives[i]);
            }
        }
    }
}

static void *control_listener(void *drive_index_ptr) {
    while (!go) sched_yield(); // Spinlock to ensure successful startup (very brief... either both threads start or we quit)

    int drive_index = *(int *)drive_index_ptr;
    servo_vars_t *drive = &drives[2*drive_index];
    ach_channel_t *channel = &channels[2*drive_index+1];

    size_t copied_size;
    drive_control_t control;

    int inhibited = 0;

    int ach_status;
    while (go) {
        if ((ach_status = ach_wait_last(channel, &control, sizeof(control), &copied_size, NULL)) != ACH_OK) {
            // TODO handle ach errors
        }
        if (copied_size != sizeof(control))
            continue; // Ignore improperly sized messages (maybe print a debug message?)

        // Great, all is well.
        switch(control.control_type) {
            case AMCDRIVE_CONTROL_CURRENT:
                if (!inhibited)
                    amcdrive_set_current(drive, control.current);
                break;
            case AMCDRIVE_EMERGENCY_STOP:
                inhibited = 1;
                amcdrive_stop_drive(drive);
                break;
            case AMCDRIVE_RESUME_OPERATION:
                inhibited = 0;
                amcdrive_start_drive(drive);
                break;
            default:
                break;
        }
    }
}

static void run() {

    pthread_t update;
    int result;

    result = pthread_create(&update, NULL, drive_listener, NULL);
    if (result < 0) {
        eprintf("Unable to create drive listener thread, exiting");
        exit(3);
    }

    int indicies[MAX_DRIVES];
    pthread_t controls[MAX_DRIVES];
    for (int i = 0; i < drive_count; i++) {
        indicies[i] = i;
        result = pthread_create(&controls[i], NULL, control_listener, &indicies[i]);
        if (result < 0) {
            eprintf("Unable to create control listener thread, exiting\n");
            exit(4);
        }
    }

    go = 1;

    for (int i = 0; i < drive_count; i++) {
        pthread_join(controls[i], NULL);
    }
    pthread_join(update, NULL);
    // Right now we never get here.
}

int main(int argc, char **argv) {
    parse_command_line(argc, argv);

    if (open_channels() < 0) {
        eprintf("Failed to open ach channels, existing\n");
        exit(1);
    }

    if (open_drives() < 0) {
        eprintf("Failed to open drives, exiting\n");
        exit(2);
    }

    run();

    return 0;
}



