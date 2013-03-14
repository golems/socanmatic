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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#include <inttypes.h>

/* Assume linux socketcan for now */
#include <linux/can.h>
#include <linux/can/raw.h>

#include "socia.h"
#include "socia_private.h"


void check_sdo_can( socia_sdo_msg_t *sdo ) {
    struct can_frame can;
    socia_sdo2can( &can, sdo, 0);
    assert( can.can_id == SOCIA_SDO_REQ_ID( sdo->node ) );
    assert( (can.can_id & (uint16_t)~SOCIA_SDO_NODE_MASK) == SOCIA_SDO_REQ_BASE );
    assert( (can.can_id & SOCIA_SDO_NODE_MASK) == sdo->node );
    assert( can.data[0] == sdo->command );
    assert( can.data[1] == (sdo->index & 0xff) );
    assert( can.data[2] == sdo->index >> 8 );
    assert( can.data[3] == sdo->subindex );
    size_t i;
    for( i = 0; i < sdo->length; i ++ ) {
        assert( can.data[4+i] == sdo->data[i] );
    }
    socia_sdo_msg_t sdo2;

    socia_can2sdo( &sdo2, &can );
    assert( sdo2.node == sdo->node );
    assert( sdo2.command == sdo->command );
    assert( sdo2.index == sdo->index );
    assert( sdo2.subindex == sdo->subindex );
    for( i = 0; i < sdo->length; i ++ ) {
        assert( sdo2.data[i] == sdo->data[i] );
    }
    //assert( 0 == memcmp(sdo, &sdo2, sizeof(sdo2)) );
}

void check_sdo_dl( ) {

    socia_sdo_msg_t sdo;

    // 8 bit
    socia_sdo_set_data_u8( &sdo, 0x11 );

    // set values
    socia_sdo_set_ex_dl( &sdo, 0x10, 0x20, 0x30 );

    // check values
    assert( 0x10 == sdo.node );
    assert( 0x20 == sdo.index );
    assert( 0x30 == sdo.subindex );
    assert( SOCIA_SDO_CMD_DL1 == sdo.command );

    check_sdo_can( &sdo );

    // 16 bit
    socia_sdo_set_data_u16( &sdo, 0x2211 );

    // set values
    socia_sdo_set_ex_dl( &sdo, 0x10, 0x20, 0x30 );

    // check values
    assert( 0x10 == sdo.node );
    assert( 0x20 == sdo.index );
    assert( 0x30 == sdo.subindex );
    assert( SOCIA_SDO_CMD_DL2 == sdo.command );

    check_sdo_can( &sdo );

    // 32 bit
    socia_sdo_set_data_u32( &sdo, 0x44332211 );

    // set values
    socia_sdo_set_ex_dl( &sdo, 0x10, 0x20, 0x30 );

    // check values
    assert( 0x10 == sdo.node );
    assert( 0x20 == sdo.index );
    assert( 0x30 == sdo.subindex );
    assert( SOCIA_SDO_CMD_DL4 == sdo.command );

    check_sdo_can( &sdo );
}

static void byteorder(void) {
    uint8_t a[2];
    uint8_t b[4];

    socia_byte_stle16( a, 0x2211 );
    socia_byte_stle32( b, 0x44332211 );

    assert( 0x11 == a[0] );
    assert( 0x22 == a[1] );

    assert( 0x11 == b[0] );
    assert( 0x22 == b[1] );
    assert( 0x33 == b[2] );
    assert( 0x44 == b[3] );


    assert( 0x2211 == socia_byte_ldle16(a) );
    assert( 0x44332211 == socia_byte_ldle32(b) );

    assert( 0x2211 == socia_byte_ldle16(b) );

}

static void sdo_data(void) {
    socia_sdo_msg_t sdo;
    socia_sdo_set_data_u16( &sdo, 0x2211 );
    assert( 2 == sdo.length );
    assert( 0x11 == sdo.data[0] );
    assert( 0x22 == sdo.data[1] );

    socia_sdo_set_data_u32( &sdo, 0x00332211 );
    assert( 4 == sdo.length );
    assert( 0x11 == sdo.data[0] );
    assert( 0x22 == sdo.data[1] );
    assert( 0x33 == sdo.data[2] );
    assert( 0x00 == sdo.data[3] );


    socia_sdo_set_data_i16( &sdo, -42 );
    assert( 2 == sdo.length );
    assert( -42 == socia_sdo_get_data_i16( &sdo ) );
}

int main( int argc, char **argv ) {
    (void) argc; (void) argv;

    byteorder();
    sdo_data();

    check_sdo_dl( );

    return 0;
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
