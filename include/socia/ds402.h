/* Copyright (c) 2013, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Neil T. Dantam <ntd@gatech.edu>
 *
 * Georgia Tech Humanoid Robotics Lab
 * Under Direction of Prof. Mike Stilman <mstilman@cc.gatech.edu>
 *
 *
 * This file is provided under the following "BSD-style" License:
 *
 *
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOO OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef SOCIA_402_H
#define SOCIA_402_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum socia_402_abort_conn {
    SOCIA_402_ABORT_CONN_NO_ACTION          = 0,
    SOCIA_402_ABORT_CONN_MALFUNCTION        = 1,
    SOCIA_402_ABORT_CONN_DISABLE_VOLTAGE    = 2,
    SOCIA_402_ABORT_CONN_QUICK_STOP         = 3
    /* 4 to 32767 reserved */
    /* -32768 to -1 manufacturer specific */
} socia_402_abort_conn_t;

typedef enum socia_402_motor_type {
    SOCIA_402_MOTOR_TYPE_NON_STANDARD         = 0x0000,
    SOCIA_402_MOTOR_TYPE_PHASE_MODULATED_DC   = 0x0001,
    SOCIA_402_MOTOR_TYPE_FREQ_CONTROLLED_DC   = 0x0002,
    SOCIA_402_MOTOR_TYPE_PM_SYNCHRONOUS       = 0x0003,
    SOCIA_402_MOTOR_TYPE_FC_SYNCHRONOUS       = 0x0004,
    SOCIA_402_MOTOR_TYPE_SWITCHED_RELUCTANCE  = 0x0005,
    SOCIA_402_MOTOR_TYPE_WOUND_ROTOR          = 0x0006,
    SOCIA_402_MOTOR_TYPE_SQUIRREL_CAGE        = 0x0007,
    SOCIA_402_MOTOR_TYPE_STEPPER              = 0x0008,
    SOCIA_402_MOTOR_TYPE_MICRO_STEP_STEPPER   = 0x0009,
    SOCIA_402_MOTOR_TYPE_SINUSOIDAL_PM_BL     = 0x000A,
    SOCIA_402_MOTOR_TYPE_TRAPEZOIDAL_PM_BL    = 0x000B,
    /* 0x000C - 0x7FFF reserved */
    /* 0x8000 - 0xFFFF manufacturer specific */
} socia_402_motor_type_t;

typedef enum socia_402_supported_drive_modes_mask {
    SOCIA_402_SUPPORTED_DRIVE_MODES_MASK_PP   = 1 << 0,
    SOCIA_402_SUPPORTED_DRIVE_MODES_MASK_VL   = 1 << 1,
    SOCIA_402_SUPPORTED_DRIVE_MODES_MASK_PV   = 1 << 2,
    SOCIA_402_SUPPORTED_DRIVE_MODES_MASK_TQ   = 1 << 3,
    /* bit 4 reserved */
    SOCIA_402_SUPPORTED_DRIVE_MODES_MASK_HM   = 1 << 5,
    SOCIA_402_SUPPORTED_DRIVE_MODES_MASK_IP   = 1 << 6,
    /* bits 7-15 reserved */
    /* bits 16-31 manufacturer specific */
} socia_402_supported_drive_modes_mask_;

typedef enum socia_402_digital_input_mask {
    SOCIA_402_DIGITAL_INPUT_MASK_NEG_LIMIT_SW = 1 << 0,
    SOCIA_402_DIGITAL_INPUT_MASK_POS_LIMIT_SW = 1 << 1,
    SOCIA_402_DIGITAL_INPUT_MASK_HOME_SW      = 1 << 2,
    SOCIA_402_DIGITAL_INPUT_MASK_INTERLOCK    = 1 << 3,
    /* bits 4-15 reserved */
    /* bits 16-31 manufacturer specific */
} socia_402_digital_input_mask_t;


