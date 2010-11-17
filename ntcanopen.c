/*
 * Copyright (c) 2008, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the Georgia Tech Research Corporation nor
 *       the names of its contributors may be used to endorse or
 *       promote products derived from this software without specific
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEORGIA TECH RESEARCH CORPORATION ''AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GEORGIA
 * TECH RESEARCH CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \file ntcanopen.c
 *
 * \brief CANopen implementation using esd's NTCAN API
 *
 * \author Neil Dantam
 *
 * \bug AMC servo drives require expedited SDO reads to have a full
 * 8-byte data portion.  We must accomodate.
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ntcan.h>
#include <assert.h>
#include <errno.h>
#include "ntcanopen.h"

extern int verbosity;

void canOpenTranslateSDO( CMSG *dst, const sdo_msg_t *src, const int is_response ) {
    assert( src->length <= 4 );
    dst->id = (is_response ? 0x580 : 0x600 ) + src->node;
    dst->len = src->length + 4;
    //dst->len = 8;
    dst->data[0] = src->command;
    dst->data[1] = src->index & 0xFF;
    dst->data[2] = (src->index >> 8) & 0xFF;
    dst->data[3] = src->subindex;
    memcpy( &(dst->data[4]), &(src->data[0]), src->length );
    bzero( &(dst->data[ 4 + src->length ]), 4 - src->length );
}

void canOpenTranslateCAN( sdo_msg_t *dst, const CMSG *src, const int is_request ) {
    assert( src->len <= 8 );
    dst->node = src->id - (is_request ? 0x600 : 0x580);
    dst->length = src->len - 4;
    dst->command = src->data[0];
    dst->index = src->data[1] + (src->data[2] << 8);
    dst->subindex = src->data[3];
    memcpy( &(dst->data[0]), &(src->data[4]), dst->length );
    bzero( &(dst->data[ dst->length ]), 4 - dst->length );
}

NTCAN_RESULT canOpenIdAddSDOResponse( NTCAN_HANDLE h, const uint8_t node ) {
    return canIdAdd(h, CANOPEN_SDO_RESP_ID( node ) ) ;
}

NTCAN_RESULT canOpenIdDeleteSDOResponse( NTCAN_HANDLE h, const uint8_t node ) {
    return canIdDelete(h, CANOPEN_SDO_RESP_ID( node ) ) ;
}

NTCAN_RESULT canOpenWriteSDO( const NTCAN_HANDLE h, const sdo_msg_t *sdomsg ) {
    CMSG msg;
    canOpenTranslateSDO( &msg, sdomsg, 0);
    msg.len = 8; // AMC sucks and barfs if msg isn't the full 8 bytes
    int32_t num = 1;
    return canWrite( h, &msg, &num, NULL );
}

NTCAN_RESULT canOpenWaitSDO( const NTCAN_HANDLE h, sdo_msg_t *sdomsg ) {
    CMSG msg;
    NTCAN_RESULT ntr;
    uint32_t num = 1;
    do{
        ntr = canRead(h, &msg, &num, NULL );
        if( ntr != NTCAN_SUCCESS ) return ntr;
    }while( msg.id != CANOPEN_SDO_RESP_ID( sdomsg->node ) );
    if( sdomsg )
        canOpenTranslateCAN( sdomsg, &msg, 0 );
    return ntr;
}


NTCAN_RESULT canOpenSDOWriteWait( const NTCAN_HANDLE h, const sdo_msg_t *req,
                                  sdo_msg_t *resp ) {
    NTCAN_RESULT r = canOpenWriteSDO( h, req );
    if( r != NTCAN_SUCCESS ) return r;
    if( ! resp ) {
        sdo_msg_t m;
        m.node = req->node;
        return  canOpenWaitSDO( h, &m );
    }
    resp->node = req->node;
    return  canOpenWaitSDO( h, resp );
}

static void put_sdo_dl_args( sdo_msg_t *sdo, uint8_t node,
                             uint16_t index, uint8_t subindex ) {
    sdo->command = canOpenCommand( CANOPEN_EX_DL, 0, 1, 0 );
    sdo->node = node;
    sdo->index = index;
    sdo->subindex = subindex;
}

static void put_sdo_ul_args( sdo_msg_t *sdo, uint8_t node,
                             uint16_t index, uint8_t subindex ) {
    sdo->command = canOpenCommand( CANOPEN_EX_UL, 0, 1, 0 );
    sdo->node = node;
    sdo->index = index;
    sdo->subindex = subindex;
    sdo->length = 0;
}

NTCAN_RESULT canOpenSDOWriteWait_dl_u8( NTCAN_HANDLE h, uint8_t *rcmd,
                                        uint8_t node,
                                        uint16_t index, uint8_t subindex,
                                        uint8_t value) {
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_dl_args( &msg, node, index, subindex );
    canOpenPut_uint8( &msg, value );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    return ntr;
}

NTCAN_RESULT canOpenSDOWriteWait_dl_u16( NTCAN_HANDLE h, uint8_t *rcmd,
                                         uint8_t node,
                                         uint16_t index, uint8_t subindex,
                                         uint16_t value) {
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_dl_args( &msg, node, index, subindex );
    canOpenPut_uint16( &msg, value );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    return ntr;
}

NTCAN_RESULT canOpenSDOWriteWait_dl_u32( NTCAN_HANDLE h, uint8_t *rcmd,
                                         uint8_t node,
                                         uint16_t index, uint8_t subindex,
                                         uint32_t value) {
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_dl_args( &msg, node, index, subindex );
    canOpenPut_uint32( &msg, value );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    return ntr;
}


NTCAN_RESULT canOpenSDOWriteWait_dl_i32( NTCAN_HANDLE h, uint8_t *rcmd,
                                         uint8_t node,
                                         uint16_t index, uint8_t subindex,
                                         int32_t value) {
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_dl_args( &msg, node, index, subindex );
    canOpenPut_int32( &msg, value );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    return ntr;
}

NTCAN_RESULT canOpenSDOWriteWait_ul_u8( NTCAN_HANDLE h, uint8_t *rcmd,
                                        uint8_t *value, uint8_t node,
                                        uint16_t index, uint8_t subindex ){
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_ul_args( &msg, node, index, subindex );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    if( value ) *value = canOpenGet_uint8( &rmsg );
    return ntr;
}

NTCAN_RESULT canOpenSDOWriteWait_ul_u16( NTCAN_HANDLE h, uint8_t *rcmd,
                                         uint16_t *value, uint8_t node,
                                         uint16_t index, uint8_t subindex ){
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_ul_args( &msg, node, index, subindex );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    if( value ) *value = canOpenGet_uint16( &rmsg );
    return ntr;
}

NTCAN_RESULT canOpenSDOWriteWait_ul_u32( NTCAN_HANDLE h, uint8_t *rcmd,
                                         uint32_t *value, uint8_t node,
                                         uint16_t index, uint8_t subindex ){
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_ul_args( &msg, node, index, subindex );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    if( value ) *value = canOpenGet_uint32( &rmsg );
    return ntr;
}

NTCAN_RESULT canOpenSDOWriteWait_ul_i32( NTCAN_HANDLE h, uint8_t *rcmd,
                                         int32_t *value, uint8_t node,
                                         uint16_t index, uint8_t subindex ){
    NTCAN_RESULT ntr;
    sdo_msg_t msg, rmsg;
    put_sdo_ul_args( &msg, node, index, subindex );
    ntr = canOpenSDOWriteWait( h, &msg, &rmsg );
    if( rcmd ) *rcmd = rmsg.command;
    if( value ) *value = canOpenGet_int32( &rmsg );
    return ntr;
}

void canOpenDumpSDO( const sdo_msg_t *sdo ) {
    printf( "%02x.%02x[%04x.%02x] ", sdo->node, sdo->command, sdo->index, sdo->subindex);
    int i;
    for( i = 0; i < sdo->length; i++ )
        printf("%02x:", sdo->data[i] );
}

uint8_t canOpenCommand( const canopen_command_spec_t command_spec,
                        const uint8_t nodata_len,
                        const uint8_t is_expedited,
                        const uint8_t is_len_in_cmd ) {
    uint8_t c = 0;
    assert( nodata_len <= 3 );
    assert(  (is_expedited && is_len_in_cmd) || ! nodata_len );
    c |= is_len_in_cmd?1:0;
    c |= (is_expedited?1:0) << 1;
    c |= (nodata_len & 0x3) << 2;
    c |= (command_spec & 0x7) << 5;
    return c;
}


void canOpenPut_uint8(sdo_msg_t *s, uint8_t d) {
    s->length = 1;
    s->data[0] = d ;
    bzero( &(s->data[1]), 3);
}

uint8_t canOpenGet_uint8(sdo_msg_t *s) {
    assert( s->length >= 1 );
    uint8_t d = 0;
    d |= s->data[0];
    return d;
}
void canOpenPut_uint16(sdo_msg_t *s, uint16_t d) {
    s->length = 2;
    s->data[0] = d & 0xff;
    s->data[1] = (d >> 8) & 0xff;
    bzero( &(s->data[2]), 2);
}

uint16_t canOpenGet_uint16(sdo_msg_t *s) {
    assert( s->length >= 2 );
    uint16_t d = 0;
    d |= s->data[0];
    d |= (s->data[1] << 8);
    return d;
}

void canOpenPut_int16(sdo_msg_t *s, int16_t d) {
    s->length = 2;
    s->data[0] = d & 0xff;
    s->data[1] = (d >> 8) & 0xff;
    bzero( &(s->data[2]), 2);
}

int16_t canOpenGet_int16(sdo_msg_t *s) {
    assert( s->length >= 2 );
    int16_t d = 0;
    d |= s->data[0];
    d |= (s->data[1] << 8);
    return d;
}


void canOpenPut_uint32(sdo_msg_t *s, uint32_t d) {
    s->length = 4;
    s->data[0] = d & 0xff;
    s->data[1] = (d >> 8) & 0xff;
    s->data[2] = (d >> 16) & 0xff;
    s->data[3] = (d >> 24) & 0xff;
}

uint32_t canOpenGet_uint32(sdo_msg_t *s) {
    assert( s->length >= 2 );
    uint32_t d = 0;
    d |= s->data[0];
    d |= (s->data[1] << 8);
    d |= (s->data[2] << 16);
    d |= (s->data[3] << 24);
    return d;
}

void canOpenPut_int32(sdo_msg_t *s, int32_t d) {
    s->length = 2;
    s->data[0] = d & 0xff;
    s->data[1] = (d >> 8) & 0xff;
    s->data[2] = (d >> 16) & 0xff;
    s->data[3] = (d >> 24) & 0xff;
}

int32_t canOpenGet_int32(sdo_msg_t *s) {
    assert( s->length >= 2 );
    int32_t d = 0;
    d |= s->data[0];
    d |= (s->data[1] << 8);
    d |= (s->data[2] << 16);
    d |= (s->data[3] << 24);
    return d;
}



/* int canOpenDictCompare( const void *a, const void *b ) { */
/*   const canopen_dict_entry_t *aa = a, *bb = b; */
/*   uint32_t u = aa->node | (aa->desc->subindex << 8 ) | (aa->desc->index << 16); */
/*   uint32_t v = bb->node | (bb->desc->subindex << 8 ) | (bb->desc->index << 16); */
/*   if( u == v ) return 0; */
/*   if( u > v ) return 1; */
/*   if( u < v ) return -1; */
/*   assert(0); */
/* } */

