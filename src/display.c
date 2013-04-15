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
static void display_emcy( const struct can_frame *can ) ;
static void display_nmt_err( const struct can_frame *can ) ;
static void display_malformed( const struct can_frame *can ) ;


void canmat_display( const canmat_dict_t *dict, const struct can_frame *can ) {

    switch( canmat_frame_func(can) ) {
    case CANMAT_FUNC_CODE_NMT:
        display_nmt( can );
        break;
    case CANMAT_FUNC_CODE_SDO_TX:
    case CANMAT_FUNC_CODE_SDO_RX:
        display_sdo( dict, can );
        break;
    case CANMAT_FUNC_CODE_NMT_ERR:
        display_nmt_err( can );
        break;
    case CANMAT_FUNC_CODE_SYNC_EMCY:
        /* TODO: display sync */
        display_emcy(can);
        break;
    case CANMAT_FUNC_CODE_TIME:
    case CANMAT_FUNC_CODE_PDO1_TX:
    case CANMAT_FUNC_CODE_PDO1_RX:
    case CANMAT_FUNC_CODE_PDO2_TX:
    case CANMAT_FUNC_CODE_PDO2_RX:
    case CANMAT_FUNC_CODE_PDO3_TX:
    case CANMAT_FUNC_CODE_PDO3_RX:
    case CANMAT_FUNC_CODE_PDO4_TX:
    case CANMAT_FUNC_CODE_PDO4_RX:
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
        printf("%c%02x", i ? ':' : ' ', sdo->data.byte[i] );
    }
}


static int sdo_check_length( const canmat_sdo_msg_t *sdo, size_t len  ) {
    if( len < sdo->length ) {
        printf(" overflow");
        sdo_bytes(sdo);
        return -1;
    } else
        if ( len > sdo->length ) {
        printf(" underflow");
        sdo_bytes(sdo);
        return -1;
    } else {
        return 0;
    }
}

static const char *ccs2str( enum canmat_ccs cs ) {
    switch(cs) {
    case CANMAT_CCS_SEG_DL: return "seg_dl";
    case CANMAT_CCS_EX_DL:  return "ex_dl";
    case CANMAT_CCS_EX_UL:  return "ex_ul";
    case CANMAT_CCS_SEG_UL: return "seg_ul";
    case CANMAT_CCS_ABORT:  return "abort";
    case CANMAT_CCS_BLK_UL: return "blk_ul";
    case CANMAT_CCS_BLK_DL: return "blk_dl";
    }
    return "?";
}

static const char *scs2str( enum canmat_scs cs ) {
    switch(cs) {
    case CANMAT_SCS_SEG_DL: return "seg_dl";
    case CANMAT_SCS_EX_DL:  return "ex_dl";
    case CANMAT_SCS_EX_UL:  return "ex_ul";
    case CANMAT_SCS_SEG_UL: return "seg_ul";
    case CANMAT_SCS_ABORT:  return "abort";
    case CANMAT_SCS_BLK_UL: return "blk_ul";
    case CANMAT_SCS_BLK_DL: return "blk_dl";
    }
    return "?";
}

