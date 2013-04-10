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


#ifndef SOCANMATIC_301_H
#define SOCANMATIC_301_H

#ifdef __cplusplus
extern "C" {
#endif

/** COB-ID mask for node bits */
#define CANMAT_NODE_MASK ((uint16_t)0x7F)

/* ID for all nodes */
#define CANMAT_NODE_ALL 0x00

static inline uint16_t canmat_frame_func( const struct can_frame *can ) {
    return can->can_id & (~CANMAT_NODE_MASK & 0xFFFF);
}

static inline uint8_t canmat_frame_node( const struct can_frame *can ) {
    return can->can_id & CANMAT_NODE_MASK;
}

const char *canmat_sdo_abort_code2str( uint32_t code );


typedef enum canmat_cobid_sync_mask {
    CANMAT_COBID_SYNC_MASK_FRAME  = 1 << 29,
    CANMAT_COBID_SYNC_MASK_GEN    = 1 << 30,
} canmat_cobid_sync_mask_t;

typedef enum canmat_frame_type {
    CANMAT_FRAME_TYPE_BASE  = 0, ///< 11-bit CAN ID
    CANMAT_FRAME_TYPE_EXT   = 1  ///< 29-bit CAN ID
} canmat_frame_type_t;

typedef enum canmat_cobid_sync_gen {
    CANMAT_COBID_SYNC_GEN_DEV_NO_GEN  = 0,  ///< device does not generate sync
    CANMAT_COBID_SYNC_GEN_DEV_GEN     = 1,  ///< device generates sync
} canmat_cobid_sync_gen_t;

/** The value "save" when written little endian */
#define  CANMAT_STORE_PARAM_SAVE                  \
    ((0x73)         |                           \
     (0x61 << 8)    |                           \
     (0x76 << 16)   |                           \
     (0x65 << 24))

/** The value "load" when written little endian */
#define  CANMAT_RESTORE_PARAM_LOAD                \
    ((0x6C)         |                           \
     (0x6F << 8)    |                           \
     (0x71 << 16)   |                           \
     (0x64 << 24))

/* 29 bits of masking */
#define CANMAT_COBID_MASK 0x1FFFFFFF

typedef enum canmat_store_param_mask {
    CANMAT_STORE_PARAM_MASK_CMD  = 1 << 0, /* device stores parameters autonomously */
    CANMAT_STORE_PARAM_MASK_AUTO = 1 << 0  /* device saves parameters on command */
} canmat_store_param_mask_t;

typedef enum canmat_cobid_time_mask {
    CANMAT_COBID_TIME_MASK_FRAME   = 1 << 29,
    CANMAT_COBID_TIME_MASK_PRODUCE = 1 << 30,
    CANMAT_COBID_TIME_MASK_CONSUME = 1 << 30
} canmat_cobid_time_mask_t;

typedef enum canmat_cobid_emcy_mask {
    CANMAT_COBID_EMCY_MASK_FRAME = 1 << 29,
    CANMAT_COBID_EMCY_MASK_VALID = 1 << 31
} canmat_cobid_emcy_mask_t;

typedef enum canmat_cobid_pdo_mask {
    CANMAT_COBID_PDO_MASK_FRAME = 1 << 29,
    /* 30 is reserved */
    CANMAT_COBID_PDO_MASK_VALID = 1 << 31
} canmat_cobid_pdo_mask_t;

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