/* void canOpenDictInit( canopen_dict_t *dict,  */
/*                       canopen_dict_entry_t *entries, size_t count ) { */
/*   assert(  */
/*          ({  */
/*            int r = 1, i; */
/*            for( i = 0; i < count; i ++ ) { */
/*              r = r && ( entries[i].desc != NULL ); */
/*              r = r && ( entries[i].data != NULL ); */
/*            }; */
/*            r; */
/*          }) ); */
/*   qsort( entries, count, sizeof( canopen_dict_entry_t ), canOpenDictCompare ); */
/*   dict->entries = entries; */
/*   dict->entry_count = count; */
/* } */


/* static canopen_dict_entry_t *canOpenDictLookup( canopen_dict_t *dict, uint8_t node, */
/*                                                 uint16_t index, uint8_t subindex ) { */
/*   canopen_dict_entry_t kent; */
/*   sdo_desc_t kdesc; */
/*   kent.desc = &kdesc; */
/*   kent.node = node; */
/*   kdesc.index = index; */
/*   kdesc.subindex = subindex; */

/*   return  bsearch(&kent, dict->entries,  */
/*                   dict->entry_count, sizeof( canopen_dict_entry_t ), */
/*                   canOpenDictCompare ); */
/* } */

/* static void canOpenDictDataCopy( sdo_msg_data_t *dst, sdo_msg_data_t *src,  */
/*                                  canopen_datatype_t type ) { */
/*   switch( type ) { */
/*   case CANOPEN_UINT16: */
/*     dst->u16 = src->u16; */
/*     break; */
/*   case CANOPEN_UINT32: */
/*     dst->u32 = src->u32; */
/*     break; */
/*   case CANOPEN_INT16: */
/*     dst->i16 = src->i16; */
/*     break; */
/*   case CANOPEN_INT32: */
/*     dst->i32 = src->i32; */
/*     break; */
/*   default: */
/*     assert(0); */
/*   } */
/* } */

