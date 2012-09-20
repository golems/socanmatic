/* -*- mode: C; c-basic-offset: 4 -*- */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*
 * Copyright (c) 2008, Georgia Tech Research Corporation
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

/**
 * \file ntcanopen.h
 *
 * \brief CANopen implementation using esd's NTCAN API. See the website
 * http://en.wikipedia.org/wiki/Canopen#Service_Data_Object_.28SDO.29_protocol
 * for details of service data object (SDO) protocol.
 *
 * \author Neil Dantam
 * \author Can Erdogan (bug fixes)
 *
 * \bug Deferring implentation of segmented messages
 *
 * \bug esd's driver will non-deterministically order loopback and
 * off-the-wire CAN messages.
 */

#ifndef NTCANOPEN_H_
#define NTCANOPEN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ntcan.h"

/// The maximum size of the data field in an SDO
#define MAX_SDO_DATA_LENGTH 4

/// The board info for the AMC can. This channel has to be treated differently 
/// because the receiver can not deal with variable size SDO packages. We use
/// the board to check an index (to find out if the message is for the AMC),
/// and manipulate the SDO accordingly.
/// TODO This constant is already defined in amccan.h. A common location
/// should be determined to share these constants.
#define AMCCAN_INDEX_BOARD_INFO             0x20D8

/// container struct for SDO requests
typedef struct {

	// Message contents
    uint8_t command;   ///< CANopen command word, with ccs, reserved, n, e and s fields (see the website)
    uint16_t index;    ///< CANopen index
    uint8_t subindex;  ///< CANopen subindex
    uint8_t data[MAX_SDO_DATA_LENGTH];   ///< CANopen data section
	
	// Additional information
    uint8_t node;      ///< CANopen Node ID
    uint8_t length;    ///< CANopen length of data, is either set in n (command) or in the data (depends on s)
	uint8_t is_amc;
	
 } sdo_msg_t;


#define CANOPEN_SDO_REQ_ID(node) (0x600 + (node))
#define CANOPEN_SDO_RESP_ID(node) (0x580 + (node))

/* /// type of sdo data */
/* typedef enum { */
/*   CANOPEN_UINT16, */
/*   CANOPEN_INT16, */
/*   CANOPEN_UINT32, */
/*   CANOPEN_INT32 */
/* } canopen_datatype_t; */

/* /// sdo message data */
/* typedef union { */
/*   uint16_t u16; */
/*   int16_t  i16; */
/*   uint32_t u32; */
/*   int32_t  i32; */
/* } sdo_msg_data_t; */

/* /// descriptor to sdo messages */
/* typedef struct { */
/*   uint16_t index; */
/*   uint8_t subindex; */
/*   canopen_datatype_t type; */
/*   char *name; */
/* } sdo_desc_t; */

/* /// CANopen dictionary entry */
/* typedef struct { */
/*   sdo_desc_t *desc; */
/*   sdo_msg_data_t *data; */
/*   struct timespec *modtime; */
/*   uint8_t node; */
/* } canopen_dict_entry_t; */

/* /\** CANopen dictionary */
/*     This structure is used to cache CANopen dictionary reads  */
/*  *\/  */
/* typedef struct { */
/*   canopen_dict_entry_t *entries; */
/*   size_t entry_count; */
/* } canopen_dict_t; */

/* /\** Initializes a CANopen dictionary. */

/*     \pre dict points to unitialized dict struct.  entries points to */
/*     array of initialized entry structs. */

/*     \post dict has a pointer to entries.  Entries has been sorted. */

/*     \param entries array of dict_entry's */
/*     \param count size of entries */

/*  *\/ */
/* void canOpenDictInit( canopen_dict_t *dict,  */
/*                       canopen_dict_entry_t *entries, size_t count ); */

/* /\** Reads a value from the dictionary. */

/*     Not thread safe. */

/*     \param data if not NULL, pointer to place to output data */
/*     \param modtime if not NULL, pointer to place to output modtime of data */

/*     \return 0 if entry found, nonzero otherwise. */

/*  *\/ */
/* int canOpenDictRead( canopen_dict_t *dict, */
/*                      sdo_msg_data_t *data, struct timespec *modtime, */
/*                      uint8_t node, uint16_t index, uint8_t subindex ); */

/* /\** Reads a value from the dictionary. */

/*     Not thread safe. */

/*     \return 0 on successful write. Nonzero if dict entry not found. */
/*  *\/ */
/* int canOpenDictWrite( canopen_dict_t *dict, */
/*                       uint8_t node, uint16_t index, uint8_t subindex, */
/*                       sdo_msg_data_t *data, struct timespec *modtime */
/*                       ); */


    typedef enum {
        CANOPEN_NMT_INVAL = -1,
        CANOPEN_NMT_START_REMOTE = 0x1,
        CANOPEN_NMT_STOP_REMOTE = 0x2,
        CANOPEN_NMT_PRE_OP = 0x80,
        CANOPEN_NMT_RESET_NODE = 0x81,
        CANOPEN_NMT_RESET_COM = 0x82
    } canopen_nmt_msg_t;

/// send a CANopen Network Management message
    NTCAN_RESULT canOpenWriteNMT( NTCAN_HANDLE h, uint8_t node,
                                  canopen_nmt_msg_t nmt_msg );


/// store a uint8_t into the sdo_msg_t
    void canOpenPut_uint8(sdo_msg_t *s, uint8_t d);

/// read a uint8_t in the sdo_msg_t
    uint8_t canOpenGet_uint8(sdo_msg_t *s );

