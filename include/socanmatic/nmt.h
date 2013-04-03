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

#ifndef SOCANMATIC_NMT_H
#define SOCANMATIC_NMT_H

/**
 * \file canmat_canopen.h
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


/// Produce COB-ID for response from node


#ifdef __cplusplus
extern "C" {
#endif


/*********************/
/* NMT Communication */
/*********************/

typedef enum canmat_nmt_msg {
    CANMAT_NMT_START_REMOTE = 0x1,
    CANMAT_NMT_STOP_REMOTE  = 0x2,
    CANMAT_NMT_PRE_OP       = 0x80,
    CANMAT_NMT_RESET_NODE   = 0x81,
    CANMAT_NMT_RESET_COM    = 0x82
} canmat_nmt_msg_t;


/**  Toggle bit used with remote-transmission requests.
 *   Node will toggle the bit on each message.
 */
#define CANMANT_NMT_ERR_TOGGLE_MASK (1 << 8)

typedef enum canmat_nmt_err_msg {
    CANMAT_NMT_ERR_BOOT    = 0x00,
    CANMAT_NMT_ERR_STOPPED = 0x04,
    CANMAT_NMT_ERR_OP      = 0x05,
    CANMAT_NMT_ERR_PRE_OP  = 0x7F
} canmat_nmt_err_msg_t;


/**  Send an NMT Message.
 */
int canmat_send_nmt( canmat_iface_t *cif, uint8_t node,
                     canmat_nmt_msg_t nmt_msg );






#ifdef __cplusplus
}
#endif

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */

#endif //SOCANMATIC_NMT_H