/* int canOpenDictRead( canopen_dict_t *dict, */
/*                      sdo_msg_data_t *data, struct timespec *modtime, */
/*                      uint8_t node, uint16_t index, uint8_t subindex ) { */

/*   canopen_dict_entry_t *pent = canOpenDictLookup( dict, node, index, subindex ); */

/*   if( ! pent ) return -1; */
/*   canOpenDictDataCopy( data, pent->data, pent->desc->type ); */
/*   if( modtime && pent->modtime )  */
/*     memcpy( modtime, pent->modtime, sizeof( struct timespec ) ); */

/*   return 0; */
/* } */

/* int canOpenDictWrite( canopen_dict_t *dict, */
/*                       uint8_t node, uint16_t index, uint8_t subindex, */
/*                       sdo_msg_data_t *data, struct timespec *modtime */
/*                       ) { */
/*   canopen_dict_entry_t *pent = canOpenDictLookup( dict, node, index, subindex ); */
/*   if( ! pent ) return -1; */

/*   canOpenDictDataCopy( pent->data, data, pent->desc->type ); */
/*   return 0; */
/* } */

void canOpenTranslateNMT( CMSG *msg, uint8_t node, canopen_nmt_msg_t nmt_msg ) {
    msg->id = 0;
    msg->len = 2;
    msg->data[0] = nmt_msg;
    msg->data[1] = node;
}

