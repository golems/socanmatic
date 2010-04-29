
#include <stdio.h>
#include <stdarg.h>
#include <ntcan.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/amccan.h"
#include "include/amcdrive.h"
#include "ntcanopen.h"

static uint16_t cob_base = 0x200;

#define eprintf(f, args...) fprintf(stderr, f, ## args)

#ifdef DEBUG
#define dprintf(f, args...) fprintf(stderr, f, ## args)
#else
#define dprintf(f, args...)
#endif

static NTCAN_RESULT fail(NTCAN_RESULT result, const char *format, ...) {
    va_list ap;
    
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    
    return result != NTCAN_SUCCESS ? result : -1;
}

static NTCAN_RESULT try_ntcan(char *op, NTCAN_RESULT ntr) {
    if (NTCAN_SUCCESS != ntr)
        return fail(ntr, "Failed %s: %s\n", op, canResultString(ntr));
    return ntr;
}

static NTCAN_RESULT try_ntcan_dl(const char *op, const uint8_t *rcmd, NTCAN_RESULT ntr) {
    if (NTCAN_SUCCESS != ntr)
        return fail(ntr, "Failed %s: %s\n", op, canResultString(ntr));
    if (rcmd && 0x80 == *rcmd)
        return fail(-1, "Bad SDO DL %s\n", op);
    return ntr;
}

static NTCAN_RESULT amcdrive_get_info(NTCAN_HANDLE handle, uint id, servo_vars_t *drive_info) {
    NTCAN_RESULT status;
    uint8_t rcmd;
    
    status = try_ntcan_dl("read max current", &rcmd,
        canOpenSDOWriteWait_ul_u16(handle, &rcmd, &drive_info->k_p, id,
            AMCCAN_INDEX_BOARD_INFO, AMCCAN_SUBINDEX_MAX_PEAK_CURRENT));
    if (status != NTCAN_SUCCESS)
        return status;
    
    uint32_t switchingFrequencyPBF;
    status = try_ntcan_dl("read switching frequency", &rcmd,
        canOpenSDOWriteWait_ul_u32(handle, &rcmd, &switchingFrequencyPBF, id,
            AMCCAN_INDEX_BOARD_INFO, AMCCAN_SUBINDEX_BOARD_SWITCH_FREQ));
    if (status != NTCAN_SUCCESS)
        return status;
    drive_info->k_s = 1000*amcccan_decode_pbf(switchingFrequencyPBF);
    
    status = try_ntcan_dl("read feedback interpolation", &rcmd,
        canOpenSDOWriteWait_ul_u16(handle, &rcmd, &drive_info->k_i, id,
            AMCCAN_INDEX_FEEDBACK_PARM, AMCCAN_SUBINDEX_FEEDBACK_POS_INTERP));
    if (status != NTCAN_SUCCESS)
        return status;
    
    return status;
}

