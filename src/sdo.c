/* -*- mode: C; c-basic-offset: 4 -*- */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
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


#include <assert.h>
#include <string.h>
#include "socanmatic.h"
#include "socanmatic_private.h"

/**********/
/** DEFS **/
/**********/

// Create a struct can_frame from a canmat_sdo_msg_t
void canmat_sdo2can (struct can_frame *dst, const canmat_sdo_msg_t *src, const int is_response ) {
    // FIXME: better message validation
    assert( src->length <= 4 );

    // Set the message ID and the length
    dst->can_id = (canid_t)(is_response ? CANMAT_SDO_RESP_ID(src->node) : CANMAT_SDO_REQ_ID(src->node));
    dst->can_dlc = (uint8_t)(src->length + 4);   // 4 due to "len" (1), "msg_lost" (1) and "reserved" (2) fields

    // set command byte
    dst->data[0] = canmat_sdo_cmd_byte( src );;

    // set indices
    dst->data[1] = (uint8_t)(src->index & 0xFF);
    dst->data[2] = (uint8_t)((src->index >> 8) & 0xFF);
    dst->data[3] = src->subindex;

    // set sdo data
    memcpy( &(dst->data[4]), &(src->data[0]), src->length );

    // Reset the rest of the data that is not filled
    memset( dst->data + 4 + src->length, 0, (size_t)(4 - src->length) );
}

// NOTE: Does this translation stand correct if the dst message does not
// use the entire data?

// Create a canmat_sdo_msg_t from a struct can_frame
void canmat_can2sdo( canmat_sdo_msg_t *dst, const struct can_frame *src ) {
    // FIXME: better message validation, check that message is actually an SDO

    assert( src->can_dlc <= 8 );

    uint8_t cmd = src->data[0];
    dst->cmd.s = cmd & 0x1;
    dst->cmd.e = (cmd >> 1) & 0x1;
    dst->cmd.n = (uint8_t) ((cmd >> 2) & 0x3);
    dst->cmd.ccs = (enum canmat_command_spec) ((cmd >> 5) & 0x7);

    dst->node = (uint8_t)(src->can_id & CANMAT_SDO_NODE_MASK);

    dst->length = (uint8_t)(src->can_dlc - 4);



    dst->index = (uint16_t)(src->data[1] + (src->data[2] << 8));
    dst->subindex = src->data[3];
    memcpy( &(dst->data[0]), &(src->data[4]), dst->length );
    memset( &(dst->data[ dst->length ]), 0, (size_t)(4 - dst->length) );
}


/// Send and SDO request and wait for the response
ssize_t canmat_sdo_query( int fd, const canmat_sdo_msg_t *req,
                          canmat_sdo_msg_t *resp ) {
    ssize_t r = canmat_sdo_query_send( fd, req );
    if( r >= 0 ) {
        return canmat_sdo_query_recv( fd, resp, req );
    } else {
        return r;
    }
}

/// Send an SDO query
ssize_t canmat_sdo_query_send( int fd, const canmat_sdo_msg_t *req ) {
    struct can_frame can;
    canmat_sdo2can( &can, req, 0 );
    return  canmat_can_send( fd, &can );
}

/// Receive and SDO query response
ssize_t canmat_sdo_query_recv( int fd, canmat_sdo_msg_t *resp,
                               const canmat_sdo_msg_t *req ) {

    ssize_t r;
    struct can_frame can;
    do {
        r = canmat_can_recv( fd, &can );
        if( ! canmat_can_ok( r ) )
            return r;
    } while ( can.can_id != (canid_t)CANMAT_SDO_RESP_ID(req->node) );

    canmat_can2sdo( resp, &can );
    return r;

}


ssize_t canmat_sdo_query_resp( int fd, const canmat_sdo_msg_t *resp ) {
    struct can_frame can;
    canmat_sdo2can( &can, resp, 1 );
    return  canmat_can_send( fd, &can );
}

