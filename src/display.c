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

#include <errno.h>
#include <string.h>
#include <assert.h>
#include "socanmatic.h"
#include "socanmatic_private.h"


static void display_raw( const struct can_frame *can ) ;
static void display_sdo( const canmat_dict_t *dict, const struct can_frame *can ) ;
static void display_nmt( const struct can_frame *can ) ;
static void display_nmt_err( const struct can_frame *can ) ;
static void display_malformed( const struct can_frame *can ) ;




void canmat_display( const canmat_dict_t *dict, const struct can_frame *can ) {

    switch( canmat_frame_func(can) ) {
    case CANMAT_FUNC_NMT:
        display_nmt( can );
        break;
    case CANMAT_FUNC_SDO_TX:
    case CANMAT_FUNC_SDO_RX:
        display_sdo( dict, can );
        break;
    case CANMAT_FUNC_NMT_ERR:
        display_nmt_err( can );
        break;
    case CANMAT_FUNC_TIME:
    case CANMAT_FUNC_SYNC: /* or EMCY */
    case CANMAT_FUNC_PDO1_TX:
    case CANMAT_FUNC_PDO1_RX:
    case CANMAT_FUNC_PDO2_TX:
    case CANMAT_FUNC_PDO2_RX:
    case CANMAT_FUNC_PDO3_TX:
    case CANMAT_FUNC_PDO3_RX:
    case CANMAT_FUNC_PDO4_TX:
    case CANMAT_FUNC_PDO4_RX:
    default:
        display_raw(can);
    }
}


static void display_raw( const struct can_frame *can ) {
    fputs("raw, ", stdout);
    canmat_dump_frame( stdout, can );
}


static void display_malformed( const struct can_frame *can ) {
    fputs("malformed, ", stdout);
    canmat_dump_frame( stdout, can );
}


static void display_sdo( const canmat_dict_t *dict, const struct can_frame *can ) {
    uint16_t func = canmat_frame_func(can);
    assert( CANMAT_FUNC_SDO_RX == func ||
            CANMAT_FUNC_SDO_TX == func );
}
static void display_nmt( const struct can_frame *can )  {
    assert( CANMAT_FUNC_NMT == can->can_id );

}

static void display_nmt_err( const struct can_frame *can ) {

    uint16_t func = canmat_frame_func(can);
    uint8_t node = canmat_frame_node(can);

    assert( CANMAT_FUNC_NMT_ERR == func );

    if( can->can_dlc < 1 ) {
        display_malformed(can);
        return;
    }

    const char *status = "unknown";


    switch(can->data[0]) {
    case CANMAT_NMT_ERR_BOOT:     status = "boot";
        break;
    case CANMAT_NMT_ERR_STOPPED:  status = "stopped";
        break;
    case CANMAT_NMT_ERR_OP:       status = "operational";
        break;
    case CANMAT_NMT_ERR_PRE_OP:   status = "pre-operational";
        break;
    }

    printf( "nmt-err (0x%03x), node 0x%02x %s (0x%02x)\n",
            func, node, status, can->data[0] );
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
