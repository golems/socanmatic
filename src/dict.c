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
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#include "socanmatic.h"
#include "socanmatic_private.h"



static int dict_compar_name( const void *a, const void *b ) {
    const char *a1 = (const char *)a;
    const canmat_dict_name_tree_t *b1 = (const canmat_dict_name_tree_t *)b;
    return strcasecmp( a1, b1->parameter_name );
}

static int dict_compar_index( const void *a, const void *b ) {
    int32_t *a1 = (int32_t*)a;
    const canmat_obj_t *b1 = (const canmat_obj_t*)b;
    return *a1 - ((b1->index << 16) | b1->subindex);
}

canmat_obj_t *canmat_dict_search_name( const struct canmat_dict *dict, const char *name ) {
    canmat_dict_name_tree_t *p =
        (canmat_dict_name_tree_t *) bsearch( name, dict->btree_name,
                                             dict->length, sizeof( dict->btree_name[0] ),
                                             dict_compar_name );
    if( p ) {
        return dict->obj + p->i;
    } else {
        return NULL;
    }
}


canmat_obj_t *canmat_dict_search_index( const struct canmat_dict *dict, uint16_t idx, uint8_t subindex ) {
    int32_t key = idx << 16 | subindex;
    return  (canmat_obj_t *) bsearch( &key, dict->obj,
                                      dict->length, sizeof( dict->obj[0] ),
                                      dict_compar_index );
}

canmat_status_t canmat_obj_ul( canmat_iface_t *cif, uint8_t node, const canmat_obj_t *obj,
                               canmat_scalar_t *val, uint32_t *err_val ) {
    if( NULL == obj  ||
        CANMAT_OBJECT_TYPE_VAR != obj->object_type )
    {
        return CANMAT_ERR_PARAM;
    }

    canmat_sdo_msg_t req = {
        .node = node,
        .index = obj->index,
        .subindex = obj->subindex,
        .data_type = obj->data_type
    };
    canmat_sdo_msg_t resp;

    canmat_status_t r = canmat_sdo_ul( cif, &req, &resp );
    assert( resp.length <= 4 );
    if( CANMAT_OK == r ) {
        memcpy( val, &resp.data, sizeof(resp.length) );
    } else if (CANMAT_ERR_ABORT == r ) {
        void * p = err_val ? (void*)err_val : (void*)val;
        memcpy( p, &resp.data, sizeof(resp.length) );
    }
    return r;
}


canmat_status_t canmat_obj_dl( canmat_iface_t *cif,
                               uint8_t node, const canmat_obj_t *obj, const canmat_scalar_t *val,
                               uint32_t *err_val)
{
    if( NULL == obj  || CANMAT_OBJECT_TYPE_VAR != obj->object_type )
    {
        return CANMAT_ERR_PARAM;
    }

    canmat_sdo_msg_t req = {
        .node = node,
        .index = obj->index,
        .subindex = obj->subindex,
        .data_type = obj->data_type
    };
    memcpy( &req.data, val, sizeof(req.data) );
    canmat_sdo_msg_t resp;

    canmat_status_t r = canmat_sdo_dl( cif, &req, &resp );
    if( err_val ) {
        if( CANMAT_CS_ABORT == resp.cmd_spec ) {
            *err_val = resp.data.u32;
        } else {
            *err_val = 0;
        }
    }
    return r;
}


canmat_status_t canmat_obj_dl_str( canmat_iface_t *cif, uint8_t node, const canmat_obj_t *obj, const char *val,
                                   uint32_t *err_val) {
    if( NULL == obj ) return CANMAT_ERR_PARAM;

    canmat_scalar_t sval;
    if( canmat_typed_parse( obj->data_type, val, &sval ) ) {
        return CANMAT_ERR_PARAM;
    }

    return canmat_obj_dl( cif, node, obj, &sval, err_val );
}

