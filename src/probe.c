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

#define PDO_MAX 8

canmat_status_t canmat_probe_pdo( const struct canmat_dict *dict,  canmat_iface_t *cif,  uint8_t node ) {
    canmat_sdo_msg_t resp;
    canmat_status_t  r;
    // RPDO
    for( uint8_t i = 0; i < PDO_MAX; i ++ ) {
        // com size
        uint8_t com_size;
        {
            canmat_sdo_msg_t req = { .index = (uint16_t)(CANMAT_RPDO_COM_BASE+i),
                                     .subindex = 0,
                                     .node = node,
                                     .data_type = CANMAT_DATA_TYPE_UNSIGNED8 };
            r = canmat_sdo_ul( cif, &req, &resp );

            if( CANMAT_ERR_ABORT == r ) continue;
            if( CANMAT_OK != r ) return r;
            if( CANMAT_SCS_EX_UL != resp.cmd_spec ) return CANMAT_ERR_PROTO;
            com_size = resp.data.u8;
        }


        printf("RPDO %"PRIu8"\n", i );
        printf("> com size: %d\n", com_size );

        // cob-id
        if( com_size > 0 ) {
            canmat_sdo_msg_t req = { .index = (uint16_t)(CANMAT_RPDO_COM_BASE+i),
                                     .subindex = 1,
                                     .node = node,
                                     .data_type = CANMAT_DATA_TYPE_UNSIGNED32 };
            r = canmat_sdo_ul( cif, &req, &resp );

            if( CANMAT_OK != r ) goto ABORT;
            if( CANMAT_SCS_EX_UL != resp.cmd_spec ) return CANMAT_ERR_PROTO;

            printf("> cob-id: 0x%x\n", resp.data.u32 & CANMAT_COBID_MASK );
            printf("> frame: %d\n", (resp.data.u32 & CANMAT_COBID_PDO_MASK_FRAME) ? 1 : 0 );
            printf("> valid: %d\n", (resp.data.u32 & (uint32_t)CANMAT_COBID_PDO_MASK_VALID) ? 1 : 0 );
        }
        // trans type
        if( com_size > 1 ) {
            canmat_sdo_msg_t req = { .index = (uint16_t)(CANMAT_RPDO_COM_BASE+i),
                                     .subindex = 2,
                                     .node = node,
                                     .data_type = CANMAT_DATA_TYPE_UNSIGNED8 };
            r = canmat_sdo_ul( cif, &req, &resp );

            if( CANMAT_OK != r ) goto ABORT;
            if( CANMAT_SCS_EX_UL != resp.cmd_spec ) return CANMAT_ERR_PROTO;

            printf("> trans_type: 0x%x\n", resp.data.u8 );

        }

        // mapping
        uint8_t map_size;
        {
            canmat_sdo_msg_t req = { .index = (uint16_t)(CANMAT_RPDO_MAP_BASE+i),
                                     .subindex = 0,
                                     .node = node,
                                     .data_type = CANMAT_DATA_TYPE_UNSIGNED8 };
            r = canmat_sdo_ul( cif, &req, &resp );

            if( CANMAT_OK != r ) goto ABORT;
            if( CANMAT_SCS_EX_UL != resp.cmd_spec ) return CANMAT_ERR_PROTO;
            map_size = resp.data.u8;
        }
        printf("> mapped: %d\n", map_size);
        for( uint8_t j = 0; j < map_size; j ++ ) {
            canmat_sdo_msg_t req = { .index = (uint16_t)(CANMAT_RPDO_MAP_BASE+i),
                                     .subindex = 1 + j,
                                     .node = node,
                                     .data_type = CANMAT_DATA_TYPE_UNSIGNED32 };
            r = canmat_sdo_ul( cif, &req, &resp );

            if( CANMAT_OK != r ) goto ABORT;
            if( CANMAT_SCS_EX_UL != resp.cmd_spec ) return CANMAT_ERR_PROTO;
            uint16_t idx = resp.data.u32 >> 16;
            uint8_t subidx = (resp.data.u32) >> 8 & 0xFF;
            canmat_obj_t *obj = canmat_dict_search_index( dict, idx, subidx );
            printf("  > %d: '%s' (%x.%x) (%d)\n",
                   j, obj ? obj->parameter_name : "unknown",
                   idx, subidx, resp.data.u32 & 0xFF);
        }

    }
    return CANMAT_OK;

ABORT:
    return r;
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
