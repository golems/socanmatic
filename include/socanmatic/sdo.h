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
#define CANMAT_SDO_REQ_ID(node)  ((canid_t)( ((node)&CANMAT_NODE_MASK) | CANMAT_FUNC_SDO_RX))

/** Make COB-ID for response (slave->master) */
#define CANMAT_SDO_RESP_ID(node) ((canid_t)( ((node)&CANMAT_NODE_MASK) | CANMAT_FUNC_SDO_TX))

typedef enum canmat_command_spec {
    CANMAT_SEG_DL  = 0, ///< segment download
    CANMAT_EX_DL   = 1, ///< expedited download (master->node)
    CANMAT_EX_UL   = 2, ///< expedited upload (node->master)
    CANMAT_SEG_UL  = 3, ///< segment upload
    CANMAT_ABORT    = 4
} canmat_command_spec_t;

typedef enum canmat_command_byte {
    CANMAT_SDO_CMD_DL1 = 0x2f,
    CANMAT_SDO_CMD_DL2 = 0x2b,
    CANMAT_SDO_CMD_DL3 = 0x27,
    CANMAT_SDO_CMD_DL4 = 0x23,

    CANMAT_SDO_CMD_UL1 = 0x4F,
    CANMAT_SDO_CMD_UL2 = 0x4B,
    CANMAT_SDO_CMD_UL3 = 0x47,
    CANMAT_SDO_CMD_UL4 = 0x43
} canmat_command_byte_t;

/** Container struct for SDO requests.
 *
 */
typedef struct canmat_sdo_msg {
    // Message contents
    uint16_t index;    ///< CANopen index
    uint8_t subindex;  ///< CANopen subindex

    // Additional information
    uint8_t node;      ///< CANopen Node ID
    uint8_t length;    ///< CANopen length of data, is either set in n (command) or in the data (depends on s)

    /** Command word
     *
     * It seems that e and s bits are redundant with the comman
     * specifier, and n field is reduandant to the overall CAN message
     * length.
     */
    struct {
        enum canmat_command_spec ccs : 3; ///< Client Command Specifier
        /** Number of bytes in the data part of the message which do not
         *  contain data.  Only valid if e and s are set
         */
        unsigned short n : 2;
        _Bool e;               ///< Is an expedited transfer?
        _Bool s;               ///< Is size specified in n?
    } cmd;

    /* Now the message data */
    union {
        uint8_t data[4];   ///< CANopen data section
        uint32_t value;    ///< CANopen data as unsigned integer
    };
} canmat_sdo_msg_t;

/// Send and SDO request and wait for the response
canmat_status_t canmat_sdo_query( canmat_iface_t *cif, const canmat_sdo_msg_t *req,
                                  canmat_sdo_msg_t *resp );

/// Send an SDO query
canmat_status_t canmat_sdo_query_send( canmat_iface_t *cif, const canmat_sdo_msg_t *req );

/// Receive and SDO query response
canmat_status_t canmat_sdo_query_recv( canmat_iface_t *cif, canmat_sdo_msg_t *resp,
                                       const canmat_sdo_msg_t *req );

static inline uint8_t canmat_sdo_cmd_byte( const canmat_sdo_msg_t *sdo ) {
    return (uint8_t) ( (sdo->cmd.s ? 1 : 0)            |
                       ((sdo->cmd.e ? 1 : 0) << 1)     |
                       ((sdo->cmd.n & 0x3)  << 2)      |
                       ((sdo->cmd.ccs & 0x7)    << 5) );
}

int canmat_sdo_print( FILE *f, const canmat_sdo_msg_t *sdo );

/** Send an SDO query response
 *
 * These messages are only sent by slave devices, so this is mainly
 * for testing.
 */
canmat_status_t canmat_sdo_query_resp( canmat_iface_t *cif, const canmat_sdo_msg_t *resp );


#define CANMAT_SDO_GEN_SET_DATA( CTYPE, BITS, C )                       \
    static inline void                                                  \
    canmat_sdo_set_data_ ## C ## BITS ( canmat_sdo_msg_t *sdo, CTYPE x ) \
    {                                                                   \
        union canmat_byte ## BITS u;                                    \
        u.C = x;                                                        \
        sdo->length = sizeof(x);                                        \
        canmat_byte_stle ## BITS(sdo->data, u.u);                       \
    }
CANMAT_SDO_GEN_SET_DATA( uint8_t, 8, u );
CANMAT_SDO_GEN_SET_DATA( int8_t,  8, i );
CANMAT_SDO_GEN_SET_DATA( uint16_t, 16, u );
CANMAT_SDO_GEN_SET_DATA( int16_t,  16, i );
CANMAT_SDO_GEN_SET_DATA( uint32_t, 32, u );
CANMAT_SDO_GEN_SET_DATA( int32_t,  32, i );
CANMAT_SDO_GEN_SET_DATA( float,    32, f );

