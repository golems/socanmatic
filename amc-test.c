
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
	uint32_t k_p;

	try("canOpen", canOpen(0, 0, 10, 128, 1000, 2000, &handle));
	try("canSetBaudrate", canSetBaudrate(handle, NTCAN_BAUD_1000));

	try("canOpenIdAddSDOResponse", canOpenIdAddSDOResponse(handle, 0x20));
	try("canOpenIdAddSDOResponse", canOpenIdAddSDOResponse(handle, 0x21));

    uint8_t rcmd;
    uint16_t maxCurrent;
	try("canOpenSDOWriteWait_ul_u32" canOpenSDOWriteWait_ul_u32(handle, 
	    &rcmd, &macCurrent, 0x20, AMCCAN_INDEX_BOARD_INFO,
        AMCCAN_SUBINDEX_MAX_PEAK_CURRENT));
    
    eprintf("Max continuous current: %d\n", maxCurrent);
    
    canClose(handle);
		
}

