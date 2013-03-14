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

#ifndef SOCIA_SDO_H
#define SOCIA_SDO_H

#define SOCIA_SDO_REQ_BASE  (0x600)
#define SOCIA_SDO_RESP_BASE (0x580)

#define SOCIA_SDO_REQ_ID(node)  ((canid_t)((node) + SOCIA_SDO_REQ_BASE))
#define SOCIA_SDO_RESP_ID(node) ((canid_t)((node) + SOCIA_SDO_RESP_BASE))

typedef enum socia_command_spec {
    SOCIA_SEG_DL = 0, ///< segment download
    SOCIA_EX_DL = 1,  ///< expedited download (client->server)
    SOCIA_EX_UL = 2,  ///< expedited upload (server->client)
    SOCIA_SEG_UL = 3, ///< segment upload
    SOCIA_ABORT = 4
} socia_command_spec_t;


typedef enum socia_command_byte {
    SOCIA_SDO_CMD_DL1 = 0x2f,
    SOCIA_SDO_CMD_DL2 = 0x2b,
    SOCIA_SDO_CMD_DL3 = 0x27,
    SOCIA_SDO_CMD_DL4 = 0x23
} socia_command_byte_t;

/** Container struct for SDO requests.
 */
typedef struct socia_sdo_msg {
    // Message contents
    uint16_t index;    ///< CANopen index
    uint8_t command;   ///< CANopen command word, with ccs, reserved, n, e and s fields (see the website)
    uint8_t subindex;  ///< CANopen subindex

    // Additional information
    uint8_t node;      ///< CANopen Node ID
    uint8_t length;    ///< CANopen length of data, is either set in n (command) or in the data (depends on s)

    union {
        uint8_t data[4];   ///< CANopen data section
        uint32_t value;    ///< CANopen data as unsigned integer
    };
} socia_sdo_msg_t;


/* Set the sdo command byte */
void socia_sdo_set_cmd( socia_sdo_msg_t *sdo,
                        socia_command_spec_t command_spec,
                        uint8_t nodata_len,
                        uint8_t is_expedited,
                        uint8_t is_len_in_cmd );



#define SOCIA_SDO_GEN_SET_DATA( CTYPE, BITS, C )                        \
    static inline void                                                  \
    socia_sdo_set_data_ ## C ## BITS ( socia_sdo_msg_t *sdo, CTYPE x )  \
    {                                                                   \
        union socia_byte ## BITS u;                                     \
        u.C = x;                                                        \
        sdo->length = sizeof(x);                                        \
        socia_byte_stle ## BITS(sdo->data, u.u);                        \
    }
SOCIA_SDO_GEN_SET_DATA( uint8_t, 8, u );
SOCIA_SDO_GEN_SET_DATA( int8_t,  8, i );
SOCIA_SDO_GEN_SET_DATA( uint16_t, 16, u );
SOCIA_SDO_GEN_SET_DATA( int16_t,  16, i );
SOCIA_SDO_GEN_SET_DATA( uint32_t, 32, u );
SOCIA_SDO_GEN_SET_DATA( int32_t,  32, i );
SOCIA_SDO_GEN_SET_DATA( float,    32, f );

#define SOCIA_SDO_GEN_GET_DATA( CTYPE, BITS, C )                        \
    static inline CTYPE                                                 \
    socia_sdo_get_data_ ## C ## BITS ( socia_sdo_msg_t *sdo )           \
    {                                                                   \
        union socia_byte ## BITS u;                                     \
        u.u = socia_byte_ldle ## BITS(sdo->data );                      \
        return u.C;                                                     \
    }
SOCIA_SDO_GEN_GET_DATA( uint8_t, 8, u );
SOCIA_SDO_GEN_GET_DATA( int8_t,  8, i );
SOCIA_SDO_GEN_GET_DATA( uint16_t, 16, u );
SOCIA_SDO_GEN_GET_DATA( int16_t,  16, i );
SOCIA_SDO_GEN_GET_DATA( uint32_t, 32, u );
SOCIA_SDO_GEN_GET_DATA( int32_t,  32, i );
SOCIA_SDO_GEN_GET_DATA( float,    32, f );


/** Set fields of sdo for an expedited download.
 *
 * @precondition: sdo->data and sdo->length are set
 */
void socia_sdo_set_ex_dl( socia_sdo_msg_t *sdo,
                          uint8_t node, uint16_t index, uint8_t subindex );


#endif //SOCIA_H
