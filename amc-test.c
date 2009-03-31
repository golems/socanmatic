
#include <stdio.h>
#include <stdlib.h>
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
	NTCAN_HANDLE handle;
	servo_vars_t servo;

	status = amcdrive_init(0, 0x20, REQUEST_TPDO_POSITION|REQUEST_TPDO_VELOCITY, 1000, &handle, &servo);
	try("amcdrive_init", status);
	
	canClose(handle);
}