void canmat_sdo_set_ex_dl( canmat_sdo_msg_t *sdo,
                           uint8_t node, uint16_t index, uint8_t subindex ) {

    /* Set values */
    sdo->index = index;
    sdo->subindex = subindex;
    sdo->node = node;

    /* Set Command */
    sdo->cmd.n = (unsigned char) ((4 - sdo->length) & 0x3);
    sdo->cmd.e = 1;
    sdo->cmd.s = 1;
    sdo->cmd.ccs = CANMAT_EX_DL;
}



#define DEF_SDO_DL( VAL_TYPE, SUFFIX )                                  \
    ssize_t canmat_sdo_dl_ ## SUFFIX( int fd, uint8_t *rccs,            \
                                      uint8_t node,                     \
                                      uint16_t index, uint8_t subindex, \
                                      VAL_TYPE value ) {                \
        canmat_sdo_msg_t req, resp;                                     \
        canmat_sdo_set_data_ ## SUFFIX( &req, value );                  \
        canmat_sdo_set_ex_dl( &req, node,                               \
                              index, subindex );                        \
        ssize_t r = canmat_sdo_query( fd, &req, &resp );                \
        if ( canmat_can_ok(r) ) *rccs = resp.cmd.ccs;                   \
        return r;                                                       \
    }                                                                   \

DEF_SDO_DL(  uint8_t, u8 )
DEF_SDO_DL(   int8_t, i8 )

DEF_SDO_DL( uint16_t, u16 )
DEF_SDO_DL(  int16_t, i16 )

DEF_SDO_DL( uint32_t, u32 )
DEF_SDO_DL(  int32_t, i32 )



/* precondition: sdo contains message data and length
 *
 * Expedited only
 */
static ssize_t sdo_ul( int fd,
                       canmat_sdo_msg_t *resp,
                       uint8_t node, uint16_t index, uint8_t subindex ) {
    canmat_sdo_msg_t req;
    // Build SDO Message
    req.cmd.ccs = CANMAT_EX_UL;
    req.cmd.e = 1;
    req.cmd.n = 0;
    req.cmd.s = 0;

    req.node = node;
    req.index = index;
    req.subindex = subindex;
    req.length = 0;
    // Query
    ssize_t r = canmat_sdo_query( fd, &req, resp );

    // Result
    return r;
}

#define DEF_SDO_UL( VAL_TYPE, IS_SIGNED, SUFFIX )                       \
    ssize_t canmat_sdo_ul_ ## SUFFIX( int fd,                           \
                                      uint8_t *rccs, VAL_TYPE *value,   \
                                      uint8_t node,                     \
                                      uint16_t index, uint8_t subindex ) \
    {                                                                   \
        canmat_sdo_msg_t resp;                                          \
        ssize_t r;                                                      \
        r = sdo_ul(fd, &resp, node, index, subindex);                   \
        if( canmat_can_ok(r) ) {                                        \
            *rccs = resp.cmd.ccs;                                       \
            *value = canmat_sdo_get_data_ ## SUFFIX( &resp );           \
        }                                                               \
        return r;                                                       \
    }

DEF_SDO_UL(  uint8_t, 0, u8 )
DEF_SDO_UL(   int8_t, 1, i8 )

DEF_SDO_UL( uint16_t, 0, u16 )
DEF_SDO_UL(  int16_t, 1, i16 )

DEF_SDO_UL( uint32_t, 0, u32 )
DEF_SDO_UL(  int32_t, 1, i32 )


int canmat_sdo_print( FILE *f, const canmat_sdo_msg_t *sdo ) {
    fprintf(f, "%02x.%02x(%d:%d:%d:%d)[%04x.%02x]",
            sdo->node,
            canmat_sdo_cmd_byte(sdo), sdo->cmd.e, sdo->cmd.s, sdo->cmd.e, sdo->cmd.ccs,
            sdo->index, sdo->subindex);
    int i;
    for( i = 0; i < sdo->length; i++ ) {
        fprintf(f,"%c%02x",
                i ? ':' : ' ',
                sdo->data[i] );

    }
    fputc('\n', f);
    return 0;
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