/// store a uint16_t into the sdo_msg_t
    void canOpenPut_uint16(sdo_msg_t *s, uint16_t d);

/// read a uint16_t in the sdo_msg_t
    uint16_t canOpenGet_uint16(sdo_msg_t *s);

/// store a int16_t into the sdo_msg_t
    void canOpenPut_int16(sdo_msg_t *s, int16_t d);

/// read a int16_t in the sdo_msg_t
    int16_t canOpenGet_int16(sdo_msg_t *s);

/// store a uint32_t into the sdo_msg_t
    void canOpenPut_uint32(sdo_msg_t *s, uint32_t d);

/// read a uint32_t in the sdo_msg_t
    uint32_t canOpenGet_uint32(sdo_msg_t *s);

/// store a int32_t into the sdo_msg_t
    void canOpenPut_int32(sdo_msg_t *s, int32_t d);

/// read a int32_t in the sdo_msg_t
    int32_t canOpenGet_int32(sdo_msg_t *s);


/// translates an sdo_msg_t struct to a CMSG struct
    void canOpenTranslateSDO( CMSG *dst, const sdo_msg_t *src, const int is_response );

/// translates a CMSG struct to a sdo_msg_t struct
    void canOpenTranslateCAN( sdo_msg_t *dst, const CMSG *src, const int is_request );

/** Adds the COB-ID for SDO reponses from node
    \param node CANopen node id
    \post h will be listening for SDO responses from node
*/
    NTCAN_RESULT canOpenIdAddSDOResponse( NTCAN_HANDLE h, const uint8_t node );


/** Deletes the COB-ID for SDO reponses from node
    \param node CANopen node id
    \post h will NOT be listening for SDO responses from node
*/
    NTCAN_RESULT canOpenIdDeleteSDOResponse( NTCAN_HANDLE h, const uint8_t node );

/// Sends a request to the SDO server
    NTCAN_RESULT canOpenWriteSDO( const NTCAN_HANDLE h, const sdo_msg_t *sdomsg );

/** Waits to receive an SDO message, discarding all other messages received

    \pre h is a valid handle and is set to receive the proper COB-ID
    for sdomsg->node

    \post all messages in h prior to the requested SDO are discarded

    \param sdomsg NULL or pointer to buffer to hold response
*/
    NTCAN_RESULT canOpenWaitSDO( const NTCAN_HANDLE h, sdo_msg_t *sdomsg );


/** Writes the sdo message and waits for a response.

    \pre h is a valid handle and is set to receive the proper COB-ID
    for response to req

    \post all messages in h prior to the requested SDO are discarded

    \param resp NULL or pointer to buffer to hold response
*/
    NTCAN_RESULT canOpenSDOWriteWait( const NTCAN_HANDLE h, const sdo_msg_t *req,
                                      sdo_msg_t *resp );



/** Performs an SDO download of a uint8_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_dl_u8( NTCAN_HANDLE h, uint8_t *rcmd,
                                            uint8_t node,
                                            uint16_t index, uint8_t subindex,
                                            uint8_t value );
    NTCAN_RESULT canOpenSDOWriteWait_dl_u8_AMC( NTCAN_HANDLE h, uint8_t *rcmd,
                                            uint8_t node,
                                            uint16_t index, uint8_t subindex,
                                            uint8_t value, uint8_t is_amc);

/** Performs an SDO download of a uint16_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_dl_u16( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex,
                                             uint16_t value );

    NTCAN_RESULT canOpenSDOWriteWait_dl_u16_AMC( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex,
                                             uint16_t value, uint8_t is_amc);

/** Performs an SDO download of a uint32_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_dl_u32( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex,
                                             uint32_t value );

    NTCAN_RESULT canOpenSDOWriteWait_dl_u32_AMC( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex,
                                             uint32_t value, uint8_t is_amc);

/** Performs an SDO download of a int32_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_dl_i32( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex,
                                             int32_t value);

    NTCAN_RESULT canOpenSDOWriteWait_dl_i32_AMC( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex,
                                             int32_t value, uint8_t is_amc);


/** Performs an SDO upload of a uint8_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_ul_u8( NTCAN_HANDLE h, uint8_t *rcmd,
                                            uint8_t *value,
                                            uint8_t node,
                                            uint16_t index, uint8_t subindex );

/** Performs an SDO upload of a uint16_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_ul_u16( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint16_t *value,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex );


    NTCAN_RESULT canOpenSDOWriteWait_ul_u16_AMC( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint16_t *value,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex, uint8_t is_amc);


/** Performs an SDO upload of a uint32_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_ul_u32( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint32_t *value,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex );

    NTCAN_RESULT canOpenSDOWriteWait_ul_u32_AMC( NTCAN_HANDLE h, uint8_t *rcmd,
                                             uint32_t *value,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex, uint8_t is_amc);

/** Performs an SDO upload of a int32_t and waits for the response.
 */
    NTCAN_RESULT canOpenSDOWriteWait_ul_i32( NTCAN_HANDLE h, uint8_t *rcmd,
                                             int32_t *value,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex );

    NTCAN_RESULT canOpenSDOWriteWait_ul_i32_AMC( NTCAN_HANDLE h, uint8_t *rcmd,
                                             int32_t *value,
                                             uint8_t node,
                                             uint16_t index, uint8_t subindex, uint8_t is_amc);

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

/** returns character str describing the NTCAN_RESULT.

    Returned character points to a constant string. Don't change it.
*/
    const char *canResultString( int ntr );


    int32_t canOpenDecode_int32( uint8_t *p );


#ifdef __cplusplus
}
#endif

#endif
