/*
 * Copyright (c) 2008-2013, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Neil T. Dantam <ntd@gatech.edu>
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
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef SOCANMATIC_SDO_H
#define SOCANMATIC_SDO_H

#ifdef __cplusplus
extern "C" {
#endif

/** Make COB-ID for request (master->slave) */
#define CANMAT_SDO_REQ_ID(node)  ((canid_t)( ((node)&CANMAT_NODE_MASK) | CANMAT_FUNC_CODE_SDO_RX))

/** Make COB-ID for response (slave->master) */
#define CANMAT_SDO_RESP_ID(node) ((canid_t)( ((node)&CANMAT_NODE_MASK) | CANMAT_FUNC_CODE_SDO_TX))

#define CANMAT_CS_ABORT 4

enum canmat_ccs {
    CANMAT_CCS_SEG_DL  = 0, ///< segment download
    CANMAT_CCS_EX_DL   = 1, ///< expedited download (master->node)
    CANMAT_CCS_EX_UL   = 2, ///< expedited upload (node->master)
    CANMAT_CCS_SEG_UL  = 3, ///< segment upload
    CANMAT_CCS_ABORT   = CANMAT_CS_ABORT,
    CANMAT_CCS_BLK_UL  = 5, ///< Block Upload
    CANMAT_CCS_BLK_DL  = 6  ///< Block Download
};

enum canmat_scs {
    CANMAT_SCS_SEG_UL   = 0,   ///< Segment Download
    CANMAT_SCS_SEG_DL   = 1,   ///< Segment Download
    CANMAT_SCS_EX_UL    = 2,   ///< Expredited Upload Response
    CANMAT_SCS_EX_DL    = 3,   ///< Expedited Download response
    CANMAT_SCS_ABORT    = CANMAT_CS_ABORT,
    CANMAT_SCS_BLK_DL   = 5,   ///< Block Download response
    CANMAT_SCS_BLK_UL   = 6,   ///< Block Upload response
};

struct canmat_sdo_cmd_ex {
    unsigned s  : 1;      ///< size
    unsigned e  : 1;      ///< expedited
    unsigned n  : 2;      ///< no-data length
    unsigned x  : 1;      ///< unused
    unsigned cs : 3;      ///< command specifier
};


typedef enum canmat_command_byte {
    CANMAT_SDO_CMD_DL1 = 0x2f,
    CANMAT_SDO_CMD_DL2 = 0x2b,
    CANMAT_SDO_CMD_DL3 = 0x27,
    CANMAT_SDO_CMD_DL4 = 0x23,
    CANMAT_SDO_CMD_DL0 = 0x22,

    CANMAT_SDO_CMD_UL1 = 0x4F,
    CANMAT_SDO_CMD_UL2 = 0x4B,
    CANMAT_SDO_CMD_UL3 = 0x47,
    CANMAT_SDO_CMD_UL4 = 0x43,
    CANMAT_SDO_CMD_UL0 = 0x42,

    CANMAT_SDO_CMD_DL_ACK = 0x60,

    CANMAT_SDO_CMD_ABORT = 0x80
} canmat_command_byte_t;

typedef enum canmat_abort {
    CANMAT_ABORT_RESET = 0x00000000,
    CANMAT_ABORT_TOGGLE_NOT_ALTERNATED = 0x05030000,
    CANMAT_ABORT_SDO_TIMEOUT = 0x05040000,
    CANMAT_ABORT_INVALID_CMD_SPEC = 0x05040001,
    CANMAT_ABORT_INVALID_BLOCK_SIZE = 0x05040002,
    CANMAT_ABORT_INVALID_SEQ_NO = 0x05040003,
    CANMAT_ABORT_CRC = 0x05040004,
    CANMAT_ABORT_OOM = 0x05040005,
    CANMAT_ABORT_UNSUPP_ACCESS = 0x06010000,
    CANMAT_ABORT_WRITE_ONLY = 0x06010001,
    CANMAT_ABORT_READ_ONLY = 0x06010002,
    CANMAT_ABORT_OBJ_EXIST = 0x06020000,
    CANMAT_ABORT_UNMAPPABLE = 0x06040041,
    CANMAT_ABORT_PDO_OVERFLOW = 0x06040042,
    CANMAT_ABORT_INCOMPAT_PARAM = 0x06040043,
    CANMAT_ABORT_INCOMPAT_DEV = 0x06040047,
    CANMAT_ABORT_ERR_HW = 0x06060000,
    CANMAT_ABORT_DATA = 0x06070010,
    CANMAT_ABORT_DATA_TOO_HI = 0x06070012,
    CANMAT_ABORT_DATA_TOO_LO = 0x06070013,
    CANMAT_ABORT_SUBINDEX_EXIST =  0x06090011,
    CANMAT_ABORT_RANGE =  0x06090030,
    CANMAT_ABORT_RANGE_TOO_HI =  0x06090031,
    CANMAT_ABORT_RANGE_TOO_LO =  0x06090032,
    CANMAT_ABORT_MAX_BELOW_MIN = 0x06090036,
    CANMAT_ABORT_GENERAL = 0x08000000,
    CANMAT_ABORT_STORE = 0x08000020,
    CANMAT_ABORT_STORE_LOCAL = 0x08000021,
    CANMAT_ABORT_STORE_DEV_STATE = 0x08000022,
    CANMAT_ABORT_DICT = 0x08000023
} canmat_abort_t;

/** Container struct for SDO requests.
 *
 */
