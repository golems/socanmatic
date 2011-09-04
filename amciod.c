/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil  -*- */
/* ex: set shiftwidth=4 expandtab: */
/** \file amciod.c
 *
 *  \author Jon Scholz
 *  \author Neil Dantam
 *  \author Evan Seguin
 *  \author Kasemsit Teeyapan
 */

/** \mainpage

 *  \brief amciod is an ACH front-end daemon to communicate with two
 *  AMC drives over CAN network.  It reads motor command messages from
 *  an ACH channel and publishes the current states of the motors to
 *  another ACH channel.
 *
 *  NOTE: The daemon reads state message in [rad, rad/s], and writes
 *  command message in [Ampere].
 */

#include <argp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include <somatic.h>
#include <somatic/daemon.h>
#include <ach.h>

#include <somatic/util.h>
#include <somatic.pb-c.h>
#include <somatic/msg/motor.h>

#include <math.h>

#include "amccan.h"
#include "amcdrive.h"

// Default channel constants
#define AMCIOD_CMD_CHANNEL_NAME "amc-cmd"
#define AMCIOD_STATE_CHANNEL_NAME "amc-state"

#define MAX_CURRENT 50        // Max motor current (Amps)
#define ENCODER_COUNT 4000
#define GEAR_REDUCTION (15/1)

#define COUNT_TO_RAD(count) \
  ( (count) * 2 * M_PI / (ENCODER_COUNT * GEAR_REDUCTION ) )

#define RAD_TO_COUNT(rad) \
  ( (rad) * (ENCODER_COUNT * GEAR_REDUCTION ) / (2 * M_PI) )

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

/* ----------- */
/* PROTOTYPES  */
/* ----------- */

typedef struct {
    somatic_d_t d;
    somatic_d_opts_t d_opts;
    Somatic__MotorState *state_msg;
    struct timespec time;

    struct timespec wait_time;
    ach_channel_t cmd_chan;
    ach_channel_t state_chan;
} cx_t;

static void
amciod_update_state( cx_t *cx, servo_vars_t *servos, ach_channel_t *state_chan);

static void
amciod_execute_and_update( cx_t *cx, servo_vars_t *servos,
                           Somatic__MotorCmd *msg, ach_channel_t *state_chan);

static int amciod_open(servo_vars_t *servos);
static void amciod_run(cx_t *cx, servo_vars_t *servos);

/* ---------- */
/* ARGP Junk  */
/* ---------- */

//TODO: replace these with vars from parsing args
static const char *opt_cmd_chan = AMCIOD_CMD_CHANNEL_NAME;
static const char *opt_state_chan = AMCIOD_STATE_CHANNEL_NAME;

static int opt_verbosity = 0;
static double opt_frequency = 30.0;   // refresh at 30 hz
static size_t n_buses = 0;            // number of bus
static size_t n_modules = 0;          // number of amc modules
static int32_t opt_bus_id = 0;        // amc bus id
static uint8_t opt_mod_id[2];         // amc module id

static struct argp_option options[] = {
    {
        .name = "cmd-chan",
        .key = 'c',
        .arg = "amciod-cmd",
        .flags = 0,
        .doc = "ACH channel to send amcdrive commands to"
    },
    {
        .name = "state-chan",
        .key = 's',
        .arg = "amciod-state",
        .flags = 0,
        .doc = "ACH channel to listen for commands on"
    },
    {
        .name = "verbose",
        .key = 'v',
        .arg = NULL,
        .flags = 0,
        .doc = "Causes verbose output"
    },
    {
        .name = "Create",
        .key = 'C',
        .arg = NULL,
        .flags = 0,
        .doc = "Create channels with specified names (off by default)"
    },
    {
        .name = "frequency",
        .key = 'f',
        .arg = "freq",
        .flags = 0,
        .doc = "Refresh rate on state channel when no commands are received"
    },
    {
        .name = "module",
        .key = 'm',
        .arg = "module_id",
        .flags = 0,
        .doc = "Define a module ID for a motor index (e.g. 0x20)"
    },
    {
        .name = "bus",
        .key = 'b',
        .arg = "CAN_bus",
        .flags = 0,
        .doc = "Define a CAN bus for a module"
    },
    SOMATIC_D_ARGP_OPTS,
    {
        .name = NULL,
        .key = 0,
        .arg = NULL,
        .flags = 0,
        .doc = NULL
    }
};


/// argp parsing function
static int parse_opt( int key, char *arg, struct argp_state *state);
/// argp program version
const char *argp_program_version = "amciod 0.2";
/// argp program arguments documention
static char args_doc[] = "";
/// argp program doc line
static char doc[] = "reads somatic messages and sends amcdrive motor commands";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL };

/**
 * \brief Generate the amcdrive calls requested by the specified motor
 * command message, and update the state
 */
