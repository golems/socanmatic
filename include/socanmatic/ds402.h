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


#ifndef SOCANMATIC_402_H
#define SOCANMATIC_402_H

#ifdef __cplusplus
extern "C" {
#endif

extern canmat_dict_t canmat_dict402;

typedef enum canmat_402_abort_conn {
    CANMAT_402_ABORT_CONN_NO_ACTION          = 0,
    CANMAT_402_ABORT_CONN_MALFUNCTION        = 1,
    CANMAT_402_ABORT_CONN_DISABLE_VOLTAGE    = 2,
    CANMAT_402_ABORT_CONN_QUICK_STOP         = 3
    /* 4 to 32767 reserved */
    /* -32768 to -1 manufacturer specific */
} canmat_402_abort_conn_t;

typedef enum canmat_402_motor_type {
    CANMAT_402_MOTOR_TYPE_NON_STANDARD         = 0x0000,
    CANMAT_402_MOTOR_TYPE_PHASE_MODULATED_DC   = 0x0001,
    CANMAT_402_MOTOR_TYPE_FREQ_CONTROLLED_DC   = 0x0002,
    CANMAT_402_MOTOR_TYPE_PM_SYNCHRONOUS       = 0x0003,
    CANMAT_402_MOTOR_TYPE_FC_SYNCHRONOUS       = 0x0004,
    CANMAT_402_MOTOR_TYPE_SWITCHED_RELUCTANCE  = 0x0005,
    CANMAT_402_MOTOR_TYPE_WOUND_ROTOR          = 0x0006,
    CANMAT_402_MOTOR_TYPE_SQUIRREL_CAGE        = 0x0007,
    CANMAT_402_MOTOR_TYPE_STEPPER              = 0x0008,
    CANMAT_402_MOTOR_TYPE_MICRO_STEP_STEPPER   = 0x0009,
    CANMAT_402_MOTOR_TYPE_SINUSOIDAL_PM_BL     = 0x000A,
    CANMAT_402_MOTOR_TYPE_TRAPEZOIDAL_PM_BL    = 0x000B,
    /* 0x000C - 0x7FFF reserved */
    /* 0x8000 - 0xFFFF manufacturer specific */
} canmat_402_motor_type_t;

/** Bit mask for the "Supported drive modes" dictionary object */
typedef enum canmat_402_supported_drive_modes_mask {
    CANMAT_402_SUPPORTED_DRIVE_MODES_MASK_PP   = 1 << 0, ///< Profile Position
    CANMAT_402_SUPPORTED_DRIVE_MODES_MASK_VL   = 1 << 1, ///< Velocity
    CANMAT_402_SUPPORTED_DRIVE_MODES_MASK_PV   = 1 << 2, ///< Profile Velocity
    CANMAT_402_SUPPORTED_DRIVE_MODES_MASK_TQ   = 1 << 3, ///< Torque
    /* bit 4 reserved */
    CANMAT_402_SUPPORTED_DRIVE_MODES_MASK_HM   = 1 << 5, ///< Homing
    CANMAT_402_SUPPORTED_DRIVE_MODES_MASK_IP   = 1 << 6, ///< Interpolated position
    /* bits 7-15 reserved */
    /* bits 16-31 manufacturer specific */
} canmat_402_supported_drive_modes_mask_;

typedef enum canmat_402_digital_input_mask {
    CANMAT_402_DIGITAL_INPUT_MASK_NEG_LIMIT_SW = 1 << 0,
    CANMAT_402_DIGITAL_INPUT_MASK_POS_LIMIT_SW = 1 << 1,
    CANMAT_402_DIGITAL_INPUT_MASK_HOME_SW      = 1 << 2,
    CANMAT_402_DIGITAL_INPUT_MASK_INTERLOCK    = 1 << 3,
    /* bits 4-15 reserved */
    /* bits 16-31 manufacturer specific */
} canmat_402_digital_input_mask_t;


typedef enum canmat_402_ctrlmask {
    CANMAT_402_CTRLMASK_SWITCH_ON               = 1 <<  0,
    CANMAT_402_CTRLMASK_ENABLE_VOLTAGE          = 1 <<  1,
    CANMAT_402_CTRLMASK_QUICK_STOP              = 1 <<  2,
    CANMAT_402_CTRLMASK_ENABLE_OPERATION        = 1 <<  3,
    CANMAT_402_CTRLMASK_OP_MODE_SPECIFIC0       = 1 <<  4,
    CANMAT_402_CTRLMASK_OP_MODE_SPECIFIC1       = 1 <<  5,
    CANMAT_402_CTRLMASK_OP_MODE_SPECIFIC2       = 1 <<  6,
    CANMAT_402_CTRLMASK_RESET_FAULT             = 1 <<  7,
    CANMAT_402_CTRLMASK_HALT                    = 1 <<  8,
    CANMAT_402_CTRLMASK_RESERVED0               = 1 <<  9,
    CANMAT_402_CTRLMASK_RESERVED1               = 1 << 10,
    CANMAT_402_CTRLMASK_MANUFACTURER_SPECIFIC0  = 1 << 11,
    CANMAT_402_CTRLMASK_MANUFACTURER_SPECIFIC1  = 1 << 12,
    CANMAT_402_CTRLMASK_MANUFACTURER_SPECIFIC2  = 1 << 13,
    CANMAT_402_CTRLMASK_MANUFACTURER_SPECIFIC3  = 1 << 14,
    CANMAT_402_CTRLMASK_MANUFACTURER_SPECIFIC4  = 1 << 15
} canmat_402_ctrlmask_t;

/** Velocity-mode controlword masks */
typedef enum canmat_402_ctrlmask_vl {
    CANMAT_402_CTRLMASK_VL_RFG_ENABLE  = 1 << 4,
    CANMAT_402_CTRLMASK_VL_RFG_UNLOCK  = 1 << 5,
    CANMAT_402_CTRLMASK_VL_RFG_USE_REF = 1 << 6,
} canmat_402_ctrlmask_vl_t;

/** Profile position mode controlword masks */
typedef enum canmat_402_ctrlmask_pp {
    CANMAT_402_CTRLMASK_PP_NEW_SET_POINT          = 1 << 4,
    CANMAT_402_CTRLMASK_PP_CHANGE_SET_IMMEDIATELY = 1 << 5,
    CANMAT_402_CTRLMASK_PP_ABS_REL                = 1 << 6,
} canmat_402_ctrlmask_pp_t;

/** Profile velocity mode controlword masks all reserved */
/** Profile torque mode controlword masks all reserved */

/** Homing mode controlword masks */
typedef enum canmat_402_ctrlmask_hm {
    CANMAT_402_CTRLMASK_HM_OP_START        = 1 << 4,
    /* bits 5 and 6 reserved */
} canmat_402_ctrlmask_hm_t;


/** Interpolation position mode controlword masks */
typedef enum canmat_402_ctrlmask_ip {
    CANMAT_402_CTRLMASK_IP_ENABLe        = 1 << 4,
    /* bits 5 and 6 reserved */
} canmat_402_ctrlmask_ip_t;

/** Bit masks to produce specific control commands.
 *
 * To build the control word for the command, bitwise and the word
 * with the MASK_AND and bitwise or the word with the MASK_OR */

typedef enum canmat_402_ctrlcmd_mask {
    CANMAT_402_CTRLCMD_MASK_AND_SHUTDOWN  = (~CANMAT_402_CTRLMASK_RESET_FAULT & ~CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_OR_SHUTDOWN   = (CANMAT_402_CTRLMASK_QUICK_STOP | CANMAT_402_CTRLMASK_ENABLE_VOLTAGE),
    /* Need to set the "enable operation" bit independently */
    CANMAT_402_CTRLCMD_MASK_AND_SWITCH_ON = (~CANMAT_402_CTRLMASK_RESET_FAULT),
    CANMAT_402_CTRLCMD_MASK_OR_SWITCH_ON  = (CANMAT_402_CTRLMASK_QUICK_STOP | CANMAT_402_CTRLMASK_ENABLE_VOLTAGE |
                                            CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_AND_DISABLE_VOLTAGE = (~CANMAT_402_CTRLMASK_RESET_FAULT &
                                                  ~CANMAT_402_CTRLMASK_ENABLE_VOLTAGE),
    CANMAT_402_CTRLCMD_MASK_OR_DISABLE_VOLTAGE  = 0,
    CANMAT_402_CTRLCMD_MASK_AND_QUICK_STOP  = (~CANMAT_402_CTRLMASK_RESET_FAULT & ~CANMAT_402_CTRLMASK_QUICK_STOP),
    CANMAT_402_CTRLCMD_MASK_OR_QUICK_STOP   = CANMAT_402_CTRLMASK_ENABLE_VOLTAGE,
    CANMAT_402_CTRLCMD_MASK_AND_DISABLE_OP  = (~CANMAT_402_CTRLMASK_RESET_FAULT & ~CANMAT_402_CTRLMASK_ENABLE_OPERATION),
    CANMAT_402_CTRLCMD_MASK_OR_DISABLE_OP   = (CANMAT_402_CTRLMASK_QUICK_STOP | CANMAT_402_CTRLMASK_ENABLE_VOLTAGE |
                                              CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_AND_ENSABLE_OP  = (~CANMAT_402_CTRLMASK_RESET_FAULT),
    CANMAT_402_CTRLCMD_MASK_OR_ENSABLE_OP   = (CANMAT_402_CTRLMASK_ENABLE_OPERATION| CANMAT_402_CTRLMASK_QUICK_STOP |
                                              CANMAT_402_CTRLMASK_ENABLE_VOLTAGE | CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_AND_RESET_FAULT = 0xFFFF,
    CANMAT_402_CTRLCMD_MASK_OR_RESET_FAULT  = CANMAT_402_CTRLMASK_RESET_FAULT
} canmat_402_ctrlcmd_mask_t;

#define CANMAT_402_INDEX_STATUSWORD     0x6041

enum canmat_402_statmask {
    CANMAT_402_STATMASK_READY_TO_SWITCH_ON       = 1 <<  0,
    CANMAT_402_STATMASK_SWITCHED_ON              = 1 <<  1,
    CANMAT_402_STATMASK_OP_ENABLED               = 1 <<  2,
    CANMAT_402_STATMASK_FAULT                    = 1 <<  3,
    CANMAT_402_STATMASK_VOLTAGE_ENABLED          = 1 <<  4,
    CANMAT_402_STATMASK_QUICK_STOP               = 1 <<  5,
    CANMAT_402_STATMASK_SWITCH_ON_DISABLED       = 1 <<  6,
    CANMAT_402_STATMASK_WARNING                  = 1 <<  7,
    CANMAT_402_STATMASK_MANUFACTURER_SPECIFIC0   = 1 <<  8,
    CANMAT_402_STATMASK_REMOTE                   = 1 <<  9,
    CANMAT_402_STATMASK_TARGET_REACHED           = 1 << 10,
    CANMAT_402_STATMASK_INTERNAL_LIMIT_ACTIVE    = 1 << 11,
    CANMAT_402_STATMASK_OP_MODE_SPECIFIC0        = 1 << 12,
    CANMAT_402_STATMASK_OP_MODE_SPECIFIC1        = 1 << 13,
    CANMAT_402_STATMASK_MANUFACTURER_SPECIFIC1   = 1 << 14,
    CANMAT_402_STATMASK_MANUFACTURER_SPECIFIC2   = 1 << 15
} canmat_402_statmask_t;


/** Canopen State machine.
 *
 * Set using the ControWord. Read using the status word.  Values of
 * the enum are the status word with don't care bits masked out.
 */
typedef enum canmat_402_state_val {
    CANMAT_402_STATE_VAL_OFF_NRDY              = 0x00,
    CANMAT_402_STATE_VAL_OFF_SW_ON_DISABLE     = 0x40,
    CANMAT_402_STATE_VAL_OFF_RDY               = 0x21,
    CANMAT_402_STATE_VAL_ON_OP_DIS             = 0x23,
    CANMAT_402_STATE_VAL_ON_OP_EN              = 0x27,
    CANMAT_402_STATE_VAL_ON_QUICK_STOP         = 0x03,
    CANMAT_402_STATE_VAL_FAULT_REACTION_ACTIVE = 0x0F,
    CANMAT_402_STATE_VAL_FAULT                 = 0x08,
    CANMAT_402_STATE_VAL_UNKNOWN               = 0xFFFF
} canmat_402_state_val_t;

/** Mask out don't care bits for mapping status_word to drive state
 *  See table 19, p. 41.
 */
typedef enum canmat_402_state_val_mask {
    CANMAT_402_STATE_VAL_MASK_OFF_NRDY              = ~0x4F,
    CANMAT_402_STATE_VAL_MASK_OFF_SW_ON_DISABLE     = ~0x4F,
    CANMAT_402_STATE_VAL_MASK_OFF_RDY               = ~0x6F,
    CANMAT_402_STATE_VAL_MASK_ON_OP_DIS             = ~0x6F,
    CANMAT_402_STATE_VAL_MASK_ON_OP_EN              = ~0x6F,
    CANMAT_402_STATE_VAL_MASK_ON_QUICK_STOP         = ~0x6F,
    CANMAT_402_STATE_VAL_MASK_FAULT_REACTION_ACTIVE = ~0x4F,
    CANMAT_402_STATE_VAL_MASK_FAULT                 = ~0x4F
} canmat_402_state_val_mask_t;

typedef enum canmat_402_fault_reaction {
    /* -32768 to -1 manufacturer specific */
    CANMAT_402_FAULT_REACTION_DISABLE           = 0,
    CANMAT_402_FAULT_REACTION_SLOW_DOWN_RAMP    = 1,
    CANMAT_402_FAULT_REACTION_QUICK_STOP_RAMP   = 2,
    CANMAT_402_FAULT_REACTION_CURRENT_LIMIT     = 3,
    CANMAT_402_FAULT_REACTION_VOLTAGE_LIMIT     = 4,
    /* 5 - 32767 reserved */
} canmat_402_fault_reaction_t;

enum canmat_402_op_mode {
    /* -1 to -128 manufacturer specific */
    CANMAT_402_OP_MODE_POSITION             = 0x1,
    CANMAT_402_OP_MODE_VELOCITY             = 0x3,
    CANMAT_402_OP_MODE_CURRENT              = 0x4,
    CANMAT_402_OP_MODE_RESERVED1            = 0x5,
    CANMAT_402_OP_MODE_HOMING               = 0x6,
    CANMAT_402_OP_MODE_PVT                  = 0x7, /* Interpolated Position */
    CANMAT_402_OP_MODE_CYCLIC_SYNC_POSITION = 0x8,
    CANMAT_402_OP_MODE_CYCLIC_SYNC_VELOCITY = 0x9,
    CANMAT_402_OP_MODE_CYCLIC_SYNC_TORQUE   = 0xA,
    //11 ... 127 reserved
    CANMAT_402_OP_MODE_CUSTOM               = 0xff
} amccan_op_mode_t;

enum canmat_402_polarity_mask {
    CANMAT_402_POLARITY_MASK_VEL = 1 << 6,
    CANMAT_402_POLARITY_MASK_POS = 1 << 7
} canmat_402_polarity_mask_t;

#define CANMAT_402_POLARITY_FACTOR(bit) ((bit) ? -1 : 1)

#define CANMAT_402_POLARITY_FACTOR_VEL(polarity) \
    CANMAT_402_POLARITY_FACTOR( (polarity) & CANMAT_402_POLARITY_MASK_VEL )
#define CANMAT_402_POLARITY_FACTOR_POS(polarity) \
    CANMAT_402_POLARITY_FACTOR( (polarity) & CANMAT_402_POLARITY_MASK_POS )

canmat_obj_print_fun* canmat_402_print_lookup( const char *name );

#ifdef __cplusplus
}
#endif

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
#endif // SOCANMATIC_402_H