typedef enum socia_402_ctrlmask {
    SOCIA_402_CTRLMASK_SWITCH_ON               = 1 <<  0,
    SOCIA_402_CTRLMASK_ENABLE_VOLTAGE          = 1 <<  1,
    SOCIA_402_CTRLMASK_QUICK_STOP              = 1 <<  2,
    SOCIA_402_CTRLMASK_ENABLE_OPERATION        = 1 <<  3,
    SOCIA_402_CTRLMASK_OP_MODE_SPECIFIC0       = 1 <<  4,
    SOCIA_402_CTRLMASK_OP_MODE_SPECIFIC1       = 1 <<  5,
    SOCIA_402_CTRLMASK_OP_MODE_SPECIFIC2       = 1 <<  6,
    SOCIA_402_CTRLMASK_RESET_FAULT             = 1 <<  7,
    SOCIA_402_CTRLMASK_HALT                    = 1 <<  8,
    SOCIA_402_CTRLMASK_RESERVED0               = 1 <<  9,
    SOCIA_402_CTRLMASK_RESERVED1               = 1 << 10,
    SOCIA_402_CTRLMASK_MANUFACTURER_SPECIFIC0  = 1 << 11,
    SOCIA_402_CTRLMASK_MANUFACTURER_SPECIFIC1  = 1 << 12,
    SOCIA_402_CTRLMASK_MANUFACTURER_SPECIFIC2  = 1 << 13,
    SOCIA_402_CTRLMASK_MANUFACTURER_SPECIFIC3  = 1 << 14,
    SOCIA_402_CTRLMASK_MANUFACTURER_SPECIFIC4  = 1 << 15
} socia_402_ctrlmask_t;

/** Velocity-mode controlword masks */
typedef enum socia_402_ctrlmask_vl {
    SOCIA_402_CTRLMASK_VL_RFG_ENABLE  = 1 << 4,
    SOCIA_402_CTRLMASK_VL_RFG_UNLOCK  = 1 << 5,
    SOCIA_402_CTRLMASK_VL_RFG_USE_REF = 1 << 6,
} socia_402_ctrlmask_vl_t;

/** Profile position mode controlword masks */
typedef enum socia_402_ctrlmask_pp {
    SOCIA_402_CTRLMASK_PP_NEW_SET_POINT          = 1 << 4,
    SOCIA_402_CTRLMASK_PP_CHANGE_SET_IMMEDIATELY = 1 << 5,
    SOCIA_402_CTRLMASK_PP_ABS_REL                = 1 << 6,
} socia_402_ctrlmask_pp_t;

/** Profile velocity mode controlword masks all reserved */
/** Profile torque mode controlword masks all reserved */

/** Homing mode controlword masks */
typedef enum socia_402_ctrlmask_hm {
    SOCIA_402_CTRLMASK_HM_OP_START        = 1 << 4,
    /* bits 5 and 6 reserved */
} socia_402_ctrlmask_hm_t;


/** Interpolation position mode controlword masks */
typedef enum socia_402_ctrlmask_ip {
    SOCIA_402_CTRLMASK_IP_ENABLe        = 1 << 4,
    /* bits 5 and 6 reserved */
} socia_402_ctrlmask_ip_t;

/** Bit masks to produce specific control commands.
 *
 * To build the control word for the command, bitwise and the word
 * with the MASK_AND and bitwise or the word with the MASK_OR */