static void display_sdo( const canmat_dict_t *dict, const struct can_frame *can ) {
    uint16_t func = canmat_frame_func(can);
    uint8_t node = canmat_frame_node(can);
    assert( CANMAT_FUNC_CODE_SDO_RX == func ||
            CANMAT_FUNC_CODE_SDO_TX == func );
    _Bool is_tx = CANMAT_FUNC_CODE_SDO_TX == func;

    uint16_t index = canmat_can2sdo_index( can );
    uint8_t subindex = canmat_can2sdo_subindex( can );

    canmat_obj_t *obj = canmat_dict_search_index( dict, index, subindex );

    canmat_sdo_msg_t sdo;
    canmat_status_t r = canmat_can2sdo( &sdo, can, obj ? obj->data_type : CANMAT_DATA_TYPE_UNSIGNED32 );
    if( ! (CANMAT_OK == r || CANMAT_ERR_ABORT == r) ) {
        printf("bad sdo, %s: ", canmat_strerror(r));
        display_raw(can);
        return;
    }



    const char *param = obj ? obj->parameter_name : "unknown";

    const char *cs = "unknown";
    if( is_tx ) {
        cs = scs2str((enum canmat_scs)sdo.cmd_spec);
    } else {
        cs = ccs2str((enum canmat_ccs)sdo.cmd_spec);
    }

    printf("sdo %s, node %03x %s '%s' (%04x.%02x)",
           is_tx ? "tx" : "rx",
           node, cs,
           param, sdo.index, sdo.subindex
        );

    // print data
    if ( CANMAT_CS_ABORT == sdo.cmd_spec ) {
        if( 4 == sdo.length ) {
            printf( " '%s' (0x%04"PRIx32")",
                    canmat_sdo_strerror(sdo.data.u32),
                    sdo.data.u32 );
        } else {
            printf(" bad length (%d)", sdo.length);
            sdo_bytes(&sdo);
        }
    } else if( (CANMAT_FUNC_CODE_SDO_RX == func && CANMAT_CCS_EX_DL == sdo.cmd_spec) ||
               (CANMAT_FUNC_CODE_SDO_TX == func && CANMAT_SCS_EX_UL == sdo.cmd_spec) ) {
        if( obj ) {
            switch( obj->data_type ) {
            case CANMAT_DATA_TYPE_INTEGER8:
                if( sdo_check_length(&sdo, 1) ) break;
                printf( " %"PRId8, sdo.data.i8 );
                break;
            case CANMAT_DATA_TYPE_INTEGER16:
                if( sdo_check_length(&sdo, 2) ) break;
                printf( " %"PRId16, sdo.data.i16 );
                break;
            case CANMAT_DATA_TYPE_INTEGER32:
                if( sdo_check_length(&sdo, 4) ) break;
                printf( " %"PRId32, sdo.data.i32 );
                break;
            case CANMAT_DATA_TYPE_UNSIGNED8:
                if( sdo_check_length(&sdo, 1) ) break;
                printf( " 0x%"PRIx8, sdo.data.u8 );
                break;
            case CANMAT_DATA_TYPE_UNSIGNED16:
                if( sdo_check_length(&sdo, 2) ) break;
                printf( " 0x%"PRIx16, sdo.data.u16 );
                break;
            case CANMAT_DATA_TYPE_UNSIGNED32:
                if( sdo_check_length(&sdo, 4) ) break;
                printf( " 0x%"PRIx32, sdo.data.u32 );
                break;
            case CANMAT_DATA_TYPE_VOID:
                printf(" (no data)");
            default:
                printf(" unhandled type");
                sdo_bytes(&sdo);
            }
        } else {
            sdo_bytes(&sdo);
        }
    } else if( (CANMAT_FUNC_CODE_SDO_RX == func && CANMAT_CCS_EX_UL == sdo.cmd_spec) ||
               (CANMAT_FUNC_CODE_SDO_TX == func && CANMAT_SCS_EX_DL == sdo.cmd_spec) ) {
        // ignore this data
        ;
    } else {
        printf(" unhandled ");
        //FIXME
        sdo_bytes(&sdo);
    }

    fputc('\n', stdout);
}

static void display_nmt( const struct can_frame *can )  {
    assert( CANMAT_FUNC_CODE_NMT == can->can_id );

    if( can->can_dlc >= 2 ) {
        const char *action = "unknown";
        switch( can->data[0] ) {
        case CANMAT_NMT_START_REMOTE: action = "start";      break;
        case CANMAT_NMT_STOP_REMOTE:  action = "stop";       break;
        case CANMAT_NMT_PRE_OP:       action = "pre-op";     break;
        case CANMAT_NMT_RESET_NODE:   action = "reset-node"; break;
        case CANMAT_NMT_RESET_COM:    action = "reset-com";  break;
        }
        printf("nmt, node 0x%"PRIx8" '%s' (0x%"PRIx8")\n",
               can->data[1], action, can->data[0]);
    } else {
        display_malformed(can);
    }


}

