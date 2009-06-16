
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

    status = amcdrive_reset_drive(handle, 0x21);
    status = amcdrive_init_drive(handle, 0x21,REQUEST_TPDO_VELOCITY|REQUEST_TPDO_CURRENT|ENABLE_RPDO_CURRENT, 1000, 
        &servo);
    try("amcdrive_init", status);
    try("canIdAdd", canIdAdd(handle, 0x301));
    try("canIdAdd", canIdAdd(handle, 0x401));
    double current = 5.0;
    try("spin up", amcdrive_set_current(&servo, current));
  

    usleep(500000);
    try("spin down", amcdrive_set_current(&servo, 0));
}