canmat_status_t canmat_typed_parse( enum canmat_data_type type, const char *str, canmat_scalar_t *val ) {
    unsigned long u;
    long i;

    errno = 0;
    switch(type) {
    case CANMAT_DATA_TYPE_INTEGER8:
        i = strtol( str, NULL, 0 );
        if ( i > INT8_MAX || i < INT8_MIN || 0 != errno ) return CANMAT_ERR_PARAM;
        else val->i8 = (int8_t)i;
        break;
    case CANMAT_DATA_TYPE_INTEGER16:
        i = strtol( str, NULL, 0 );
        if ( i > INT16_MAX || i < INT16_MIN || 0 != errno ) return CANMAT_ERR_PARAM;
        else val->i16 = (int16_t)i;
        break;
    case CANMAT_DATA_TYPE_INTEGER32:
        i = strtol( str, NULL, 0 );
        if ( i > INT32_MAX || i < INT32_MIN || 0 != errno ) return CANMAT_ERR_PARAM;
        else val->i32 = (int32_t)i;
        break;
    case CANMAT_DATA_TYPE_UNSIGNED8:
        u = strtoul( str, NULL, 0 );
        if ( u > UINT8_MAX || 0 != errno ) return CANMAT_ERR_PARAM;
        else val->u8 = (uint8_t)u;
        break;
    case CANMAT_DATA_TYPE_UNSIGNED16:
        u = strtoul( str, NULL, 0 );
        if ( u > UINT16_MAX || 0 != errno ) return CANMAT_ERR_PARAM;
        else val->u16 = (uint16_t)u;
        break;
    case CANMAT_DATA_TYPE_UNSIGNED32:
        u = strtoul( str, NULL, 0 );
        if ( u > UINT32_MAX || 0 != errno ) return CANMAT_ERR_PARAM;
        else val->u32 = (uint32_t)u;
        break;
    default: return CANMAT_ERR_PARAM;
    }

    return CANMAT_OK;
}


static int print_masks( struct canmat_code_descriptor fmt[], unsigned u ) {

    int r = 0;
    size_t maxlen = 0;
    for( size_t i = 0; fmt[i].value && fmt[i].name; i ++ ) {
        size_t n = strlen(fmt[i].name);
        if( n > maxlen ) maxlen = n;
    }

    printf("0x%x\n", u );
    for( size_t i = 0; fmt[i].value && fmt[i].name; i ++ ) {
        size_t space_size = maxlen + 1 - strlen(fmt[i].name);
        char space[ space_size + 1 ];
        memset( space, ' ', space_size );
        space[space_size] = '\0';
        r += printf( "> %s:%s%d, (0x%08x)\n", fmt[i].name,  space,
                     (u & (unsigned)fmt[i].value) ? 1 : 0, (unsigned)(fmt[i].value));
    }
    return r;
}

// FIXME: some fiddly issues with signs here

static int print_enum( struct canmat_code_descriptor fmt[], int64_t u ) {
    printf("%"PRId64" (0x%"PRIx64")\n", u, (uint64_t)u );
    for( size_t i = 0; fmt[i].name; i ++ ) {
        if( u == fmt[i].value ) {
            if( 0 != strcmp( fmt[i].name, fmt[i].description ) ) {
                printf( "> %s: %s\n",
                        fmt[i].name, fmt[i].description );
            } else {
                printf( "> %s\n", fmt[i].name );
            }
        }
    }
    return 0;
}


canmat_status_t canmat_obj_print( FILE *f, const canmat_obj_t *obj, canmat_scalar_t *val ) {
    switch(obj->data_type) {
    case CANMAT_DATA_TYPE_INTEGER8:
        if (obj->value_descriptor) {
            print_enum( obj->value_descriptor, val->i8 );
        } else {
            fprintf( f, "%"PRId8"\n", val->i8 );
        }
        return CANMAT_OK;
    case CANMAT_DATA_TYPE_INTEGER16:
        if (obj->value_descriptor) {
            print_enum( obj->value_descriptor, val->i16 );
        } else {
            fprintf( f, "%"PRId16"\n", val->i16 );
        }
        return CANMAT_OK;
    case CANMAT_DATA_TYPE_INTEGER32:
        if (obj->value_descriptor) {
            print_enum( obj->value_descriptor, val->i32 );
        } else {
            fprintf( f, "%"PRId32"\n", val->i32 );
        }
        return CANMAT_OK;
    case CANMAT_DATA_TYPE_UNSIGNED8:
        if( obj->mask_descriptor ) {
            print_masks( obj->mask_descriptor, val->u8 );
        } else if (obj->value_descriptor) {
            print_enum( obj->value_descriptor, val->u8 );
        } else {
            fprintf( f, "0x%"PRIx8" (%"PRId8")\n", val->u8, val->u8 );
        }
        return CANMAT_OK;
    case CANMAT_DATA_TYPE_UNSIGNED16:
        if( obj->mask_descriptor ) {
            print_masks( obj->mask_descriptor, val->u16 );
        } else if (obj->value_descriptor) {
            print_enum( obj->value_descriptor, val->u16 );
        } else {
            fprintf( f, "0x%"PRIx16" (%"PRId16")\n", val->u16, val->u16 );
        }
        return CANMAT_OK;
    case CANMAT_DATA_TYPE_UNSIGNED32:
        if( obj->mask_descriptor ) {
            print_masks( obj->mask_descriptor, val->u32 );
        } else if (obj->value_descriptor) {
            print_enum( obj->value_descriptor, val->u32 );
        } else {
            fprintf( f, "0x%"PRIx32" (%"PRId32")\n", val->u32, val->u32 );
        }
        return CANMAT_OK;
    default:
        return CANMAT_ERR_PARAM;
    }
}


/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
