
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

int main(int argc, char **argv) {
    NTCAN_RESULT status;
    servo_vars_t servo;

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

    status = amcdrive_init_drive(handle, 0x20,REQUEST_TPDO_VELOCITY|REQUEST_TPDO_CURRENT|ENABLE_RPDO_CURRENT, 1000, 
        &servo);
    try("amcdrive_init", status);
    try("canIdAdd", canIdAdd(handle, 0x301));
    
    double current = 0.0;
    double setpoint =8000.0;
    try("spin up", amcdrive_set_current(handle, &servo, current));
  
	eprintf("CAUTION: Controller is now active\n");
		int tw, tf, ts;
		tw = tf = ts = 0;
 
	double ie = 0;
	int i = 0; 
    time_t start = time(NULL);
    double current_d, v_d;
    double e_old = 0;
	int ringsize = 05; 
	double ring[ringsize];
	int k = 0;
    int l = 0;
    for(k = 0; k < ringsize; ++k )
        ring[k] = 0;
	double ev;
    int j;

    while(1) {
        CMSG canMsg;
        
        int len = 1;
        try("canRead",
            canRead(handle, &canMsg, &len, NULL));
        if (canMsg.id == 0x301) {
            int32_t velocity = 0;
            memcpy(&velocity, &canMsg.data[2], sizeof(int32_t));
            velocity = ctohl(velocity);

            double pv = amccan_decode_ds1((int32_t) velocity,(uint16_t) servo.k_i,(uint32_t) servo.k_s);
            double new_ev = setpoint - pv;
           
			ring[l++] = new_ev;
			l %= ringsize;

			ev = 0;
            for(j = 0; j < ringsize; j++)
                ev += ring[j];
            ev /= ringsize;


			double de = (ev - e_old) / .001;
            e_old = ev;
			
			ie += ev * .001;	           

 
            current = ev*.0010 + de*0.000001 + ie*0.0005;
			if (current > 50)
				current = 50;
			if (current < -50)
				current = -50;
            try("control", amcdrive_set_current(handle, &servo, current));



			printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\n",i,pv,setpoint,ev,(de / 1000),(current * 1000),ie);
//            printf("Velocity: %f\n", pv);
//            printf("Error: %f\n", ev);
            ++i;
        }
        if (canMsg.id == 0x401) {
            int16_t current;
            memcpy(&current, &canMsg.data[2], 2);
            current_d = ((double)current / ((1 << 15) / ((double)servo.k_p)));
            printf("Current: %f\n", current_d);
        }

        time_t now = time(NULL);
        if (now - start >20)
            break;
//		if ((now - start > 5) && !tw){
//			eprintf("-4000!");
//			setpoint = -8000;
//			ie = 0;
//			tw = 1;
//		}
//		if ((now - start >10) && !tf){
//			eprintf("4000!");
//			setpoint = -8000;
//			ie = 0;
//			tf = 1;
//		}
//		if ((now - start >15) && !ts){
//			eprintf("8000!");
//			setpoint = -8000;
//			ie = 0;
//			ts = 1;
//		{

    }

    usleep(500000);
    try("spin down", amcdrive_set_current(handle, &servo, 0));
}