int main(int argc, char *argv[]) {

    static cx_t cx;
    memset(&cx,0,sizeof(cx));
    cx.d_opts.ident = "amciod";
    cx.d_opts.sched_rt = SOMATIC_D_SCHED_MOTOR;

    // ------------- INIT --------------------

    /// Parse command line args
    argp_parse(&argp, argc, argv, 0, NULL, &cx);

    // init daemon context
    somatic_d_init(&cx.d, &cx.d_opts);

    /// AMC drive init
    servo_vars_t servos[2];
    int r = amciod_open(servos);
    aa_hard_assert(r == NTCAN_SUCCESS,
                   "AMC initialization failed. Error number: %i\n", r);

    /// Ach channels for amciod
    //ach_channel_t cmd_chan, state_chan;
    somatic_d_channel_open(&cx.d, &cx.cmd_chan, opt_cmd_chan, NULL );
    somatic_d_channel_open(&cx.d, &cx.state_chan, opt_state_chan, NULL );
    ach_flush(&cx.cmd_chan);

    /// Set the wait time based on specificed frequency
    cx.wait_time = (struct timespec){
        .tv_sec = 0,
        .tv_nsec = (long int)((1.0/opt_frequency) * 1e9)
    };

    // init message
    cx.state_msg = somatic_motor_state_alloc();
    cx.state_msg->has_status = 1;
    cx.state_msg->status = SOMATIC__MOTOR_STATUS__MOTOR_OK;
    {
        double x[n_modules];
        memset(x,0,sizeof(x[0])*n_modules);
        somatic_motor_state_set_position( cx.state_msg, x, n_modules );
        somatic_motor_state_set_velocity( cx.state_msg, x, n_modules );
        somatic_motor_state_set_current( cx.state_msg, x, n_modules );
    }

    if (opt_verbosity) {
        fprintf(stderr, "\n* AMCIOD *\n");
        fprintf(stderr, "Verbosity:    %d\n", opt_verbosity);
        fprintf(stderr, "command channel:      %s\n", opt_cmd_chan);
        fprintf(stderr, "state channel:      %s\n", opt_state_chan);
        fprintf(stderr, "n_module = %d\n", n_modules);
        fprintf(stderr, "module id = 0x%x  0x%x\n",
                servos[0].canopen_id, servos[1].canopen_id);
        fprintf(stderr, "-------\n");
    }

    // ------------- RUN --------------------
    amciod_run( &cx, servos );

    // ------------ DESTROY --------------

    /// Stop motors
    r = amcdrive_set_current(&servos[0], 0.0);
    aa_hard_assert(r == NTCAN_SUCCESS, "amcdrive_set_current error: %i\n", r);
    r = amcdrive_set_current(&servos[1], 0.0);
    aa_hard_assert(r == NTCAN_SUCCESS, "amcdrive_set_current error: %i\n", r);

    /// Close channels
    ach_close(&cx.cmd_chan);
    ach_close(&cx.state_chan);

    somatic_d_destroy(&cx.d);

    return 0;
}

static int parse_opt(int key, char *arg, struct argp_state *state) {
    cx_t *cx = (cx_t*)state->input;

    if (n_buses > 1) {
        fprintf(stderr, "ERROR: Accept only one -b parameters.\n");
        exit(1);
    }
    if (n_modules > 2) {
        fprintf(stderr, "ERROR: Accept only two -m parameters.\n");
        exit(1);
    }

    long int mod;
    switch (key) {

    case 'c':
        opt_cmd_chan = strdup(arg);
        break;
    case 's':
        opt_state_chan = strdup(arg);
        break;
    case 'v':
        opt_verbosity++;
        break;
    case 'f':
        opt_frequency = atof(arg);
        break;
    case 'b':
        opt_bus_id = atoi(arg);      // accept only one bus id
        n_buses++;
        break;
    case 'm':
        mod = strtol(arg, NULL, 16); // Add an AMC module id to the list
        assert(mod < UINT8_MAX);
        opt_mod_id[n_modules++] = (uint8_t) mod;
        break;
    }
    somatic_d_argp_parse( key, arg, &cx->d_opts );



    return 0;
}


/**
 * \brief
 */
static int amciod_open(servo_vars_t *servos){

    unsigned flags =
        REQUEST_TPDO_VELOCITY | REQUEST_TPDO_POSITION |
        ENABLE_RPDO_CURRENT | REQUEST_TPDO_STATUSWORD;
    NTCAN_RESULT r = amcdrive_open_drives( opt_bus_id, opt_mod_id, n_modules,
                                           flags, 1000, servos);
    if (r != NTCAN_SUCCESS) {
        perror("amcdrive_open_drives");
        return r;
    }
    // FIXME: WTF? -ntd
    servos[1].current_sign = -1;

    r = amcdrive_set_current(&servos[0], 0.0);
    aa_hard_assert(r == NTCAN_SUCCESS, "amcdrive_set_current error: %i\n", r);

    r = amcdrive_set_current(&servos[1], 0.0);
    aa_hard_assert(r == NTCAN_SUCCESS, "amcdrive_set_current error: %i\n", r);

    return 0;
}


