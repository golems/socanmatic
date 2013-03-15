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

#ifndef SOCIA_SDO_H
#define SOCIA_SDO_H

#ifdef __cplusplus
extern "C" {
#endif

/** COB-ID base for request message (master -> slave)
 */
#define SOCIA_SDO_REQ_BASE  ((uint16_t)0x600)
/** COB-ID base for response message (slave -> master)
 */
#define SOCIA_SDO_RESP_BASE ((uint16_t)0x580)

/** COB-ID mask for node bits */
#define SOCIA_SDO_NODE_MASK ((uint16_t)0x7F)

/** Make COB-ID for request (master->slave) */
#define SOCIA_SDO_REQ_ID(node)  ((canid_t)( ((node)&SOCIA_SDO_NODE_MASK) | SOCIA_SDO_REQ_BASE))

/** Make COB-ID for response (slave->master) */
#define SOCIA_SDO_RESP_ID(node) ((canid_t)( ((node)&SOCIA_SDO_NODE_MASK) | SOCIA_SDO_RESP_BASE))

typedef enum socia_command_spec {
    SOCIA_SEG_DL = 0, ///< segment download
    SOCIA_EX_DL = 1,  ///< expedited download (master->node)
    SOCIA_EX_UL = 2,  ///< expedited upload (node->master)
    SOCIA_SEG_UL = 3, ///< segment upload
    SOCIA_ABORT = 4
} socia_command_spec_t;


typedef enum socia_command_byte {
    SOCIA_SDO_CMD_DL1 = 0x2f,
    SOCIA_SDO_CMD_DL2 = 0x2b,
    SOCIA_SDO_CMD_DL3 = 0x27,
    SOCIA_SDO_CMD_DL4 = 0x23
} socia_command_byte_t;

/** Container struct for SDO requests.
 */
typedef struct socia_sdo_msg {
    // Message contents
    uint16_t index;    ///< CANopen index
    uint8_t subindex;  ///< CANopen subindex

    // Additional information
    uint8_t node;      ///< CANopen Node ID
    uint8_t length;    ///< CANopen length of data, is either set in n (command) or in the data (depends on s)

    /** Command word */
    struct {
        enum socia_command_spec ccs : 3; ///< Client Command Specifier  (ccs)
        /** Number of bytes in the data part of the message which do not
         *  contain data.  Only valid if e and s are set (n)
         */
        unsigned short n : 2;
        _Bool e;      ///< Is an expedited transfer? (e)
        _Bool s; ///< Is size specified in n? (s)
    } cmd;

    /* Now the message data */
    union {
        uint8_t data[4];   ///< CANopen data section
        uint32_t value;    ///< CANopen data as unsigned integer
    };
} socia_sdo_msg_t;

/// Send and SDO request and wait for the response
ssize_t socia_sdo_query( int fd, const socia_sdo_msg_t *req,
                         socia_sdo_msg_t *resp );

/// Send an SDO query
ssize_t socia_sdo_query_send( int fd, const socia_sdo_msg_t *req );

/// Receive and SDO query response
ssize_t socia_sdo_query_recv( int fd, socia_sdo_msg_t *resp,
                              const socia_sdo_msg_t *req );


/** Send an SDO query response
 *
 * These messages are only sent by slave devices, so this is mainly
 * for testing.
 */
ssize_t socia_sdo_query_resp( int fd, const socia_sdo_msg_t *resp );


#define SOCIA_SDO_GEN_SET_DATA( CTYPE, BITS, C )                        \
    static inline void                                                  \
    socia_sdo_set_data_ ## C ## BITS ( socia_sdo_msg_t *sdo, CTYPE x )  \
    {                                                                   \
        union socia_byte ## BITS u;                                     \
        u.C = x;                                                        \
        sdo->length = sizeof(x);                                        \
        socia_byte_stle ## BITS(sdo->data, u.u);                        \
    }
SOCIA_SDO_GEN_SET_DATA( uint8_t, 8, u );
SOCIA_SDO_GEN_SET_DATA( int8_t,  8, i );
SOCIA_SDO_GEN_SET_DATA( uint16_t, 16, u );
SOCIA_SDO_GEN_SET_DATA( int16_t,  16, i );
SOCIA_SDO_GEN_SET_DATA( uint32_t, 32, u );
SOCIA_SDO_GEN_SET_DATA( int32_t,  32, i );
SOCIA_SDO_GEN_SET_DATA( float,    32, f );

#define SOCIA_SDO_GEN_GET_DATA( CTYPE, BITS, C )                        \
    static inline CTYPE                                                 \
    socia_sdo_get_data_ ## C ## BITS ( socia_sdo_msg_t *sdo )           \
    {                                                                   \
        union socia_byte ## BITS u;                                     \
        u.u = socia_byte_ldle ## BITS(sdo->data );                      \
        return u.C;                                                     \
    }
SOCIA_SDO_GEN_GET_DATA( uint8_t, 8, u );
SOCIA_SDO_GEN_GET_DATA( int8_t,  8, i );
SOCIA_SDO_GEN_GET_DATA( uint16_t, 16, u );
SOCIA_SDO_GEN_GET_DATA( int16_t,  16, i );
SOCIA_SDO_GEN_GET_DATA( uint32_t, 32, u );
SOCIA_SDO_GEN_GET_DATA( int32_t,  32, i );
SOCIA_SDO_GEN_GET_DATA( float,    32, f );


/// Create a struct can_frame from a socia_sdo_msg_t
void socia_sdo2can (struct can_frame *dst, const socia_sdo_msg_t *src, const int is_response );

/// Create a socia_sdo_msg_t from a struct can_frame
void socia_can2sdo( socia_sdo_msg_t *dst, const struct can_frame *src );


/** Set fields of sdo for an expedited download.
 *
 * @precondition: sdo->data and sdo->length are set
 */
void socia_sdo_set_ex_dl( socia_sdo_msg_t *sdo,
                          uint8_t node, uint16_t index, uint8_t subindex );


/*********/
/* 8-bit */
/*********/
ssize_t socia_sdo_dl_u8( int fd, uint8_t *rccs,
                         uint8_t node,
                         uint16_t index, uint8_t subindex,
                         uint8_t value );

ssize_t socia_sdo_ul_u8( int fd, uint8_t *rccs,
                         uint8_t *value,
                         uint8_t node,
                         uint16_t index, uint8_t subindex );

ssize_t socia_sdo_dl_i8( int fd, uint8_t *rccs,
                         uint8_t node,
                         uint16_t index, uint8_t subindex,
                         int8_t value );

ssize_t socia_sdo_ul_i8( int fd, uint8_t *rccs,
                         int8_t *value,
                         uint8_t node,
                         uint16_t index, uint8_t subindex );

/**********/
/* 16-bit */
/**********/
ssize_t socia_sdo_dl_u16( int fd, uint8_t *rccs,
                          uint8_t node,
                          uint16_t index, uint8_t subindex,
                          uint16_t value );

ssize_t socia_sdo_ul_u16( int fd, uint8_t *rccs,
                          uint16_t *value,
                          uint8_t node,
                          uint16_t index, uint8_t subindex );

ssize_t socia_sdo_dl_i16( int fd, uint8_t *rccs,
                          uint8_t node,
                          uint16_t index, uint8_t subindex,
                          int16_t value );

ssize_t socia_sdo_ul_i16( int fd, uint8_t *rccs,
                          int16_t *value,
                          uint8_t node,
                          uint16_t index, uint8_t subindex );

/**********/
/* 32-bit */
/**********/
ssize_t socia_sdo_dl_u32( int fd, uint8_t *rccs,
                          uint8_t node,
                          uint16_t index, uint8_t subindex,
                          uint32_t value );

ssize_t socia_sdo_ul_u32( int fd, uint8_t *rccs,
                          uint32_t *value,
                          uint8_t node,
                          uint16_t index, uint8_t subindex );

ssize_t socia_sdo_dl_i32( int fd, uint8_t *rccs,
                          uint8_t node,
                          uint16_t index, uint8_t subindex,
                          int32_t value );

ssize_t socia_sdo_ul_i32( int fd, uint8_t *rccs,
                          int32_t *value,
                          uint8_t node,
                          uint16_t index, uint8_t subindex );




#ifdef __cplusplus
}
#endif
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
#endif //SOCIA_H