#define CANMAT_SDO_GEN_GET_DATA( CTYPE, BITS, C )                       \
    static inline CTYPE                                                 \
    canmat_sdo_get_data_ ## C ## BITS ( const canmat_sdo_msg_t *sdo )   \
    {                                                                   \
        union canmat_byte ## BITS u;                                    \
        u.u = canmat_byte_ldle ## BITS(sdo->data );                     \
        return u.C;                                                     \
    }

CANMAT_SDO_GEN_GET_DATA( uint8_t, 8, u );
CANMAT_SDO_GEN_GET_DATA( int8_t,  8, i );
CANMAT_SDO_GEN_GET_DATA( uint16_t, 16, u );
CANMAT_SDO_GEN_GET_DATA( int16_t,  16, i );
CANMAT_SDO_GEN_GET_DATA( uint32_t, 32, u );
CANMAT_SDO_GEN_GET_DATA( int32_t,  32, i );
CANMAT_SDO_GEN_GET_DATA( float,    32, f );


/// Create a struct can_frame from a canmat_sdo_msg_t
void canmat_sdo2can (struct can_frame *dst, const canmat_sdo_msg_t *src, const int is_response );

/// Create a canmat_sdo_msg_t from a struct can_frame
void canmat_can2sdo( canmat_sdo_msg_t *dst, const struct can_frame *src );


/** Set fields of sdo for an expedited download.
 *
 * @precondition: sdo->data and sdo->length are set
 */
void canmat_sdo_set_ex_dl( canmat_sdo_msg_t *sdo,
                           uint8_t node, uint16_t index, uint8_t subindex );


/*********/
/* 8-bit */
/*********/
canmat_status_t canmat_sdo_dl_u8(
    canmat_iface_t *cif, uint8_t *rccs,
    uint8_t node,
    uint16_t index, uint8_t subindex,
    uint8_t value );

canmat_status_t canmat_sdo_ul_u8(
    canmat_iface_t *cif, uint8_t *rccs,
    uint8_t *value,
    uint8_t node,
    uint16_t index, uint8_t subindex );

canmat_status_t canmat_sdo_dl_i8(
    canmat_iface_t *cif, uint8_t *rccs,
    uint8_t node,
    uint16_t index, uint8_t subindex,
    int8_t value );

canmat_status_t canmat_sdo_ul_i8(
    canmat_iface_t *cif, uint8_t *rccs,
    int8_t *value,
    uint8_t node,
    uint16_t index, uint8_t subindex );

/**********/
/* 16-bit */
/**********/
canmat_status_t canmat_sdo_dl_u16(
    canmat_iface_t *cif, uint8_t *rccs,
    uint8_t node,
    uint16_t index, uint8_t subindex,
    uint16_t value );

canmat_status_t canmat_sdo_ul_u16(
    canmat_iface_t *cif, uint8_t *rccs,
    uint16_t *value,
    uint8_t node,
    uint16_t index, uint8_t subindex );

canmat_status_t canmat_sdo_dl_i16(
    canmat_iface_t *cif, uint8_t *rccs,
    uint8_t node,
    uint16_t index, uint8_t subindex,
    int16_t value );

canmat_status_t canmat_sdo_ul_i16(
    canmat_iface_t *cif, uint8_t *rccs,
    int16_t *value,
    uint8_t node,
    uint16_t index, uint8_t subindex );

/**********/
/* 32-bit */
/**********/
canmat_status_t canmat_sdo_dl_u32(
    canmat_iface_t *cif, uint8_t *rccs,
    uint8_t node,
    uint16_t index, uint8_t subindex,
    uint32_t value );

canmat_status_t canmat_sdo_ul_u32(
    canmat_iface_t *cif, uint8_t *rccs,
    uint32_t *value,
    uint8_t node,
    uint16_t index, uint8_t subindex );

canmat_status_t canmat_sdo_dl_i32(
    canmat_iface_t *cif, uint8_t *rccs,
    uint8_t node,
    uint16_t index, uint8_t subindex,
    int32_t value );

canmat_status_t canmat_sdo_ul_i32(
    canmat_iface_t *cif, uint8_t *rccs,
    int32_t *value,
    uint8_t node,
    uint16_t index, uint8_t subindex );


/** Return a string describing the error status of SDO */
const char *canmat_sdo_strerror( const canmat_sdo_msg_t *sdo );



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
