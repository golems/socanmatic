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


#ifndef SOCIA_301_H
#define SOCIA_301_H

#ifdef __cplusplus
extern "C" {
#endif


const char *socia_sdo_abort_code2str( uint32_t code );

typedef enum socia_emcy_class {
    SOCIA_EMCY_CODE_CLASS_NO_ERROR      = 0x0000,
    SOCIA_EMCY_CODE_CLASS_GENERIC       = 0x1000,
    SOCIA_EMCY_CODE_CURRENT             = 0x2000,
    SOCIA_EMCY_CODE_CURRENT_INPUT       = 0x2100,
    SOCIA_EMCY_CODE_CURRENT_INSIDE      = 0x2200,
    SOCIA_EMCY_CODE_CURRENT_OUTPUT      = 0x2300,
    SOCIA_EMCY_CODE_VOLTAGE             = 0x3000,
    SOCIA_EMCY_CODE_VOLTAGE_MAINS       = 0x3100,
    SOCIA_EMCY_CODE_VOLTAGE_INSIDE      = 0x3200,
    SOCIA_EMCY_CODE_VOLTAGE_OUTPUT      = 0x3300,
    SOCIA_EMCY_CODE_TEMP                = 0x4000,
    SOCIA_EMCY_CODE_TEMP_AMBIENT        = 0x4100,
    SOCIA_EMCY_CODE_TEMP_DEVICE         = 0x4200,
    SOCIA_EMCY_CODE_HARDWARE            = 0x5000,
    SOCIA_EMCY_CODE_SOFTWARE            = 0x6000,
    SOCIA_EMCY_CODE_SOFTWARE_INTERNAL   = 0x6100,
    SOCIA_EMCY_CODE_SOFTWARE_USER       = 0x6200,
    SOCIA_EMCY_CODE_SOFTWARE_DATA       = 0x6300,
    SOCIA_EMCY_CODE_ADDITIONAL_MODULES  = 0x7000,
    SOCIA_EMCY_CODE_MONITORING          = 0x8000,
    SOCIA_EMCY_CODE_MONITORING_COMM     = 0x8100,
    SOCIA_EMCY_CODE_MONITORING_PROTO    = 0x8100,
    SOCIA_EMCY_CODE_EXTERNAL            = 0x9000,
    SOCIA_EMCY_CODE_ADDITIONAL_FUNC     = 0xF000,
    SOCIA_EMCY_CODE_DEVICE_SPECIFIC     = 0xFF00
} socia_emcy_class_t;


typedef enum socia_error_reg_mask {
    SOCIA_ERROR_REG_MASK_ERROR     = 1 << 0,
    SOCIA_ERROR_REG_MASK_CURRENT   = 1 << 1,
    SOCIA_ERROR_REG_MASK_VOLTAGE   = 1 << 2,
    SOCIA_ERROR_REG_MASK_TEMP      = 1 << 3,
    SOCIA_ERROR_REG_MASK_COMM      = 1 << 4,
    SOCIA_ERROR_REG_MASK_DEV_PROF  = 1 << 5,
    SOCIA_ERROR_REG_MASK_RESERVED  = 1 << 6,
    SOCIA_ERROR_REG_MASK_MFR       = 1 << 7,
} socia_error_reg_mask_t;


typedef enum socia_cobid_sync_mask {
    SOCIA_COBID_SYNC_MASK_FRAME  = 1 << 29,
    SOCIA_COBID_SYNC_MASK_GEN    = 1 << 30,
} socia_cobid_sync_mask_t;

typedef enum socia_frame_type {
    SOCIA_FRAME_TYPE_BASE  = 0, ///< 11-bit CAN ID
    SOCIA_FRAME_TYPE_EXT   = 1  ///< 29-bit CAN ID
} socia_frame_type_t;

typedef enum socia_cobid_sync_gen {
    SOCIA_COBID_SYNC_GEN_DEV_NO_GEN  = 0,  ///< device does not generate sync
    SOCIA_COBID_SYNC_GEN_DEV_GEN     = 1,  ///< device generates sync
} socia_cobid_sync_gen_t;

/** The value "save" when written little endian */
#define  SOCIA_STORE_PARAM_SAVE      \
    ((0x73)         |                \
     (0x61 << 8)    |                \
     (0x76 << 16)   |                \
     (0x65 << 24))

/** The value "load" when written little endian */
#define  SOCIA_RESTORE_PARAM_LOAD    \
    ((0x6C)         |                \
     (0x6F << 8)    |                \
     (0x71 << 16)   |                \
     (0x64 << 24))


typedef enum socia_store_param_mask {
    SOCIA_STORE_PARAM_MASK_CMD  = 1 << 0, /* device stores parameters autonomously */
    SOCIA_STORE_PARAM_MASK_AUTO = 1 << 0  /* device saves parameters on command */
} socia_store_param_mask_t

typedef enum socia_cobid_time_mask {
    SOCIA_COBID_TIME_MASK_FRAME   = 1 << 29,
    SOCIA_COBID_TIME_MASK_PRODUCE = 1 << 30,
    SOCIA_COBID_TIME_MASK_CONSUME = 1 << 30
} socia_cobid_time_mask_t;

typedef enum socia_cobid_emcy_mask {
    SOCIA_COBID_EMCY_MASK_FRAME = 1 << 29,
    SOCIA_COBID_EMCY_MASK_VALID = 1 << 31
} socia_cobid_emcy_mask_t;


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
