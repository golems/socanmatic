/* -*- mode: C; c-basic-offset: 4 -*- */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*
 * Copyright (c) 2013, Georgia Tech Research Corporation
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





enum canmat_status canmat_pdo_remap(
    struct canmat_iface *cif, uint8_t node, uint8_t pdo, enum canmat_direction dir,
    int transmission_type, int inhibit_time, int event_timer,
    uint8_t cnt, const struct canmat_obj *objs[], uint32_t *err )
{

    // check parameters
    if( transmission_type > 0xFF ||
        inhibit_time > 0xFFFF    ||
        event_timer > 0xFFFF  )
    {
        return CANMAT_ERR_PARAM;
    }

    canmat_status_t  r;
    uint16_t idx_com;
    uint16_t idx_map;
    if( CANMAT_DL == dir ) {
        idx_com = (uint16_t)( (CANMAT_RPDO_COM_BASE+pdo) & 0xFFFF );
        idx_map = (uint16_t)( (CANMAT_RPDO_MAP_BASE+pdo) & 0xFFFF );
    } else if (CANMAT_UL == dir) {
        idx_com = (uint16_t)( (CANMAT_TPDO_COM_BASE+pdo) & 0xFFFF );
        idx_map = (uint16_t)( (CANMAT_TPDO_MAP_BASE+pdo) & 0xFFFF );
    } else {
        return CANMAT_ERR_PARAM;
    }

    // TODO: Upload some of these SDOs first and don't remap them if
    // they are already set properly

    // Read COMM
    uint32_t comm;
    r = canmat_sdo_ul_u32( cif, node, idx_com, 1,
                           &comm, err );
    if( CANMAT_OK != r ) return r;

    // Set valid bit 1 of sub-index 1 in COMM
    if( 0 == (comm & CANMAT_COBID_PDO_MASK_VALID) ) {
        r = canmat_sdo_dl_u32( cif, node, idx_com, 1,
                               comm | CANMAT_COBID_PDO_MASK_VALID, err );
        if( CANMAT_OK != r ) return r;
    }
    // Set subindex 00 to 00
    r = canmat_sdo_dl_u8( cif, node, idx_map, 0,
                          0, err );
    if( CANMAT_OK != r ) return r;

    // Modify mapping
    for( uint8_t i = 1; i <= cnt; i ++ ) {
        const struct canmat_obj *obj = objs[i - 1];
        int objsize = canmat_obj_bitsize( obj );
        if( objsize < 1 || 0 != objsize % 8 ) return CANMAT_ERR_PARAM;
        uint32_t map = (uint32_t)( (obj->index << 16) | (obj->subindex << 8) | (objsize & 0xFF) );
        r = canmat_sdo_dl_u32( cif, node, idx_map, i,
                               map, err );
        if( CANMAT_OK != r ) return r;
    }

    // Set transmission options
    if( transmission_type >= 0 ) {
        r = canmat_sdo_dl_u8( cif, node, idx_com, 2,
                              (uint8_t)transmission_type, err );
        if( CANMAT_OK != r ) return r;
    }
    if( inhibit_time >= 0 ) {
        r = canmat_sdo_dl_u16( cif, node, idx_com, 3,
                              (uint8_t)inhibit_time, err );
        if( CANMAT_OK != r ) return r;
    }
    if( event_timer >= 0 ) {
        r = canmat_sdo_dl_u16( cif, node, idx_com, 5,
                              (uint8_t)event_timer, err );
        if( CANMAT_OK != r ) return r;
    }

    // Set subindex 00h to number of mapped objects
    r = canmat_sdo_dl_u8( cif, node, idx_map, 0,
                          cnt, err );
    if( CANMAT_OK != r ) return r;

    // Set valid bit 0 of sub-index 1 in COMM
    r = canmat_sdo_dl_u32( cif, node, idx_com, 1,
                           comm & ~CANMAT_COBID_PDO_MASK_VALID, err );
    if( CANMAT_OK != r ) return r;

    return r;
}

enum canmat_status canmat_rpdo_send(
    struct canmat_iface *cif, uint8_t node, uint8_t pdo,
    uint8_t len, uint8_t data[] ) {
    // build can frame
    struct can_frame can;
    can.can_dlc = len;
    can.can_id = CANMAT_RPDO_COBID( node, pdo );
    memcpy( can.data, data, can.can_dlc );
    return canmat_iface_send( cif, &can );
}
