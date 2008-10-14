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
 * \file ntcanopen.h
 *
 * \brief CANopen implementation using esd's NTCAN API
 *
 * \author Neil Dantam
 *
 * \bugs Deferring implentation of segmented messages
 */

#ifndef NTCANOPEN_H_
#define NTCANOPEN_H_

#ifdef __cplusplus
//extern "C" {
#endif

/// container struct for SDO requests
typedef struct {
    uint8_t node;
    uint8_t command;
    uint16_t index;
    uint8_t subindex;
    uint8_t length;
    uint8_t data[4];
} sdo_msg_t;

/// translates an sdo_msg_t struct to a CMSG struct
void canOpenTranslateSDO( CMSG *dst, const sdo_msg_t *src, const int is_response );

/// translates a CMSG struct to a sdo_msg_t struct
void canOpenTranslateCAN( sdo_msg_t *dst, const CMSG *src, const int is_request );

/** Adds the COB-ID for SDO reponses from node
    \param node CANopen node id
    \post h will be listening for SDO responses from node
*/
NTCAN_RESULT canOpenIdAddSDOResponse( NTCAN_HANDLE h, const uint8_t node );

/// Sends a request to the SDO server
NTCAN_RESULT canOpenWriteSDO( const NTCAN_HANDLE h, const sdo_msg_t *sdomsg );

/** Waits to receive an SDO message, discarding all other messages received

    \pre h is a valid handle and is set to receive the proper COB-ID
    for sdomsg->node

    \post all messages in h prior to the requested SDO are discarded
*/
NTCAN_RESULT canOpenWaitSDO( const NTCAN_HANDLE h, sdo_msg_t *sdomsg );

/// writes the sdo to stdout
void canOpenDumpSDO( const sdo_msg_t *sdo );

/// Values for command specifier of SDO command byte
typedef enum {
    CANOPEN_SEG_DL = 0, //segment download
    CANOPEN_EX_DL = 1,  //expedited download (client->server)
    CANOPEN_EX_UL = 2,  //expedited upload (server->client)
    CANOPEN_SEG_UL = 3, //segment upload
    CANOPEN_ABORT = 4
} canopen_command_spec_t;



/** Generates a CANopen command byte
    \param command_spec
*/
uint8_t canOpenCommand( const canopen_command_spec_t command_spec,
                        const uint8_t nodata_len,
                        const uint8_t is_expedited, const uint8_t is_len_in_cmd );

//#ifdef __cplusplus
//}
//#endif

#endif