static void display_nmt_err( const struct can_frame *can ) {
    uint16_t func = canmat_frame_func(can);
    uint8_t node = canmat_frame_node(can);

    assert( CANMAT_FUNC_CODE_NMT_ERR == func );

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

    printf( "nmt-err (0x%03x), node 0x%02x '%s' (0x%02x)\n",
            func, node, status, can->data[0] );
}


static void display_emcy( const struct can_frame *can ) {
    uint16_t func = canmat_frame_func(can);
    uint8_t node = canmat_frame_node(can);

    assert( CANMAT_FUNC_CODE_SYNC_EMCY == func );

    if( can->can_dlc < 3 ) {
        display_malformed(can);
        return;
    }

    uint16_t eec = canmat_frame_emcy_get_eec( can );
    uint8_t er = canmat_frame_emcy_get_er( can );

    const char *status = "unknown";
    switch(eec & 0xFF00) {

    case CANMAT_EMCY_CODE_CLASS_NO_ERROR:     status = "no_error";           break;
    case CANMAT_EMCY_CODE_CLASS_GENERIC:      status = "generic";            break;
    case CANMAT_EMCY_CODE_CURRENT:            status = "current";            break;
    case CANMAT_EMCY_CODE_CURRENT_INPUT:      status = "current_input";      break;
    case CANMAT_EMCY_CODE_CURRENT_INSIDE:     status = "current_inside";     break;
    case CANMAT_EMCY_CODE_CURRENT_OUTPUT:     status = "current_output";     break;
    case CANMAT_EMCY_CODE_VOLTAGE:            status = "voltage";            break;
    case CANMAT_EMCY_CODE_VOLTAGE_MAINS:      status = "voltage_mains";      break;
    case CANMAT_EMCY_CODE_VOLTAGE_INSIDE:     status = "voltage_inside";     break;
    case CANMAT_EMCY_CODE_VOLTAGE_OUTPUT:     status = "voltage_output";     break;
    case CANMAT_EMCY_CODE_TEMP:               status = "temp";               break;
    case CANMAT_EMCY_CODE_TEMP_AMBIENT:       status = "temp_ambient";       break;
    case CANMAT_EMCY_CODE_TEMP_DEVICE:        status = "temp_device";        break;
    case CANMAT_EMCY_CODE_HARDWARE:           status = "hardware";           break;
    case CANMAT_EMCY_CODE_SOFTWARE:           status = "software";           break;
    case CANMAT_EMCY_CODE_SOFTWARE_INTERNAL:  status = "software_internal";  break;
    case CANMAT_EMCY_CODE_SOFTWARE_USER:      status = "software_user";      break;
    case CANMAT_EMCY_CODE_SOFTWARE_DATA:      status = "software_data";      break;
    case CANMAT_EMCY_CODE_ADDITIONAL_MODULES: status = "additional_modules"; break;
    case CANMAT_EMCY_CODE_MONITORING:         status = "monitoring";         break;
    case CANMAT_EMCY_CODE_MONITORING_COMM:    status = "monitoring_comm";    break;
    case CANMAT_EMCY_CODE_MONITORING_PROTO:   status = "monitoring_proto";   break;
    case CANMAT_EMCY_CODE_EXTERNAL:           status = "external";           break;
    case CANMAT_EMCY_CODE_ADDITIONAL_FUNC:    status = "additional_func";    break;
    case CANMAT_EMCY_CODE_DEVICE_SPECIFIC:    status = "device_specific";    break;
    }

    printf( "emcy (0x%03x), node 0x%02x, eec '%s' (0x%02x), er 0x%x \n",
            func, node, status, eec, er);
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
