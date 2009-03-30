
#include <stdio.h>
#include <stdarg.h>
#include <ntcan.h>
#include "amccan.h"
#include "amcdrive.h"
#include "ntcanopen.h"

static uint16_t position_cob_base = 0x200;
static uint16_t velocity_cob_base = 0x300;
static uint16_t current_cob_base = 0x400;

#define eprintf(f, args...) fprintf(stderr, f, ## args)

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
    drive_info->k_s = amcccan_decode_pbf(switchingFrequencyPBF);
    
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
    
    drive_info->rpdo_position = position_cob_base++;
    drive_info->rpdo_velocity = velocity_cob_base++;
    drive_info->rpdo_current = current_cob_base++;
    
    drive_info->tpdo_position = position_cob_base++;
    drive_info->tpdo_velocity = velocity_cob_base++;
    drive_info->tpdo_current = current_cob_base++;
    
    status = try_ntcan_dl("rpdo_position", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_RPDO_3, 
            drive_info->rpdo_position, (pdos & ENABLE_RPDO_POSITION) != 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("rpdo_velocity", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_RPDO_4, 
            drive_info->rpdo_velocity, (pdos & ENABLE_RPDO_VELOCITY) != 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("rpdo_current", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_RPDO_5, 
            drive_info->rpdo_current, (pdos & ENABLE_RPDO_CURRENT) != 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("tpdo_position", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_TPDO_3, 
            drive_info->tpdo_position, (pdos & REQUEST_TPDO_POSITION) != 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("tpdo_velocity", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_TPDO_4, 
            drive_info->tpdo_velocity, (pdos & REQUEST_TPDO_VELOCITY) != 0, 0));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("tpdo_current", &rcmd,
        amccan_dl_pdo_id(handle, &rcmd, id, AMCCAN_TPDO_5, 
            drive_info->tpdo_current, (pdos & REQUEST_TPDO_CURRENT) != 0, 0));
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
    
    status = try_ntcan_dl("control - shutdown", &rcmd,
        amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_SHUTDOWN));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("control - switch on", &rcmd,
        amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_ON));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("current mode", &rcmd,
        amccan_dl_op_mode( handle, &rcmd, id, AMCCAN_OP_MODE_CURRENT));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan_dl("control - enable",&rcmd,
        amccan_dl_control(handle, &rcmd, id, AMCCAN_CONTROL_STATE_ENABLE));
    if (status != NTCAN_SUCCESS)
        return status;
    
    status = try_ntcan("NMT Start",
        canOpenWriteNMT(handle, id, CANOPEN_NMT_START_REMOTE));

    return status;
}

NTCAN_RESULT amcdrive_init(uint bus, uint identifier, uint pdos, 
    uint update_freq, NTCAN_HANDLE *handle, servo_vars_t *drive_info) {
    NTCAN_RESULT status;
    
    *handle = -1;
    status = try_ntcan("handle open",
        canOpen(0,          //net
                0,          //flags
                10,         //txqueue
                128,        //rxqueue
                1000,       //txtimeout
                2000,       //rxtimeout
                handle));   // handle    
    if (status != NTCAN_SUCCESS)
        goto fail; // Yes, yes, goto is evil, but we don't have proper exceptions.
                   // If you don't think this is what we should be doing, ask me
                   // before changing. -- Jon Olson
    
    status = try_ntcan("add SDO response",
        canOpenIdAddSDOResponse(*handle, identifier));
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    // Put the drive into pre-operational state
    status = try_ntcan("pre-op",
        canOpenWriteNMT(*handle, identifier, CANOPEN_NMT_PRE_OP));
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    // Fetch motor controller constants (max current, switching frequency, etc)
    status = amcdrive_get_info(*handle, identifier, drive_info);
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    // Enable process data objects for receiving actual position, velocity, current and setting target position, velocity, current
    status = amcdrive_enable_pdos(*handle, identifier, pdos, drive_info);
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    // Enable the async transmission of process data objects on a timed basis
    status = amcdrive_enable_async_timer(*handle, identifier, update_freq);
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    status = amcdrive_start(*handle, identifier);
    if (status != NTCAN_SUCCESS)
        goto fail;
    
    return NTCAN_SUCCESS;
    
fail:
    
    if (*handle != -1)
        canClose(*handle);
    
    return status;
}
