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
    CANMAT_402_CTRLCMD_MASK_AND_SHUTDOWN  = (uint16_t)(~CANMAT_402_CTRLMASK_RESET_FAULT &
                                                       ~CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_OR_SHUTDOWN   = (CANMAT_402_CTRLMASK_QUICK_STOP | CANMAT_402_CTRLMASK_ENABLE_VOLTAGE),
    /* Need to set the "enable operation" bit independently */
    CANMAT_402_CTRLCMD_MASK_AND_SWITCH_ON = (uint16_t)(~CANMAT_402_CTRLMASK_RESET_FAULT &
                                                       ~CANMAT_402_CTRLMASK_ENABLE_OPERATION),
    CANMAT_402_CTRLCMD_MASK_OR_SWITCH_ON  = (CANMAT_402_CTRLMASK_QUICK_STOP | CANMAT_402_CTRLMASK_ENABLE_VOLTAGE |
                                             CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_AND_DISABLE_VOLTAGE = (uint16_t)(~CANMAT_402_CTRLMASK_RESET_FAULT &
                                                             ~CANMAT_402_CTRLMASK_ENABLE_VOLTAGE),
    CANMAT_402_CTRLCMD_MASK_OR_DISABLE_VOLTAGE  = 0,
    CANMAT_402_CTRLCMD_MASK_AND_QUICK_STOP  = (uint16_t)(~CANMAT_402_CTRLMASK_RESET_FAULT &
                                                         ~CANMAT_402_CTRLMASK_QUICK_STOP),
    CANMAT_402_CTRLCMD_MASK_OR_QUICK_STOP   = CANMAT_402_CTRLMASK_ENABLE_VOLTAGE,
    CANMAT_402_CTRLCMD_MASK_AND_DISABLE_OP  = (uint16_t)(~CANMAT_402_CTRLMASK_RESET_FAULT &
                                                         ~CANMAT_402_CTRLMASK_ENABLE_OPERATION),
    CANMAT_402_CTRLCMD_MASK_OR_DISABLE_OP   = (CANMAT_402_CTRLMASK_QUICK_STOP | CANMAT_402_CTRLMASK_ENABLE_VOLTAGE |
                                               CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_AND_ENABLE_OP  = (uint16_t)(~CANMAT_402_CTRLMASK_RESET_FAULT),
    CANMAT_402_CTRLCMD_MASK_OR_ENABLE_OP   = (CANMAT_402_CTRLMASK_ENABLE_OPERATION | CANMAT_402_CTRLMASK_QUICK_STOP |
                                              CANMAT_402_CTRLMASK_ENABLE_VOLTAGE | CANMAT_402_CTRLMASK_SWITCH_ON),
    CANMAT_402_CTRLCMD_MASK_AND_RESET_FAULT = 0xFFFF,
    CANMAT_402_CTRLCMD_MASK_OR_RESET_FAULT  = CANMAT_402_CTRLMASK_RESET_FAULT
} canmat_402_ctrlcmd_mask_t;

#define CANMAT_402_INDEX_STATUSWORD     0x6041

/** Canopen State machine.
 *
 * Set using the ControWord. Read using the status word.  Values of
 * the enum are the status word with don't care bits masked out.
 */
typedef enum canmat_402_state_val {
    CANMAT_402_STATE_VAL_OFF_NRDY              = 0x00,  ///< not ready to switch on
    CANMAT_402_STATE_VAL_OFF_SW_ON_DISABLE     = 0x40,  ///< switch on disabled
    CANMAT_402_STATE_VAL_OFF_RDY               = 0x21,  ///< ready to switch on
    CANMAT_402_STATE_VAL_ON_OP_DIS             = 0x23,  ///< switched on
    CANMAT_402_STATE_VAL_ON_OP_EN              = 0x27,  ///< operation enabled
    CANMAT_402_STATE_VAL_ON_QUICK_STOP         = 0x07,  ///< quick stop active
    CANMAT_402_STATE_VAL_FAULT_REACTION_ACTIVE = 0x0F,  ///< fault reaction active
    CANMAT_402_STATE_VAL_FAULT                 = 0x08,  ///< fault
    CANMAT_402_STATE_VAL_UNKNOWN               = 0xFFFF ///< Unknown state
} canmat_402_state_val_t;

/** Mask out don't care bits for mapping status_word to drive state
 *  See table 19, p. 41.
 */
typedef enum canmat_402_state_val_mask {
    CANMAT_402_STATE_VAL_MASK_OFF_NRDY              = 0x4F,
    CANMAT_402_STATE_VAL_MASK_OFF_SW_ON_DISABLE     = 0x4F,
    CANMAT_402_STATE_VAL_MASK_OFF_RDY               = 0x6F,
    CANMAT_402_STATE_VAL_MASK_ON_OP_DIS             = 0x6F,
    CANMAT_402_STATE_VAL_MASK_ON_OP_EN              = 0x6F,
    CANMAT_402_STATE_VAL_MASK_ON_QUICK_STOP         = 0x6F,
    CANMAT_402_STATE_VAL_MASK_FAULT_REACTION_ACTIVE = 0x4F,
    CANMAT_402_STATE_VAL_MASK_FAULT                 = 0x4F
} canmat_402_state_val_mask_t;

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


struct canmat_402_drive {
    uint8_t node_id;
    uint16_t ctrl_word;
    uint16_t stat_word;

    enum canmat_402_op_mode op_mode;

    uint32_t abort_code;

    double pos_factor;
    double vel_factor;
    double cur_factor;

    int32_t actual_pos_raw;
    int32_t actual_vel_raw;
    int32_t actual_cur_raw;

    int16_t target_vel_raw;

    double pos_max_soft;
    double pos_min_soft;
    double pos_max_hard;
    double pos_min_hard;

    double actual_pos;
    double actual_vel;
    double actual_cur;


    // PDO number (zero indexed) to configure based on the selected op mode
    int rpdo_user;

    // PDO number (zero indexed) to configure for feedback
    int tpdo_user;
    int tpdo_stat;

    int rpdo_ctrl; ///< RPDO for setting control word
};

///< initialize drive variables in struct
enum canmat_402_state_val canmat_402_state( const struct canmat_402_drive *drive );

///< initialize drive variables in struct
enum canmat_status canmat_402_init( struct canmat_iface *cif, uint8_t id, struct canmat_402_drive *drive );

///< start the drive
enum canmat_status canmat_402_start( struct canmat_iface *cif, struct canmat_402_drive *drive );

///< stop the drive
enum canmat_status canmat_402_stop( struct canmat_iface *cif, struct canmat_402_drive *drive );

///< set control mask
enum canmat_status canmat_402_dl_ctrlmask( struct canmat_iface *cif, struct canmat_402_drive *drive ,
                                           uint16_t mask_and, uint16_t mask_or );

///< set op mode and configure RPDO to receive
enum canmat_status canmat_402_set_op_mode( struct canmat_iface *cif, struct canmat_402_drive *drive,
                                           enum canmat_402_op_mode op_mode );

///< Add active RPDOs to the descriptor table
enum canmat_status canmat_402_probe_pdo(
    struct canmat_iface *cif, struct canmat_pdo_descriptor_table tab,
    const struct canmat_402_drive *drive );

const char *canmat_402_state_string( enum canmat_402_state_val s );

#define CANMAT_402_CTRLMASK_VL_RFG_ENABLE  CANMAT_402_CTRLMASK_OP_MODE_SPECIFIC0
#define CANMAT_402_CTRLMASK_VL_RFG_UNLOCK  CANMAT_402_CTRLMASK_OP_MODE_SPECIFIC1
#define CANMAT_402_CTRLMASK_VL_RFG_USE_REF CANMAT_402_CTRLMASK_OP_MODE_SPECIFIC2

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
