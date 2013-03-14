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


void check_sdo_dl( int64_t i, uint8_t length, _Bool is_signed ) {
    assert( i >= 0 || is_signed );

    socia_sdo_msg_t sdo;
    // set values
    socia_sdo_set_ex_dl( &sdo, 0x10, 0x20, 0x30,
                         i, length );
    // check values
    switch(length) {
    case 4: assert( sdo.data[3] == ((i >> 24) & 0xFF) );
    case 3: assert( sdo.data[2] == ((i >> 16) & 0xFF) );
    case 2: assert( sdo.data[1] == ((i >>  8) & 0xFF) );
    case 1: assert( sdo.data[0] == ((i >>  0) & 0xFF) );
        break;
    default: assert(0);
    }
    assert( 0x10 == sdo.node );
    assert( 0x20 == sdo.index );
    assert( 0x30 == sdo.subindex );

    // check command
    switch( length ) {
    case 1: assert( SOCIA_SDO_CMD_DL1 == sdo.command );
        break;
    case 2: assert( SOCIA_SDO_CMD_DL2 == sdo.command );
        break;
    case 3: assert( SOCIA_SDO_CMD_DL3 == sdo.command );
        break;
    case 4: assert( SOCIA_SDO_CMD_DL4 == sdo.command );
        break;
    default: assert(0);
    }

    // extract data
    int64_t j = socia_sdo_get_data( &sdo, is_signed );
    assert( j == i );
}


int main( int argc, char **argv ) {
    (void) argc; (void) argv;

    check_sdo_dl(  1, 1, 0 );
    check_sdo_dl(  1, 1, 1 );

    check_sdo_dl( -1, 1, 1 );
    check_sdo_dl( -1, 4, 1 );

    check_sdo_dl( 0x1234, 2, 0 );
    check_sdo_dl( 0x1234, 2, 1 );

    return 0;
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
