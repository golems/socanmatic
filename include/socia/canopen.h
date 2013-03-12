/* Copyright (c) 2008-2013, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Neil T. Dantam <ntd@gatech.edu>
 *            Can Erdogan <cerdogan@gatech.edu>
 *
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

#ifndef SOCIA_CANOPEN_H
#define SOCIA_CANOPEN_H

/**
 * \file socia_canopen.h
 *
 * \brief CANopen implementation using SocketCAN and esd's NTCAN
 * API. See the website
 * http://en.wikipedia.org/wiki/Canopen#Service_Data_Object_.28SDO.29_protocol
 * for details of service data object (SDO) protocol.
 *
 * \author Neil Dantam
 * \author Can Erdogan (bug fixes)
 *
 * \bug Deferring implementation of segmented messages
 *
 * \bug esd's driver will non-deterministically order loopback and
 * off-the-wire CAN messages.
 */

/// Produce COB-ID for request to node
#define SOCIA_SDO_REQ_ID(node) ((canid_t)(0x600 | (node)))

#define SOCIA_SDO_REQ_BASE 0x600
#define SOCIA_SDO_RESP_BASE 0x580

/// Produce COB-ID for response from node


#ifdef __cplusplus
extern "C" {
#endif

/// container struct for SDO requestsnnn
typedef struct socia_sdo_msg {
    // Message contents
    uint8_t command;   ///< CANopen command word, with ccs, reserved, n, e and s fields (see the website)
    uint16_t index;    ///< CANopen index
    uint8_t subindex;  ///< CANopen subindex

    // Additional information
    uint8_t node;      ///< CANopen Node ID
    uint8_t length;    ///< CANopen length of data, is either set in n (command) or in the data (depends on s)

    uint8_t data[];   ///< CANopen data section
} socia_sdo_msg_t;


typedef enum socia_nmt_msg {
    SOCIA_NMT_INVAL = -1,
    SOCIA_NMT_START_REMOTE = 0x1,
    SOCIA_NMT_STOP_REMOTE = 0x2,
    SOCIA_NMT_PRE_OP = 0x80,
    SOCIA_NMT_RESET_NODE = 0x81,
    SOCIA_NMT_RESET_COM = 0x82
} socia_nmt_msg_t;

/// Send an NMT Message
int socia_send_nmt( int fd, uint8_t node,
		    socia_nmt_msg_t nmt_msg );

/// store a uint8_t into the socia_sdo_msg_t
void socia_put_uint8(socia_sdo_msg_t *s, uint8_t d);

/// read a uint8_t in the socia_sdo_msg_t
void socia_put_uint8(socia_sdo_msg_t *s, uint8_t d);

/// read a uint8_t in the socia_sdo_msg_t
uint8_t socia_get_uint8(socia_sdo_msg_t *s );

/// store a uint16_t into the socia_sdo_msg_t
void socia_put_uint16(socia_sdo_msg_t *s, uint16_t d);

/// read a uint16_t in the socia_sdo_msg_t
uint16_t socia_get_uint16(socia_sdo_msg_t *s);

/// store a int16_t into the socia_sdo_msg_t
void socia_put_int16(socia_sdo_msg_t *s, int16_t d);

/// read a int16_t in the socia_sdo_msg_t
int16_t socia_get_int16(socia_sdo_msg_t *s);

/// store a uint32_t into the socia_sdo_msg_t
void socia_put_uint32(socia_sdo_msg_t *s, uint32_t d);

/// read a uint32_t in the socia_sdo_msg_t
uint32_t socia_get_uint32(socia_sdo_msg_t *s);

/// store a int32_t into the socia_sdo_msg_t
void socia_put_int32(socia_sdo_msg_t *s, int32_t d);

/// read a int32_t in the socia_sdo_msg_t
int32_t socia_get_int32(socia_sdo_msg_t *s);

/// translates an socia_sdo_msg_t struct to a CMSG struct
void socia_build_can( struct can_frame *dst, const socia_sdo_msg_t *src, const int is_response );

/// translates a CMSG struct to a socia_sdo_msg_t struct
void socia_build_sdo( socia_sdo_msg_t *dst, const struct can_frame *src, const int is_request );

typedef enum socia_command_spec {
    SOCIA_SEG_DL = 0, //segment download
    SOCIA_EX_DL = 1,  //expedited download (client->server)
    SOCIA_EX_UL = 2,  //expedited upload (server->client)
    SOCIA_SEG_UL = 3, //segment upload
    SOCIA_ABORT = 4
} socia_command_spec_t;


uint8_t socia_command( const socia_command_spec_t command_spec,
		       const uint8_t nodata_len,
		       const uint8_t is_expedited, const uint8_t is_len_in_cmd );


int socia_sdo_query( const int h, const socia_sdo_msg_t *req,
		     socia_sdo_msg_t *resp );


#ifdef __cplusplus
}
#endif

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */

#endif //SOCIA_CANOPEN_H
