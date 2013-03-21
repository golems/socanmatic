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



static int dict_compar( const void *a, const void *b ) {
    const char *a1 = (const char *)a;
    const socia_obj_t *b1 = (const socia_obj_t*)b;
    return strcasecmp( a1, b1->parameter_name );
}

/* Return the pointer to the item in dict with given name */
socia_obj_t *socia_dict_search_name( const struct socia_dict *dict, const char *name ) {
    return (socia_obj_t*) bsearch( name, dict->obj,
                                   dict->length, sizeof( socia_obj_t ),
                                   dict_compar );
}

socia_status_t socia_obj_ul( int fd, uint8_t node, const socia_obj_t *obj, socia_scalar_t *val ) {
    if( NULL == obj  ||
        SOCIA_OBJ_CODE_VAR != obj->object_type )
    {
        return SOCIA_ERR_PARAM;
    }

    uint8_t rccs;
    ssize_t r;
    switch(obj->data_type) {
    case SOCIA_DATA_TYPE_INTEGER8:
        r = socia_sdo_ul_i8( fd, &rccs,
                             &val->i8,
                             node,
                             obj->index, obj->subindex );
        break;
    case SOCIA_DATA_TYPE_INTEGER16:
        r = socia_sdo_ul_i16( fd, &rccs,
                              &val->i16,
                              node,
                              obj->index, obj->subindex );
        break;
    case SOCIA_DATA_TYPE_INTEGER32:
        r = socia_sdo_ul_i32( fd, &rccs,
                              &val->i32,
                              node,
                              obj->index, obj->subindex );
        break;
    case SOCIA_DATA_TYPE_UNSIGNED8:
        r = socia_sdo_ul_u8( fd, &rccs,
                             &val->u8,
                             node,
                             obj->index, obj->subindex );
        break;
    case SOCIA_DATA_TYPE_UNSIGNED16:
        r = socia_sdo_ul_u16( fd, &rccs,
                              &val->u16,
                              node,
                              obj->index, obj->subindex );
        break;
    case SOCIA_DATA_TYPE_UNSIGNED32:
        r = socia_sdo_ul_u32( fd, &rccs,
                              &val->u32,
                              node,
                              obj->index, obj->subindex );
        break;
    default: return SOCIA_ERR_PARAM;
    }
    if( r < 0 ) return SOCIA_ERR_OS;
    else return SOCIA_OK;
}


socia_status_t socia_obj_dl( int fd, uint8_t node, const socia_obj_t *obj, const socia_scalar_t *val ) {
    if( NULL == obj  || SOCIA_OBJ_CODE_VAR != obj->object_type )
    {
        return SOCIA_ERR_PARAM;
    }

    uint8_t rccs;
    ssize_t r;
    switch(obj->data_type) {
    case SOCIA_DATA_TYPE_INTEGER8:
        r = socia_sdo_dl_i8( fd, &rccs,
                             node,
                             obj->index, obj->subindex,
                             val->i8 );
        break;
    case SOCIA_DATA_TYPE_INTEGER16:
        r = socia_sdo_dl_i16( fd, &rccs,
                              node,
                              obj->index, obj->subindex,
                              val->i16 );
        break;
    case SOCIA_DATA_TYPE_INTEGER32:
        r = socia_sdo_dl_i32( fd, &rccs,
                              node,
                              obj->index, obj->subindex,
                              val->i32 );
        break;
    case SOCIA_DATA_TYPE_UNSIGNED8:
        r = socia_sdo_dl_u8( fd, &rccs,
                             node,
                             obj->index, obj->subindex,
                             val->u8 );
        break;
    case SOCIA_DATA_TYPE_UNSIGNED16:
        r = socia_sdo_dl_u16( fd, &rccs,
                              node,
                              obj->index, obj->subindex,
                              val->u16 );
        break;
    case SOCIA_DATA_TYPE_UNSIGNED32:
        r = socia_sdo_dl_u32( fd, &rccs,
                              node,
                              obj->index, obj->subindex,
                              val->u32 );
        break;
    default: return SOCIA_ERR_PARAM;
    }
    if( r < 0 ) return SOCIA_ERR_OS;
    else return SOCIA_OK;
}


socia_status_t socia_obj_dl_str( int fd, uint8_t node, const socia_obj_t *obj, const char *val ) {
    if( NULL == obj ) return SOCIA_ERR_PARAM;

    socia_scalar_t sval;
    if( socia_typed_parse( obj->data_type, val, &sval ) ) {
        return SOCIA_ERR_PARAM;
    }

    return socia_obj_dl( fd, node, obj, &sval );
}

socia_status_t socia_typed_parse( socia_data_type_t type, const char *str, socia_scalar_t *val ) {
    unsigned long u;
    long i;

    errno = 0;
    switch(type) {
    case SOCIA_DATA_TYPE_INTEGER8:
        i = strtol( str, NULL, 0 );
        if ( i > INT8_MAX || i < INT8_MIN || 0 != errno ) return SOCIA_ERR_PARAM;
        else val->i8 = (int8_t)i;
        break;
    case SOCIA_DATA_TYPE_INTEGER16:
        i = strtol( str, NULL, 0 );
        if ( i > INT16_MAX || i < INT16_MIN || 0 != errno ) return SOCIA_ERR_PARAM;
        else val->i16 = (int16_t)i;
        break;
    case SOCIA_DATA_TYPE_INTEGER32:
        i = strtol( str, NULL, 0 );
        if ( i > INT32_MAX || i < INT32_MIN || 0 != errno ) return SOCIA_ERR_PARAM;
        else val->i32 = (int32_t)i;
        break;
    case SOCIA_DATA_TYPE_UNSIGNED8:
        u = strtoul( str, NULL, 0 );
        if ( u > UINT8_MAX || 0 != errno ) return SOCIA_ERR_PARAM;
        else val->u8 = (uint8_t)u;
        break;
    case SOCIA_DATA_TYPE_UNSIGNED16:
        u = strtoul( str, NULL, 0 );
        if ( u > UINT16_MAX || 0 != errno ) return SOCIA_ERR_PARAM;
        else val->u16 = (uint16_t)u;
        break;
    case SOCIA_DATA_TYPE_UNSIGNED32:
        u = strtoul( str, NULL, 0 );
        if ( u > UINT32_MAX || 0 != errno ) return SOCIA_ERR_PARAM;
        else val->u32 = (uint32_t)u;
        break;
    default: return SOCIA_ERR_PARAM;
    }

    return SOCIA_OK;
}



/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