typedef struct canmat_sdo_msg {
    uint16_t index;        ///< CANopen index
    uint8_t subindex;      ///< CANopen subindex
    uint8_t node;          ///< CANopen Node ID
    unsigned cmd_spec : 3; ///< Command Specifier
    enum canmat_data_type data_type; ///< data type stored in the SDO
    uint8_t length;        ///< CANopen length of data
    union canmat_scalar data;
} canmat_sdo_msg_t;


/// Create a struct can_frame from a canmat_sdo_msg_t
enum canmat_status canmat_sdo2can (struct can_frame *dst, const canmat_sdo_msg_t *src, const int is_response );

static inline uint16_t canmat_can2sdo_index( const struct can_frame *src ) {
    return (uint16_t)(src->data[1] + (src->data[2] << 8));
}

static inline uint8_t canmat_can2sdo_subindex( const struct can_frame *src ) {
    return src->data[3];
}

static inline enum canmat_command_byte canmat_can2sdo_cmd( const struct can_frame *src ) {
    return (enum canmat_command_byte) (src->data[0]);
}

static inline struct canmat_sdo_cmd_ex canmat_can2sdo_cmd_ex( const struct can_frame *src ) {
    unsigned c = src->data[0];
    struct canmat_sdo_cmd_ex cmd = { .s = c & 0x1,
                                     .e = (c >> 1) & 1,
                                     .n = (c >> 2) & 0x3,
                                     .cs = (c >> 5) & 0x7 };
    return cmd;
}

static inline void canmat_sdo2can_cmd_ex( struct can_frame *dst, const struct canmat_sdo_cmd_ex cmd ) {
    dst->data[0] = (uint8_t)( (cmd.s ? 1 : 0)            |
                              ((cmd.e ? 1 : 0) << 1)     |
                              ((cmd.n & 0x3)   << 2)      |
                              ((cmd.cs & 0x7)  << 5) );
}

/// Create a canmat_sdo_msg_t from a struct can_frame
enum canmat_status canmat_can2sdo(
    canmat_sdo_msg_t *dst, const struct can_frame *src,
    enum canmat_data_type data_type );

canmat_status_t canmat_sdo_ul( canmat_iface_t *cif, const canmat_sdo_msg_t *req,
                               canmat_sdo_msg_t *resp );

canmat_status_t canmat_sdo_dl( canmat_iface_t *cif, const canmat_sdo_msg_t *req,
                               canmat_sdo_msg_t *resp );



#define CANMAT_SDO_TYPE_FUN( T_CAN, T_SHORT )                           \
    static inline canmat_status_t canmat_sdo_ul_ ## T_SHORT(            \
        canmat_iface_t *cif, uint8_t node, uint16_t idx, uint8_t subix, \
        CANMAT_##T_CAN *val, uint32_t *err )                            \
    {                                                                   \
        canmat_sdo_msg_t resp;                                          \
        canmat_sdo_msg_t req = { .index = idx,                          \
                                 .subindex = subix,                     \
                                 .node = node,                          \
                                 .data_type = CANMAT_DATA_TYPE_##T_CAN  \
        };                                                              \
        canmat_status_t r = canmat_sdo_ul( cif, &req, &resp );          \
        if( CANMAT_OK == r ) {                                          \
            *val = resp.data.T_SHORT;                                   \
        } else if (CANMAT_ERR_ABORT == r && NULL != err ) {             \
            *err = resp.data.u32;                                       \
        }                                                               \
        return r;                                                       \
    }                                                                   \
    static inline canmat_status_t canmat_sdo_dl_ ## T_SHORT (           \
        canmat_iface_t *cif, uint8_t node, uint16_t idx, uint8_t subix, \
        CANMAT_##T_CAN val, uint32_t *err )                             \
    {                                                                   \
        canmat_sdo_msg_t resp;                                          \
        canmat_sdo_msg_t req = { .index = idx,                          \
                                 .subindex = subix,                     \
                                 .node = node,                          \
                                 .data_type = CANMAT_DATA_TYPE_##T_CAN  \
        };                                                              \
        req.data.T_SHORT = val;                                         \
        canmat_status_t r = canmat_sdo_dl( cif, &req, &resp );          \
        if( CANMAT_ERR_ABORT == r && NULL != err ) {                    \
            *err = resp.data.u32;                                       \
        }                                                               \
        return r;                                                       \
    }

CANMAT_SDO_TYPE_FUN( UNSIGNED8,  u8 )
CANMAT_SDO_TYPE_FUN( UNSIGNED16, u16 )
CANMAT_SDO_TYPE_FUN( UNSIGNED32, u32 )

CANMAT_SDO_TYPE_FUN( INTEGER8,  i8 )
CANMAT_SDO_TYPE_FUN( INTEGER16, i16 )
CANMAT_SDO_TYPE_FUN( INTEGER32, i32 )

/// Send an SDO query
canmat_status_t canmat_sdo_query_send( canmat_iface_t *cif, const canmat_sdo_msg_t *req );

/// Receive and SDO query response
canmat_status_t canmat_sdo_query_recv( canmat_iface_t *cif, canmat_sdo_msg_t *resp,
                                       const canmat_sdo_msg_t *req );

int canmat_sdo_print( FILE *f, const canmat_sdo_msg_t *sdo );

/** Send an SDO query response
 *
 * These messages are only sent by slave devices, so this is mainly
 * for testing.
 */
canmat_status_t canmat_sdo_query_resp( canmat_iface_t *cif, const canmat_sdo_msg_t *resp );

/** Return a string describing the error status of SDO */
const char *canmat_sdo_strerror( uint32_t u32 );



#ifdef __cplusplus
}
#endif
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
#endif //SOCANMATIC_H
