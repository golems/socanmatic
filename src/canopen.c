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

/**
 * \file ntcanopen.c
 *
 * \brief CANopen implementation using esd's NTCAN API
 *
 * \author Neil Dantam
 * \author Can Erdogan (bug fixes)
 *
 * \bug AMC servo drives require expedited SDO reads to have a full
 * 8-byte data portion.  We must accomodate.
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>


/* Assume linux socketcan for now */
#include <linux/can.h>
#include <linux/can/raw.h>

#include <inttypes.h>

#include "socia.h"
#include "socia_private.h"



/**********/
/** DEFS **/
/**********/


/* Float conversion note: Pointer Type puns are bad, use the union trick */


// Create a struct can_frame from a socia_sdo_msg_t
void socia_build_can( struct can_frame *dst, const socia_sdo_msg_t *src, const int is_response ) {
    // FIXME: better message validation
    assert( src->length <= 4 );

    // Set the message ID and the length
    dst->can_id = (canid_t)(is_response ? SOCIA_SDO_RESP_ID(src->node) : SOCIA_SDO_REQ_ID(src->node));
    dst->can_dlc = (uint8_t)(src->length + 4);   // 4 due to "len" (1), "msg_lost" (1) and "reserved" (2) fields

    // Set the data
    dst->data[0] = src->command;
    dst->data[1] = (uint8_t)(src->index & 0xFF);
    dst->data[2] = (uint8_t)((src->index >> 8) & 0xFF);
    dst->data[3] = src->subindex;
    memcpy( &(dst->data[4]), &(src->data[0]), src->length );

    // Reset the rest of the data that is not filled
    memset( dst->data + 4 + src->length, 0, (size_t)(4 - src->length) );
}

// NOTE: Does this translation stand correct if the dst message does not
// use the entire data?

// Create a socia_sdo_msg_t from a struct can_frame
void socia_build_sdo( socia_sdo_msg_t *dst, const struct can_frame *src, const int is_request ) {
    // FIXME: better message validation
    assert( src->can_dlc <= 8 );
    dst->node = (uint8_t)(src->can_id - (is_request ? SOCIA_SDO_REQ_BASE : SOCIA_SDO_RESP_BASE));
    dst->length = (uint8_t)(src->can_dlc - 4);
    dst->command = (uint8_t)src->data[0];
    dst->index = (uint16_t)(src->data[1] + (src->data[2] << 8));
    dst->subindex = src->data[3];
    memcpy( &(dst->data[0]), &(src->data[4]), dst->length );
    memset( &(dst->data[ dst->length ]), 0, (size_t)(4 - dst->length) );
}

// Send and SDO request and wait for the response
static ssize_t sdo_query( const int fd, const socia_sdo_msg_t *req,
                      socia_sdo_msg_t *resp ) {
    struct can_frame can;
    // Send Request
    {
        socia_build_can( &can, req, 0 );
        ssize_t r = socia_can_send( fd, &can );
        if( ! socia_can_ok( r ) ) {
            return r;
        }
    }

    // Collect Response
    {
        ssize_t r;
        do {
            r = socia_can_recv( fd, &can );
            if( ! socia_can_ok( r ) )
                return r;
        } while ( can.can_id != (canid_t)SOCIA_SDO_RESP_ID(req->node) );
        socia_build_sdo( resp, &can, 0 );
        return r;
    }
}

// build command byte for SDO
void socia_sdo_set_cmd( socia_sdo_msg_t *sdo,
                        socia_command_spec_t command_spec,
                        uint8_t nodata_len,
                        uint8_t is_expedited,
                        uint8_t is_len_in_cmd ) {
    // Sanity checks
    assert( nodata_len <= 3 );
    assert(  (is_expedited && is_len_in_cmd) || ! nodata_len );

    // Create the command by setting the ccs, n, e and s fields
    // NOTE: The reserved field is set to 0 by default.

    sdo->command  = 0xFF & ( (is_len_in_cmd ? 1 : 0)         |  // s
                             ((is_expedited ? 1 : 0) << 1)   |  // e
                             ((nodata_len & 0x3)     << 2)   |  // n
                             ((command_spec & 0x7)   << 5) );   // ccs
}



void socia_sdo_set_ex_dl( socia_sdo_msg_t *sdo,
                          uint8_t node, uint16_t index, uint8_t subindex ) {

    /* Set values */
    sdo->index = index;
    sdo->subindex = subindex;
    sdo->node = node;

    /* Set Command */
    uint8_t nodata_len = (uint8_t)(4 - sdo->length);
    uint8_t is_expedited = 1;
    uint8_t is_len_in_command = 1;
    socia_sdo_set_cmd(sdo, SOCIA_EX_DL, nodata_len, is_expedited, is_len_in_command);
}



#define DEF_SDO_DL( VAL_TYPE, SUFFIX )                                  \
    ssize_t socia_sdo_dl_ ## SUFFIX( int fd, uint8_t *rcmd,             \
                                    uint8_t node,                       \
                                    uint16_t index, uint8_t subindex,   \
                                    VAL_TYPE value ) {                  \
        socia_sdo_msg_t req, resp;                                      \
        socia_sdo_set_data_ ## SUFFIX( &req, value );                   \
        socia_sdo_set_ex_dl( &req, node,                                \
                             index, subindex );                         \
        ssize_t r = sdo_query( fd, &req, &resp );                       \
        if ( socia_can_ok(r) ) *rcmd = resp.command;                    \
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
                       socia_sdo_msg_t *resp,
                       uint8_t node, uint16_t index, uint8_t subindex ) {
    socia_sdo_msg_t req;
    // Build SDO Message
    socia_sdo_set_cmd( &req, SOCIA_EX_UL, 0, 1, 0 );
    req.node = node;
    req.index = index;
    req.subindex = subindex;
    req.length = 0;
    // Query
    ssize_t r = sdo_query( fd, &req, resp );

    // Result
    return r;
}

#define DEF_SDO_UL( VAL_TYPE, IS_SIGNED, SUFFIX )                       \
    ssize_t socia_sdo_ul_ ## SUFFIX( int fd,                            \
                                     uint8_t *rcmd, VAL_TYPE *value,    \
                                     uint8_t node,                      \
                                     uint16_t index, uint8_t subindex ) \
    {                                                                   \
        socia_sdo_msg_t resp;                                           \
        ssize_t r;                                                      \
        r = sdo_ul(fd, &resp, node, index, subindex);                   \
        if( socia_can_ok(r) ) {                                         \
            *rcmd = resp.command;                                       \
            *value = socia_sdo_get_data_ ## SUFFIX( &resp );            \
        }                                                               \
        return r;                                                       \
    }

DEF_SDO_UL(  uint8_t, 0, u8 )
DEF_SDO_UL(   int8_t, 1, i8 )

DEF_SDO_UL( uint16_t, 0, u16 )
DEF_SDO_UL(  int16_t, 1, i16 )

DEF_SDO_UL( uint32_t, 0, u32 )
DEF_SDO_UL(  int32_t, 1, i32 )



/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
