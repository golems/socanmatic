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

#define SOCIA_402_INDEX_CONTROLWORD     0x6040
#define SOCIA_402_SUBINDEX_CONTROLWORD  0x00

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

enum socia_402_op_mode {
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
