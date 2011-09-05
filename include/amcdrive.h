/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil  -*- */
/* ex: set shiftwidth=4 expandtab: */

#ifndef _AMCDRIVE_H_
#define _AMCDRIVE_H_

#include <ntcan.h>
#include <ntcanopen.h>
#include <assert.h>
#include "byteorder.h"

typedef struct {

    NTCAN_HANDLE handle;

    uint8_t canopen_id;         // CANopen identifier
    int8_t  current_sign;       // Flip current?

    // Conversion factor (see Appendix A in the manual)
    uint16_t k_i;               // Feedback interpolation value
    uint32_t k_s;               // Switch frequency of the drive in Hz
    uint16_t k_p;               // Maximum rated peak current of the drive in amps

    // Read by PDO
    double act_pos;             // Actual position in counts (we use double instead of int to avoid wraparound)
    double act_vel;             // Actual velocity in counts per second
    double act_cur;             // Actual current
    int16_t status;             // Status word of the drive

    // PDO->COB mappings
    uint16_t rpdo_position;
    uint16_t rpdo_velocity;
    uint16_t rpdo_current;
    uint16_t tpdo_position;
    uint16_t tpdo_velocity;
    uint16_t tpdo_current;
    uint16_t tpdo_statusword;

} servo_vars_t;

/*
 * These macros are used for enabling/disabling RPDO and TPDO in amcdrive_open_drives()
 */
#define ENABLE_RPDO_POSITION 0x01
#define ENABLE_RPDO_VELOCITY 0x02
#define ENABLE_RPDO_CURRENT  0x04
#define REQUEST_TPDO_POSITION 0x10
#define REQUEST_TPDO_VELOCITY 0x20
#define REQUEST_TPDO_CURRENT  0x40
#define REQUEST_TPDO_STATUSWORD  0x50

//NTCAN_RESULT amcdrive_init_drive(NTCAN_HANDLE network, uint8_t identifier, uint pdos, uint update_freq, servo_vars_t *drive_info);
NTCAN_RESULT amcdrive_init_drive( servo_vars_t *drive_info,  uint pdos, uint update_freq);
//NTCAN_RESULT amcdrive_init_drives(NTCAN_HANDLE network, uint8_t *identifiers, uint count, uint pdos, uint update_freq, servo_vars_t *drive_infos);

NTCAN_RESULT amcdrive_open_drives(int32_t network, uint8_t *identifiers, uint count, /*uint pdos, uint update_freq,*/ servo_vars_t *drive_infos);

NTCAN_RESULT amcdrive_update_drives(servo_vars_t *drives, int count);

/*
 * Send current command to motor
 */
NTCAN_RESULT amcdrive_set_current(servo_vars_t *drive, double amps);

/*
 * Set the current measured position to zero
 */
NTCAN_RESULT amcdrive_reset_position( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node);

/** Reset drives */
NTCAN_RESULT amcdrive_reset_drives(servo_vars_t *drives, size_t count);

NTCAN_RESULT amcdrive_start_drive(servo_vars_t *drive);
NTCAN_RESULT amcdrive_stop_drive(servo_vars_t *drive);

void amcdrive_dump_status(FILE *f, int16_t statw);
#endif