typedef enum socia_402_ctrlcmd_mask {
    SOCIA_402_CTRLCMD_MASK_AND_SHUTDOWN  = (~SOCIA_402_CTRLMASK_RESET_FAULT & ~SOCIA_402_CTRLMASK_SWITCH_ON),
    SOCIA_402_CTRLCMD_MASK_OR_SHUTDOWN   = (SOCIA_402_CTRLMASK_QUICK_STOP | SOCIA_402_CTRLMASK_ENABLE_VOLTAGE),
    /* Need to set the "enable operation" bit independently */
    SOCIA_402_CTRLCMD_MASK_AND_SWITCH_ON = (~SOCIA_402_CTRLMASK_RESET_FAULT),
    SOCIA_402_CTRLCMD_MASK_OR_SWITCH_ON  = (SOCIA_402_CTRLMASK_QUICK_STOP | SOCIA_402_CTRLMASK_ENABLE_VOLTAGE |
                                            SOCIA_402_CTRLMASK_SWITCH_ON),
    SOCIA_402_CTRLCMD_MASK_AND_DISABLE_VOLTAGE = (~SOCIA_402_CTRLMASK_RESET_FAULT &
                                                  ~SOCIA_402_CTRLMASK_ENABLE_VOLTAGE),
    SOCIA_402_CTRLCMD_MASK_OR_DISABLE_VOLTAGE  = 0,
    SOCIA_402_CTRLCMD_MASK_AND_QUICK_STOP  = (~SOCIA_402_CTRLMASK_RESET_FAULT & ~SOCIA_402_CTRLMASK_QUICK_STOP),
    SOCIA_402_CTRLCMD_MASK_OR_QUICK_STOP   = SOCIA_402_CTRLMASK_ENABLE_VOLTAGE,
    SOCIA_402_CTRLCMD_MASK_AND_DISABLE_OP  = (~SOCIA_402_CTRLMASK_RESET_FAULT & ~SOCIA_402_CTRLMASK_ENABLE_OPERATION),
    SOCIA_402_CTRLCMD_MASK_OR_DISABLE_OP   = (SOCIA_402_CTRLMASK_QUICK_STOP | SOCIA_402_CTRLMASK_ENABLE_VOLTAGE |
                                              SOCIA_402_CTRLMASK_SWITCH_ON),
    SOCIA_402_CTRLCMD_MASK_AND_ENSABLE_OP  = (~SOCIA_402_CTRLMASK_RESET_FAULT),
    SOCIA_402_CTRLCMD_MASK_OR_ENSABLE_OP   = (SOCIA_402_CTRLMASK_ENABLE_OPERATION| SOCIA_402_CTRLMASK_QUICK_STOP |
                                              SOCIA_402_CTRLMASK_ENABLE_VOLTAGE | SOCIA_402_CTRLMASK_SWITCH_ON),
    SOCIA_402_CTRLCMD_MASK_AND_RESET_FAULT = 0xFFFF,
    SOCIA_402_CTRLCMD_MASK_OR_RESET_FAULT  = SOCIA_402_CTRLMASK_RESET_FAULT
} socia_402_ctrlcmd_mask_t;

#define SOCIA_402_INDEX_STATUSWORD     0x6041

enum socia_402_statmask {
    SOCIA_402_STATMASK_READY_TO_SWITCH_ON       = 1 <<  0,
    SOCIA_402_STATMASK_SWITCHED_ON              = 1 <<  1,
    SOCIA_402_STATMASK_OP_ENABLED               = 1 <<  2,
    SOCIA_402_STATMASK_FAULT                    = 1 <<  3,
    SOCIA_402_STATMASK_VOLTAGE_ENABLED          = 1 <<  4,
    SOCIA_402_STATMASK_QUICK_STOP               = 1 <<  5,
    SOCIA_402_STATMASK_SWITCH_ON_DISABLED       = 1 <<  6,
    SOCIA_402_STATMASK_WARNING                  = 1 <<  7,
    SOCIA_402_STATMASK_MANUFACTURER_SPECIFIC0   = 1 <<  8,
    SOCIA_402_STATMASK_REMOTE                   = 1 <<  9,
    SOCIA_402_STATMASK_TARGET_REACHED           = 1 << 10,
    SOCIA_402_STATMASK_INTERNAL_LIMIT_ACTIVE    = 1 << 11,
    SOCIA_402_STATMASK_OP_MODE_SPECIFIC0        = 1 << 12,
    SOCIA_402_STATMASK_OP_MODE_SPECIFIC1        = 1 << 13,
    SOCIA_402_STATMASK_MANUFACTURER_SPECIFIC1   = 1 << 14,
    SOCIA_402_STATMASK_MANUFACTURER_SPECIFIC2   = 1 << 15
} socia_402_statmask_t;


/** Canopen State machine.
 *
 * Set using the ControWord. Read using the status word.  Values of
 * the enum are the status word with don't care bits masked out.
 */
