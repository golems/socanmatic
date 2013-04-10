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
