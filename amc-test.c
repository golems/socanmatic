
#include <stdio.h>
#include "amccan.h"

#define eprintf(f, args...) fprintf(stderr, f, ## args)

static NTCAN_HANDLE handle;

void try(char *name, int status) {
	if (status != NTCAN_SUCCESS) {
		eprintf("%s(): %d\n", name, status);
		exit(-1);
	}
}

int main(int argc, char **argv) {
	uint32 k_p;

	try("canOpen", canOpen(0, 0, 10, 128, 1000, 2000, &handle));
	try("canSetBaudrate", canSetBaudrate(handle, NTCAN_BAUD_1000));

	try("canOpenIdAddSDOResponse", canOpenIdAddSDOResponse(handle, 0x20));
	try("canOpenIdAddSDOResponse", canOpenIdAddSDOResponse(handle, 0x21));

	try("canOpenSDOWriteWait_ul_u32(handle, 
		
}