typedef enum socia_402_state_val {
    SOCIA_402_STATE_VAL_OFF_NRDY              = 0x00,
    SOCIA_402_STATE_VAL_OFF_SW_ON_DISABLE     = 0x40,
    SOCIA_402_STATE_VAL_OFF_RDY               = 0x21,
    SOCIA_402_STATE_VAL_ON_OP_DIS             = 0x23,
    SOCIA_402_STATE_VAL_ON_OP_EN              = 0x27,
    SOCIA_402_STATE_VAL_ON_QUICK_STOP         = 0x03,
    SOCIA_402_STATE_VAL_FAULT_REACTION_ACTIVE = 0x0F,
    SOCIA_402_STATE_VAL_FAULT                 = 0x08,
    SOCIA_402_STATE_VAL_UNKNOWN               = 0xFFFF
} socia_402_state_val_t;

/** Mask out don't care bits for mapping status_word to drive state
 *  See table 19, p. 41.
 */
typedef enum socia_402_state_val_mask {
    SOCIA_402_STATE_VAL_MASK_OFF_NRDY              = ~0x4F,
    SOCIA_402_STATE_VAL_MASK_OFF_SW_ON_DISABLE     = ~0x4F,
    SOCIA_402_STATE_VAL_MASK_OFF_RDY               = ~0x6F,
    SOCIA_402_STATE_VAL_MASK_ON_OP_DIS             = ~0x6F,
    SOCIA_402_STATE_VAL_MASK_ON_OP_EN              = ~0x6F,
    SOCIA_402_STATE_VAL_MASK_ON_QUICK_STOP         = ~0x6F,
    SOCIA_402_STATE_VAL_MASK_FAULT_REACTION_ACTIVE = ~0x4F,
    SOCIA_402_STATE_VAL_MASK_FAULT                 = ~0x4F
} socia_402_state_val_mask_t;

typedef enum socia_402_fault_reaction {
    /* -32768 to -1 manufacturer specific */
    SOCIA_402_FAULT_REACTION_DISABLE           = 0,
    SOCIA_402_FAULT_REACTION_SLOW_DOWN_RAMP    = 1,
    SOCIA_402_FAULT_REACTION_QUICK_STOP_RAMP   = 2,
    SOCIA_402_FAULT_REACTION_CURRENT_LIMIT     = 3,
    SOCIA_402_FAULT_REACTION_VOLTAGE_LIMIT     = 4,
    /* 5 - 32767 reserved */
} socia_402_fault_reaction_t;

enum socia_402_op_mode {
    /* -1 to -128 manufacturer specific */
    SOCIA_402_OP_MODE_POSITION             = 0x1,
    SOCIA_402_OP_MODE_VELOCITY             = 0x3,
    SOCIA_402_OP_MODE_CURRENT              = 0x4,
    SOCIA_402_OP_MODE_RESERVED1            = 0x5,
    SOCIA_402_OP_MODE_HOMING               = 0x6,
    SOCIA_402_OP_MODE_PVT                  = 0x7, /* Interpolated Position */
    SOCIA_402_OP_MODE_CYCLIC_SYNC_POSITION = 0x8,
    SOCIA_402_OP_MODE_CYCLIC_SYNC_VELOCITY = 0x9,
    SOCIA_402_OP_MODE_CYCLIC_SYNC_TORQUE   = 0xA,
    //11 ... 127 reserved
    SOCIA_402_OP_MODE_CUSTOM               = 0xff
} amccan_op_mode_t;

enum socia_402_polarity_mask {
    SOCIA_402_POLARITY_MASK_VEL = 1 << 6,
    SOCIA_402_POLARITY_MASK_POS = 1 << 7
} socia_402_polarity_mask_t;

#define SOCIA_402_POLARITY_FACTOR(bit) ((bit) ? -1 : 1)

#define SOCIA_402_POLARITY_FACTOR_VEL(polarity) \
    SOCIA_402_POLARITY_FACTOR( (polarity) & SOCIA_402_POLARITY_MASK_VEL )
#define SOCIA_402_POLARITY_FACTOR_POS(polarity) \
    SOCIA_402_POLARITY_FACTOR( (polarity) & SOCIA_402_POLARITY_MASK_POS )


#ifdef __cplusplus
}
#endif

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
#endif // SOCIA_402_H