static NTCAN_RESULT amcdrive_enable_pdos(NTCAN_HANDLE handle, uint id, uint pdos, servo_vars_t *drive_info) {
    NTCAN_RESULT status;
    uint8_t rcmd;
    uint16_t cob_offset = id * 6;
    
    drive_info->rpdo_position = cob_base+cob_offset;
    drive_info->rpdo_velocity = cob_base+cob_offset+1;
    drive_info->rpdo_current =  cob_base+cob_offset+2;
    
    drive_info->tpdo_position = cob_base+cob_offset+3;
    drive_info->tpdo_velocity = cob_base+cob_offset+4;;
    drive_info->tpdo_current =  cob_base+cob_offset+5;
    
    // If they requested it we assume they want it
    canIdAdd(handle, drive_info->tpdo_position);
    canIdAdd(handle, drive_info->tpdo_velocity);
    canIdAdd(handle, drive_info->tpdo_current);
    
    dprintf("rpdo_position: %d\n", (pdos & ENABLE_RPDO_POSITION) == 0);
    status = try_ntcan_dl("rpdo_position", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_RPDO_3, 
            drive_info->rpdo_position, (pdos & ENABLE_RPDO_POSITION) == 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    dprintf("rpdo_velocity: %d\n", (pdos & ENABLE_RPDO_VELOCITY) == 0);
    status = try_ntcan_dl("rpdo_velocity", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_RPDO_4, 
            drive_info->rpdo_velocity, (pdos & ENABLE_RPDO_VELOCITY) == 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    dprintf("rpdo_current: %d\n", (pdos & ENABLE_RPDO_CURRENT) == 0);
    status = try_ntcan_dl("rpdo_current", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_RPDO_5, 
            drive_info->rpdo_current, (pdos & ENABLE_RPDO_CURRENT) == 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    dprintf("tpdo_position: %d\n", (pdos & REQUEST_TPDO_POSITION) == 0);
    status = try_ntcan_dl("tpdo_position", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_TPDO_3, 
            drive_info->tpdo_position, (pdos & REQUEST_TPDO_POSITION) == 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    dprintf("tpdo_velocity: %d\n", (pdos & REQUEST_TPDO_VELOCITY) == 0);
    status = try_ntcan_dl("tpdo_velocity", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_TPDO_4, 
            drive_info->tpdo_velocity, (pdos & REQUEST_TPDO_VELOCITY) == 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    dprintf("tpdo_current: %d\n", (pdos & REQUEST_TPDO_CURRENT) == 0);
    status = try_ntcan_dl("tpdo_current", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_TPDO_5, 
            drive_info->tpdo_current, (pdos & REQUEST_TPDO_CURRENT) == 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    return status;
}

NTCAN_RESULT amcdrive_enable_async_timer(NTCAN_HANDLE handle, uint id, uint update_freq) {
    NTCAN_RESULT status;
    uint8_t rcmd;
    
    // Configure PDOs to function in ASYNC mode
    status = try_ntcan_dl("tpdo_position_transmission", &rcmd,
        amccan_dl_pdo_trans(handle, &rcmd, id, 
            AMCCAN_TPDO_3, AMCCAN_PDO_TRANS_ASYNC, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("tpdo_velocity_transmission", &rcmd,
        amccan_dl_pdo_trans(handle, &rcmd, id, 
            AMCCAN_TPDO_4, AMCCAN_PDO_TRANS_ASYNC, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("tpdo_current_transmission", &rcmd,
        amccan_dl_pdo_trans(handle, &rcmd, id, 
            AMCCAN_TPDO_5, AMCCAN_PDO_TRANS_ASYNC, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("rpdo_position_set", &rcmd,
        amccan_dl_pdo_trans(handle, &rcmd, id, 
            AMCCAN_RPDO_3, AMCCAN_PDO_TRANS_ASYNC, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("rpdo_velocity_set", &rcmd,
        amccan_dl_pdo_trans(handle, &rcmd, id, 
            AMCCAN_RPDO_4, AMCCAN_PDO_TRANS_ASYNC, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("rpdo_current_set", &rcmd,
        amccan_dl_pdo_trans(handle, &rcmd, id, 
            AMCCAN_RPDO_5, AMCCAN_PDO_TRANS_ASYNC, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    uint cycle_time = update_freq <= 1000 ? 1000 / update_freq : 1;
    int tpdos[] = { AMCCAN_TPDO_3, AMCCAN_TPDO_4, AMCCAN_TPDO_5 };
    status = try_ntcan_dl("enable_timer", &rcmd,
        amccan_dl_timer1(handle, &rcmd, id, cycle_time, tpdos, 3));
    
    return status;
}

NTCAN_RESULT amcdrive_start(NTCAN_HANDLE handle, uint id) {
    NTCAN_RESULT status;
    uint8_t rcmd;
   
    // Both of these reset any errors -- requires a transition on CW from 0->1
    status = try_ntcan_dl("control - shutdown", &rcmd,
    amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_SHUTDOWN));
    if (status != NTCAN_SUCCESS)
        return status;
 
    status = try_ntcan_dl("control - reset error", &rcmd,
    amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_RESET_FAULT));
    if (status != NTCAN_SUCCESS)
        return status;
 
    status = try_ntcan_dl("control - shutdown", &rcmd,
    amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_SHUTDOWN));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("control - reset error", &rcmd,
    amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_RESET_FAULT));
    if (status != NTCAN_SUCCESS)
        return status;

    status = try_ntcan_dl("control - shutdown", &rcmd,
    amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_SHUTDOWN));
    if (status != NTCAN_SUCCESS)
	return status;
   
    status = try_ntcan_dl("control - reset error", &rcmd,
    amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_RESET_FAULT));
    if (status != NTCAN_SUCCESS)
	return status;
   

    // Put the drive into pre-operational state
    status = try_ntcan("NMT Start",
        canOpenWriteNMT(handle, id, CANOPEN_NMT_START_REMOTE));

    status = try_ntcan_dl("control - shutdown", &rcmd,
        amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_SHUTDOWN));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("control - switch on", &rcmd,
        amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_SWITCH_ON));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("current mode", &rcmd,
        amccan_dl_op_mode( handle, &rcmd, id, AMCCAN_OP_MODE_CURRENT));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("control - enable",&rcmd,
        amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_ENABLE_OPERATION));
    if (status != NTCAN_SUCCESS)
        return status;
    
    return status;
}

NTCAN_RESULT amcdrive_stop_drive(servo_vars_t *drive) {
    NTCAN_RESULT status;
    uint8_t rcmd;

    status = try_ntcan_dl("control - shutdown", &rcmd,
        amccan_dl_control(drive->handle, &rcmd, drive->canopen_id, AMCCAN_CONTROL_STATE_SHUTDOWN));
    if (status != NTCAN_SUCCESS)
        return status;

    return status;
}

NTCAN_RESULT amcdrive_start_drive(servo_vars_t *drive) {
    NTCAN_RESULT status;
    uint8_t rcmd;

    status = try_ntcan_dl("control - switch on", &rcmd,
        amccan_dl_control(drive->handle, &rcmd, drive->canopen_id, AMCCAN_CONTROL_STATE_SWITCH_ON));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("current mode", &rcmd,
        amccan_dl_op_mode( drive->handle, &rcmd, drive->canopen_id, AMCCAN_OP_MODE_CURRENT));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("control - enable",&rcmd,
        amccan_dl_control(drive->handle, &rcmd, drive->canopen_id, AMCCAN_CONTROL_STATE_ENABLE_OPERATION));
    if (status != NTCAN_SUCCESS)
        return status;

    return status;
}

NTCAN_RESULT amcdrive_reset_drive(NTCAN_HANDLE handle, uint identifier) {
    NTCAN_RESULT status;

    status = try_ntcan("reset",
        canOpenWriteNMT(handle, identifier, CANOPEN_NMT_RESET_NODE));
    if (status != NTCAN_SUCCESS)
        return status;
    sleep(3);
    
    return status;
}

NTCAN_RESULT amcdrive_reset_drives(NTCAN_HANDLE handle, uint *identifiers, uint count) {
    NTCAN_RESULT status;

    int i;
    for (i = 0; i < count; i++) {
        status = try_ntcan("reset",
            canOpenWriteNMT(handle, identifiers[i], CANOPEN_NMT_RESET_NODE));
        if (status != NTCAN_SUCCESS)
            return status;
    }
    sleep(3);
    
    return status;
}

NTCAN_RESULT amcdrive_init_drive(NTCAN_HANDLE handle, uint identifier, uint pdos, 
    uint update_freq, servo_vars_t *drive_info) {
    NTCAN_RESULT status;
    
    drive_info->current_sign = 1;
    drive_info->canopen_id = identifier;
    drive_info->handle = handle;
    
    status = try_ntcan("add SDO response",
        canOpenIdAddSDOResponse(handle, identifier));
    if (status != NTCAN_SUCCESS)
        goto fail; // Yes, yes, goto is evil, but we don't have proper exceptions.
                   // If you don't think this is what we should be doing, ask me
                   // before changing. -- Jon Olson

    // Put the drive into pre-operational state
    status = try_ntcan("pre-op",
        canOpenWriteNMT(handle, identifier, CANOPEN_NMT_PRE_OP));
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    // Fetch motor controller constants (max current, switching frequency, etc)
    status = amcdrive_get_info(handle, identifier, drive_info);
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    // Enable process data objects for receiving actual position, velocity, current and setting target position, velocity, current
    status = amcdrive_enable_pdos(handle, identifier, pdos, drive_info);
    if (status != NTCAN_SUCCESS)
        goto fail;

    // Enable the async transmission of process data objects on a timed basis
    status = amcdrive_enable_async_timer(handle, identifier, update_freq);
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    status = amcdrive_start(handle, identifier);
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    return NTCAN_SUCCESS;
    
fail:
    
    if (handle != -1)
        canClose(handle);
        
    // This error handling is incomplete. What we should really do is keep
    // track of the state of the drive as we initialize it and then have a
    // switch statement here with fallthrough to catch it in each state and
    // do any shutdown that is necessary from that state
    
    return status;
}

NTCAN_RESULT amcdrive_init_drives(NTCAN_HANDLE handle, uint *identifiers, uint count, uint pdos, uint update_freq, servo_vars_t *drive_infos) {
    NTCAN_RESULT status;
    int drive;
    
    for (drive = 0; drive < count; drive++) {
        status = amcdrive_init_drive(handle, identifiers[drive], pdos, update_freq, &drive_infos[drive]);
        if (status != NTCAN_SUCCESS)
            return status;
    }

    return NTCAN_SUCCESS;
}

NTCAN_RESULT amcdrive_open_drives(uint network, uint *identifiers, uint count, uint pdos, uint update_freq, servo_vars_t *drive_infos) {
    NTCAN_HANDLE handle;
    NTCAN_RESULT status;
    status = try_ntcan("canOpen",
        canOpen(network,    //net
                0,          //flags
                10,         //txqueue
                128,        //rxqueue
                1000,       //txtimeout
                2000,       //rxtimeout
                &handle));   // handle    
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan("canSetBaudrate",
        canSetBaudrate(handle, NTCAN_BAUD_1000));
    if (status != NTCAN_SUCCESS)
        goto fail;
        
    status = amcdrive_init_drives(handle, identifiers, count, pdos, update_freq, drive_infos);
    if (status != NTCAN_SUCCESS)
        goto fail;
        
    return NTCAN_SUCCESS;
    
fail:
    
    canClose(handle);
    return status;
}

static NTCAN_RESULT amcdrive_rpdo_cw_i16(NTCAN_HANDLE handle, uint rpdo, int16_t value) {
    CMSG canMsg;
    
    canMsg.id = rpdo;
    canMsg.len = 4;
    
    uint16_t control_word = htocs(0x8f);
    value = htocs(value);
    // Configure control word
    memcpy(&canMsg.data[0], &control_word, 2); // Copy contol word
    memcpy(&canMsg.data[2], &value, 2); // Copy
    
    int count = 1;
    return try_ntcan("amcdrive_rpdo", canWrite(handle, &canMsg, &count, NULL));
}

NTCAN_RESULT amcdrive_update_drives(servo_vars_t *drives, int count) {
    CMSG canMsgs[256]; // We can easily handle more than one message at a time

    int len = 256;
    int status = canRead(drives[0].handle, canMsgs, &len, NULL);

    if (status != NTCAN_SUCCESS)
        return status;

    int m, i;
    for (m = 0; m < len; m++) {
        CMSG *canMsg = &canMsgs[m];
        uint drive_id = (canMsg->id - 0x200) / 6;

        // Ok, so this scan is O(n), and with a map it could be O(1), but n in our case is typically 2.
        for (i = 0; i < count; i++) {
            servo_vars_t *d = &drives[i];
            if (d->canopen_id == drive_id) {
                if (d->tpdo_position == canMsg->id) {
                    int32_t position;
                    memcpy(&position, &canMsg->data[2], sizeof(int32_t));
                    position = ctohl(position);

                    d->pos_c = (d->current_sign) * position;

                }
                else if (d->tpdo_velocity == canMsg->id) {
                    int32_t velocity = 0;
                    memcpy(&velocity, &canMsg->data[2], sizeof(int32_t));
                    velocity = ctohl(velocity);

                    d->vel_cps = amccan_decode_ds1(velocity, d->k_i, d->k_s);
                    d->vel_cps *= d->current_sign; // Report velocity in same direction as current

                }
                else if (d->tpdo_current == canMsg->id) {
                    int16_t current;
                    memcpy(&current, &canMsg->data[2], sizeof(int16_t));
                    d->i_act = amccan_decode_dc1(current, d->k_p);
                    d->i_act *= d->current_sign;

                }
                break;
            }
        }
    }

    return NTCAN_SUCCESS;
}
NTCAN_RESULT amcdrive_set_current(servo_vars_t *drive, double amps) {
    NTCAN_HANDLE handle = drive->handle;
    CMSG canMsg;
    
    if (amps * 10 > drive->k_p)
        amps = drive->k_p / 10; // No, limit it.
    
    // AMC CANopen drives take current in a custom unit called DC2. This is
    // equal to amps times 2^15 / K_p (the peak current for the drive).
    float current_multiplier = (1 << 15) / (float)(drive->k_p / 10.0);
    int32_t dc2_current = amps * current_multiplier;
    dc2_current *= drive->current_sign;
    dprintf("set_dc2_current: %04x\n", dc2_current);
    
    return try_ntcan("set_current", 
        amcdrive_rpdo_cw_i16(handle, drive->rpdo_current, dc2_current));
}

/*
 * Set the current measured position to zero
 */
NTCAN_RESULT amcdrive_reset_position( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node) {

	// TODO: add ability to reset the current measured position to any value 'pos'

	NTCAN_RESULT ntr;

	int32_t pos = 0;

	// Set Position Limit 2039.01h
	ntr = canOpenSDOWriteWait_dl_i32( h, rcmd, node, AMCCAN_INDEX_POS_LIMIT, AMCCAN_SUBINDEX_POS_LIMIT_MEASURED, pos );
	if( NTCAN_SUCCESS != ntr ) return ntr;

	int i= 0;
	for (i = 0; i < 10; i++){	// Kasemsit: I do loop 10 times to make sure the position is actually reset.

		// Set Digital Input Mask: Load Measured Position 2058.08h
		ntr = canOpenSDOWriteWait_dl_u16( h, rcmd, node, 0x2058, 0x08, 0x1 );
		if( NTCAN_SUCCESS != ntr ) return ntr;

		// Reset Digital Input Mask: Load Measured Position 2058.08h
		ntr = canOpenSDOWriteWait_dl_u16( h, rcmd, node, 0x2058, 0x08, 0x0 );
		if( NTCAN_SUCCESS != ntr ) return ntr;

		// Set Digital Input Mask: Load Target Position Command 2058.09h
		ntr = canOpenSDOWriteWait_dl_u16( h, rcmd, node, 0x2058, 0x09, 0x1 );
		if( NTCAN_SUCCESS != ntr ) return ntr;

		// Reset Digital Input Mask: Load Target Position Command 2058.09h
		ntr = canOpenSDOWriteWait_dl_u16( h, rcmd, node, 0x2058, 0x09, 0x0 );
		if( NTCAN_SUCCESS != ntr ) return ntr;
	}

    return 0;
}

void amcdrive_print_info(NTCAN_HANDLE handle, uint id) {

	/* TODO: Print something....  */

	uint8_t rcmd;
    uint16_t k_p;

    // Print: Maximum Peak Current 20D8.0Ch
    canOpenSDOWriteWait_ul_u16(handle, &rcmd, &k_p, id, AMCCAN_INDEX_BOARD_INFO, AMCCAN_SUBINDEX_MAX_PEAK_CURRENT);
    printf("Maximum Peak Current (20D8.0Ch) = %u [PBC]\n", k_p);

}
