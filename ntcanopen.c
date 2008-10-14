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
 * \bugs
 */


#include <string.h>
#include <stdio.h>
#include <ntcan.h>
#include <assert.h>
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
    canIdAdd( h, 0x580 + node );
}

NTCAN_RESULT canOpenWriteSDO( const NTCAN_HANDLE h, const sdo_msg_t *sdomsg ) {
    CMSG msg;
    canOpenTranslateSDO( &msg, sdomsg, 0);
    msg.len = 8; // AMC sucks and barfs if msg isn't the fully 8 bytes
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
    }while( msg.id != sdomsg->node + 0x580);
    canOpenTranslateCAN( sdomsg, &msg, 0 );
    return ntr;
}

void canOpenDumpSDO( const sdo_msg_t *sdo ) {
    printf( "%x.%x[%x.%x] ", sdo->node, sdo->command, sdo->index, sdo->subindex);
    int i;
    for( i = 0; i < sdo->length; i++ )
        printf("%x:", sdo->data[i] );
}

uint8_t canOpenCommand( const canopen_command_spec_t command_spec,
                        const uint8_t nodata_len,
                        const uint8_t is_expedited,
                        const uint8_t is_len_in_cmd ) {
    uint8_t c = 0;
    c |= is_len_in_cmd?1:0;
    c |= (is_expedited?1:0) << 1;
    c |= (nodata_len & 0x3) << 2;
    c |= (command_spec & 0x7) << 5;
    return c;
}
