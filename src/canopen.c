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

/**
 * \file ntcanopen.c
 *
 * \brief CANopen implementation using esd's NTCAN API
 *
 * \author Neil Dantam
 * \author Can Erdogan (bug fixes)
 *
 * \bug AMC servo drives require expedited SDO reads to have a full
 * 8-byte data portion.  We must accomodate.
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

#include "socia.h"

void socia_build_can( struct can_frame *dst, const socia_sdo_msg_t *src, const int is_response ) {
    // FIXME: better message validation
    assert( src->length <= 4 );

    // Set the message ID and the length
    dst->can_id = (canid_t)(src->node + (is_response ? SOCIA_SDO_RESP_BASE : SOCIA_SDO_REQ_BASE));
    dst->can_dlc = (uint8_t)(src->length + 4);   // 4 due to "len" (1), "msg_lost" (1) and "reserved" (2) fields

    // Set the data
    dst->data[0] = src->command;
    dst->data[1] = (uint8_t)(src->index & 0xFF);
    dst->data[2] = (uint8_t)((src->index >> 8) & 0xFF);
    dst->data[3] = src->subindex;
    memcpy( &(dst->data[4]), &(src->data[0]), src->length );

    // Reset the rest of the data that is not filled
    memset( dst->data + 4 + src->length, 0, 4 - src->length );
}

// NOTE: Does this translation stand correct if the dst message does not
// use the entire data?
void socia_build_sdo( socia_sdo_msg_t *dst, const struct can_frame *src, const int is_request ) {
    // FIXME: better message validation
    assert( src->can_dlc <= 8 );
    dst->node = (uint8_t)(src->can_id - (is_request ? 0x600 : 0x580));
    dst->length = (uint8_t)(src->can_dlc - 4);
    dst->command = (uint8_t)src->data[0];
    dst->index = (uint16_t)(src->data[1] + (src->data[2] << 8));
    dst->subindex = src->data[3];
    memcpy( &(dst->data[0]), &(src->data[4]), dst->length );
    memset( &(dst->data[ dst->length ]), 0, 4 - dst->length );
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
