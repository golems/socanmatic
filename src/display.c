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

static void sdo_bytes( const canmat_sdo_msg_t *sdo  ) {
    for( size_t i = 0; i < sdo->length; i++) {
        printf("%c%02x", i ? ':' : ' ', sdo->data[i] );
    }
}


static int sdo_check_length( const canmat_sdo_msg_t *sdo, size_t len  ) {
    if( len < sdo->length ) {
        printf(" overflow");
        sdo_bytes(sdo);
        return -1;
    } else if ( len > sdo->length ) {
        printf(" underflow");
        sdo_bytes(sdo);
        return -1;
    } else {
        return 0;
    }
}

static void display_sdo( const canmat_dict_t *dict, const struct can_frame *can ) {
    uint16_t func = canmat_frame_func(can);
    uint8_t node = canmat_frame_node(can);
    assert( CANMAT_FUNC_SDO_RX == func ||
            CANMAT_FUNC_SDO_TX == func );

    canmat_sdo_msg_t sdo;
    canmat_can2sdo( &sdo, can );

    canmat_obj_t *obj = canmat_dict_search_index( dict, sdo.index, sdo.subindex );
    const char *param = obj ? obj->parameter_name : "unknown";

    const char *cs = "unknown";
    switch( sdo.cmd.ccs ) {
    case CANMAT_SEG_DL: cs = "SEG_DL"; break;
    case CANMAT_EX_DL:  cs = "EX_DL";  break;
    case CANMAT_EX_UL:  cs = "EX_UL";  break;
    case CANMAT_SEG_UL: cs = "SEG_UL"; break;
    case CANMAT_ABORT:  cs = "ABORT";  break;
    }

    printf("sdo %s, node %03x %s `%s' (%04x.%02x)",
           CANMAT_FUNC_SDO_RX == func ? "rx" : "tx",
           node, cs,
           param, sdo.index, sdo.subindex
        );

    // print data
    if( (CANMAT_FUNC_SDO_RX == func &&
         CANMAT_EX_DL == sdo.cmd.ccs) ||
        (CANMAT_FUNC_SDO_TX == func &&
         CANMAT_EX_UL == sdo.cmd.ccs) ) {
        if( obj ) {
            switch( obj->data_type ) {
            case CANMAT_DATA_TYPE_INTEGER8:
                if( sdo_check_length(&sdo, 1) ) break;
                printf( " %"PRId8"\n",
                        canmat_sdo_get_data_i8(&sdo) );
                break;
            case CANMAT_DATA_TYPE_INTEGER16:
                if( sdo_check_length(&sdo, 2) ) break;
                printf( " %"PRId16"\n",
                        canmat_sdo_get_data_i16(&sdo) );
                break;
            case CANMAT_DATA_TYPE_INTEGER32:
                if( sdo_check_length(&sdo, 4) ) break;
                printf( " %"PRId32"\n",
                        canmat_sdo_get_data_i32(&sdo) );
                break;
            case CANMAT_DATA_TYPE_UNSIGNED8:
                if( sdo_check_length(&sdo, 1) ) break;
                printf( " 0x%"PRIx8"\n",
                        canmat_sdo_get_data_u8(&sdo) );
                break;
            case CANMAT_DATA_TYPE_UNSIGNED16:
                if( sdo_check_length(&sdo, 2) ) break;
                printf( " 0x%"PRIx16"\n",
                        canmat_sdo_get_data_u16(&sdo) );
                break;
            case CANMAT_DATA_TYPE_UNSIGNED32:
                if( sdo_check_length(&sdo, 4) ) break;
                printf( " 0x%"PRIx32"\n",
                        canmat_sdo_get_data_u32(&sdo) );
                break;
            default:
                printf(" unhandled type");
                sdo_bytes(&sdo);
            }
        } else {
            sdo_bytes(&sdo);
        }
    } else if ( CANMAT_SEG_DL == func ||
                CANMAT_SEG_UL == func ) {
        printf(" segmented");
    } else if (sdo.length > 0) {
        printf(" unexpected data");
        sdo_bytes(&sdo);
    }

    fputc('\n', stdout);
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