NTCAN_RESULT canOpenWriteNMT( NTCAN_HANDLE h, uint8_t node,
                              canopen_nmt_msg_t nmt_msg ) {
    CMSG msg;
    canOpenTranslateNMT( &msg, node, nmt_msg );
    int32_t num = 1;
    return canWrite( h, &msg, &num, NULL );
}


const char *canResultString( int i ) {
    NTCAN_RESULT ntr = i;
    switch( ntr ) {
    case NTCAN_SUCCESS: return "SUCCESS";
    case NTCAN_RX_TIMEOUT: return "RX_TIMEOUT";
    case NTCAN_TX_TIMEOUT: return "TX_TIMEOUT";
    case NTCAN_TX_ERROR: return "TX_ERROR";
    case NTCAN_CONTR_OFF_BUS: return "CONTR_OFF_BUS";
    case NTCAN_CONTR_BUSY: return "CONTR_BUSY";
    case NTCAN_CONTR_WARN: return "CONTR_WARN";
    case NTCAN_NO_ID_ENABLED: return "NO_ID_ENABLED";
    case NTCAN_ID_ALREADY_ENABLED: return "ID_ALREADY_ENABLED";
    case NTCAN_INVALID_FIRMWARE: return "INVALID_FIRMWARE";
    case NTCAN_MESSAGE_LOST: return "MESSAGE_LOST";
    case NTCAN_INVALID_HARDWARE: return "INVALID_HARDWARE";
    case NTCAN_PENDING_WRITE: return "PENDING_WRITE";
    case NTCAN_PENDING_READ: return "PENDING_READ";
    case NTCAN_INVALID_DRIVER: return "INVALID_DRIVER";
    case NTCAN_SOCK_CONN_TIMEOUT: return "SOCK_CONN_TIMEOUT";
    case NTCAN_SOCK_CMD_TIMEOUT: return "SOCK_CMD_TIMEOUT";
    case NTCAN_SOCK_HOST_NOT_FOUND: return "SOCK_HOST_NOT_FOUND";
    case NTCAN_INVALID_PARAMETER: return "INVALID_PARAMETER";
    case NTCAN_INVALID_HANDLE: return "INVALID_HANDLE";
    case NTCAN_NET_NOT_FOUND: return "NET_NOT_FOUND";
    case NTCAN_INSUFFICIENT_RESOURCES: return "INSUFFICIENT_RESOURCES";
    case NTCAN_OPERATION_ABORTED: return "OPERATION_ABORTED";
    case NTCAN_WRONG_DEVICE_STATE: return "WRONG_DEVICE_STATE";
    case NTCAN_HANDLE_FORCED_CLOSE: return "HANDLE_FORCED_CLOSE";
    case NTCAN_NOT_IMPLEMENTED: return "NOT_IMPLEMENTED";
    case NTCAN_NOT_SUPPORTED: return "NOT_SUPPORTED";
    case NTCAN_CONTR_ERR_PASSIVE: return "CONTR_ERR_PASSIVE";
    default: return "unknown";
    }
}




int32_t canOpenDecode_int32( uint8_t *p ) {
    return
        p[0] |
        p[1] << 8 |
        p[2] << 16 |
        p[3] << 24 ;
}
