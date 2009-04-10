
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ntcan.h>
#include "amccan.h"
#include "amcdrive.h"

#define eprintf(f, args...) fprintf(stderr, f, ## args)

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
	


int main(int argc, char **argv) {

    servo_vars_t servos[2];
	int i = 0; 
	controller_info control[2];
	int id;

	memset(control,0,2*sizeof(control));
	drivesetup(servos);

    time_t start = time(NULL);
	eprintf("CAUTION: Controller is now active\n");

	control[0].setpoint = 2000;	
	control[1].setpoint = 2000;	

	while(1) {

		CMSG canMsg;

        int len = 1;
        try("canRead",
            canRead(handle, &canMsg, &len, NULL));

		if (canMsg.id == 0x301 || canMsg.id == 0x303) {
			id = (canMsg.id - 0x300) / 2;
			velocity_control(id,&control[id], &canMsg, &servos[id]);
		}
	
		printf("%d\t%f\t%f\t%f\t%f\n", i, control[0].pv * servos[0].current_sign, control[1].pv * servos[1].current_sign, control[0].setpoint, control[1].setpoint);
	
		time_t now = time(NULL);
        if (now - start > 20)
            break;
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
	int j;
    for (j = 3; j >= 0; j--) {
    	velocity |= canMsg->data[2+j];
    	velocity <<= 8;
    }        

	control->pv = amccan_decode_ds1(velocity, servo->k_i, servo->k_s);

	control->ev = control->setpoint * servo->current_sign - control->pv;
    control->de = (control->ev - control->ev_old)/ .001;
    control->ev_old = control->ev;
			
	control->ie += control->ev * .001;
 
    control->current = control->ev*.001 + control->de*0.000001 + control->ie*0.0005;
	control->current *= servo->current_sign;
			if (control->current > 50)
				control->current = 50;
			if (control->current < -50)
				control->current = -50;
            try("control", amcdrive_set_current(handle, servo, control->current));








	return 0;
}
