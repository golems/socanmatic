
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ntcan.h>
#include "amccan.h"
#include "amcdrive.h"

#define eprintf(f, args...) fprintf(stderr, f, ## args)

struct js_event {
    uint32_t time;
    int16_t value;
    uint8_t type;
    uint8_t number;
};

#define JS_EVENT_BUTTON 0x01
#define JS_EVENT_AXIS 0x02

static NTCAN_HANDLE handle;

void try(char *name, int status) {
    if (status != NTCAN_SUCCESS) {
        eprintf("%s(): %d\n", name, status);
        exit(-1);
    }
}

typedef struct {
	double current; //command current
	double setpoint; //command setpoint
	double pv; //read velocity
	double ev; //veolcity error
	double ev_old; //last velocity error
	double de; //derivative of error
	double ie; //integral error
} controller_info;

controller_info control[2];
static int quit_now = 0;
	
void *js_listen(void *ignored) {
    int fd = open("/dev/input/js0", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(-1);
    }

    while (!quit_now) {
        struct js_event e;
        int input = read(fd, &e, sizeof(struct js_event));
        if (input < sizeof(struct js_event))
            quit_now = 1;

        if (e.type == JS_EVENT_AXIS) {
            if (e.number == 1) {
                control[0].setpoint = e.value;
                control[1].setpoint = e.value;
            }
        }
    }
}

int main(int argc, char **argv) {

    servo_vars_t servos[2];
	int i = 0; 
	int id;

	memset(control,0,2*sizeof(control));
	drivesetup(servos);

    time_t start = time(NULL);
	eprintf("CAUTION: Controller is now active\n");

	control[0].setpoint = 0;	
	control[1].setpoint = 0;	

    pthread_t thread;
    int status = pthread_create(&thread, NULL, js_listen, NULL);
    if (status != 0) {
        perror("pthread_create");
        exit(-1);
    }

	while(!quit_now) {

		CMSG canMsg;

        int len = 1;
        try("canRead",
            canRead(handle, &canMsg, &len, NULL));

		if (canMsg.id == 0x301 || canMsg.id == 0x303) {
			id = (canMsg.id - 0x300) / 2;
			velocity_control(id, &control[id], &canMsg, &servos[id]);
		}
	
		if (i == 0)
			printf("count velocity_0 velocity_1 current_0 current_1\n");
		printf("%d\t%f\t%f\t%f\t%f\n", i, control[0].pv * servos[0].current_sign, control[1].pv * servos[1].current_sign, control[0].current, control[1].current);
	
		i++;

	}

    usleep(500000);
    try("spin down", amcdrive_set_current(handle, &servos[0], 0));
    try("spin down", amcdrive_set_current(handle, &servos[1], 0));
	return 0;
}

int drivesetup(servo_vars_t *servos){

NTCAN_RESULT status;

    try("canOpen",
        canOpen(0,          //net
                0,          //flags
                10,         //txqueue
                128,        //rxqueue
                1000,       //txtimeout
                2000,       //rxtimeout
                &handle));   // handle    
    try("canSetBaudrate",
        canSetBaudrate(handle, NTCAN_BAUD_1000));

    uint32_t drives[2];
    drives[0] = 0x20;
    drives[1] = 0x21;

    status = amcdrive_init_drives(handle,drives, 2,REQUEST_TPDO_VELOCITY|REQUEST_TPDO_CURRENT|ENABLE_RPDO_CURRENT,
        1000, servos);


    servos[0].current_sign = -1;
    try("amcdrive_init", status);
    try("canIdAdd", canIdAdd(handle, 0x301));
    try("canIdAdd", canIdAdd(handle, 0x303));

    double current = 0.0;
    try("spin up", amcdrive_set_current(handle, &servos[0], current));
    try("spin up", amcdrive_set_current(handle, &servos[1], current));
	
	return 0;
}

int velocity_control(int id,controller_info *control,CMSG *canMsg,servo_vars_t *servo) {



	int32_t velocity = 0;
	memcpy(&velocity, &canMsg->data[2], sizeof(int32_t));
	velocity = ctohl(velocity);

	control->pv = amccan_decode_ds1(velocity, servo->k_i, servo->k_s);

	control->ev = control->setpoint * servo->current_sign - control->pv;
    control->de = (control->ev - control->ev_old)/ .001;
    control->ev_old = control->ev;
			
	control->ie += control->ev * .001;
 
    control->current = control->ev*.001 + control->de*0.000001 + control->ie*0.001;
	control->current *= servo->current_sign;
			if (control->current > 50)
				control->current = 50;
			if (control->current < -50)
				control->current = -50;
            try("control", amcdrive_set_current(handle, servo, control->current));

	return 0;
}