static void amciod_run(cx_t *cx, servo_vars_t *servos) {
    /** \par Main loop
     *
     *  Listen on the motor command channel, and issue an amcdrive command for
     *  each incoming message.  When an acknowledgment is received from
     *  the module group, post it on the state channel.
     */

    somatic_d_event( &cx->d, SOMATIC__EVENT__PRIORITIES__NOTICE,
                     SOMATIC__EVENT__CODES__PROC_RUNNING,
                     NULL, NULL );

    while (!somatic_sig_received) {
        int r;
        // free memory
        aa_region_release( &cx->d.memreg );

        // returns the absolute time when receive should give up waiting
        struct timespec abstime = aa_tm_future(cx->wait_time);

        /// read current state from state channel
        Somatic__MotorCmd *cmd =
            SOMATIC_D_GET( &r, somatic__motor_cmd, &cx->d,
                           &cx->cmd_chan, &abstime,
                           ACH_O_WAIT | ACH_O_LAST  );

        aa_hard_assert(r == ACH_OK || r == ACH_TIMEOUT || ACH_MISSED_FRAME == r,
                       "Ach wait failure %s on cmd receive (%s, line %d)\n",
                       ach_result_to_string(r),
                       __FILE__, __LINE__);

        if (r == ACH_TIMEOUT) {
            amciod_update_state(cx, servos, &cx->state_chan);
        }
        else {
            /// Issue command, and update state
            amciod_execute_and_update(cx, servos, cmd, &cx->state_chan);
            /// Cleanup
        }

        if (opt_verbosity) {
            printf("pos: %.0f  %.0f [cnt]\t",
                   servos[0].act_pos, servos[1].act_pos);
            printf("vel: %.3f  %.3f [rad/s]\t",
                   COUNT_TO_RAD(servos[0].act_vel),
                   COUNT_TO_RAD(servos[1].act_vel));
            printf("status: 0x%x  0x%x\n", servos[0].status, servos[1].status);
        }
    }
    somatic_d_event( &cx->d, SOMATIC__EVENT__PRIORITIES__NOTICE,
                     SOMATIC__EVENT__CODES__PROC_STOPPING,
                     NULL, NULL );
}

static void amciod_update_state( cx_t *cx, servo_vars_t *servos,
                            ach_channel_t *state_chan)
{
    // Update amcdrive state
    NTCAN_RESULT status =  amcdrive_update_drives(servos, (int)n_modules);
    somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot update drive states!\n");
    for( size_t i = 0; i < n_modules; i++ ) {
        cx->state_msg->position->data[i] =  COUNT_TO_RAD(servos[i].act_pos);
        cx->state_msg->velocity->data[i] =  COUNT_TO_RAD(servos[i].act_vel);
        cx->state_msg->current->data[i] =  0;
    }
    somatic_metadata_set_time_now( cx->state_msg->meta );

    int r = SOMATIC_PACK_SEND( state_chan, somatic__motor_state, cx->state_msg );

    /// check message transmission
    somatic_d_check( &cx->d, SOMATIC__EVENT__PRIORITIES__CRIT,
                     SOMATIC__EVENT__CODES__COMM_FAILED_TRANSPORT,
                     ACH_OK == r,
                     "update_state",
                     "ach result: %s", ach_result_to_string(r) );

}

static void
amciod_execute_and_update( cx_t *cx, servo_vars_t *servos,
                           Somatic__MotorCmd *msg, ach_channel_t *state_chan)
{
    NTCAN_RESULT status;

    // amcdrive daemon currently accepts only current command
    if (msg->param != SOMATIC__MOTOR_PARAM__MOTOR_CURRENT) {
        fprintf(stdout, "ERROR: Accept only current command.\n");
        exit(0);
    }

    /**
     * Receive command message from the command channel, and send to amcdrive
     */

    // Print motor commands received from the command channel
    if (opt_verbosity) {
        size_t i
            ;
        for (i = 0; i < msg->values->n_data; ++i)
            fprintf(stdout, "%lf::", msg->values->data[i]);
        fprintf(stdout, "]\n");

        //size_t size = somatic__motor_cmd__get_packed_size(msg);
        //printf("\tmotor_cmd packed size = %d \n",size);
    }

    double motorLeftCurrent = msg->values->data[0];
    double motorRightCurrent = msg->values->data[1];

    // Current limit
    motorLeftCurrent = min(MAX_CURRENT, max(-MAX_CURRENT, motorLeftCurrent));
    motorRightCurrent = min(MAX_CURRENT, max(-MAX_CURRENT, motorRightCurrent));

    // Send current to amcdrive
    status = amcdrive_set_current(&servos[0], motorLeftCurrent);
    aa_hard_assert( status == NTCAN_SUCCESS, "Cannot set current (Left)!\n");
    status = amcdrive_set_current(&servos[1], motorRightCurrent);
    aa_hard_assert( status == NTCAN_SUCCESS, "Cannot set current (Right)!\n");

    // Update states
    amciod_update_state(cx, servos, state_chan);

}
