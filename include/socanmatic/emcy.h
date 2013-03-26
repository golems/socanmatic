/* Copyright (c) 2008-2013, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Neil T. Dantam <ntd@gatech.edu>
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

#ifndef SOCANMATIC_EMCY_H
#define SOCANMATIC_EMCY_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum canmat_emcy_class {
    CANMAT_EMCY_CODE_CLASS_NO_ERROR      = 0x0000,
    CANMAT_EMCY_CODE_CLASS_GENERIC       = 0x1000,
    CANMAT_EMCY_CODE_CURRENT             = 0x2000,
    CANMAT_EMCY_CODE_CURRENT_INPUT       = 0x2100,
    CANMAT_EMCY_CODE_CURRENT_INSIDE      = 0x2200,
    CANMAT_EMCY_CODE_CURRENT_OUTPUT      = 0x2300,
    CANMAT_EMCY_CODE_VOLTAGE             = 0x3000,
    CANMAT_EMCY_CODE_VOLTAGE_MAINS       = 0x3100,
    CANMAT_EMCY_CODE_VOLTAGE_INSIDE      = 0x3200,
    CANMAT_EMCY_CODE_VOLTAGE_OUTPUT      = 0x3300,
    CANMAT_EMCY_CODE_TEMP                = 0x4000,
    CANMAT_EMCY_CODE_TEMP_AMBIENT        = 0x4100,
    CANMAT_EMCY_CODE_TEMP_DEVICE         = 0x4200,
    CANMAT_EMCY_CODE_HARDWARE            = 0x5000,
    CANMAT_EMCY_CODE_SOFTWARE            = 0x6000,
    CANMAT_EMCY_CODE_SOFTWARE_INTERNAL   = 0x6100,
    CANMAT_EMCY_CODE_SOFTWARE_USER       = 0x6200,
    CANMAT_EMCY_CODE_SOFTWARE_DATA       = 0x6300,
    CANMAT_EMCY_CODE_ADDITIONAL_MODULES  = 0x7000,
    CANMAT_EMCY_CODE_MONITORING          = 0x8000,
    CANMAT_EMCY_CODE_MONITORING_COMM     = 0x8100,
    CANMAT_EMCY_CODE_MONITORING_PROTO    = 0x8200,
    CANMAT_EMCY_CODE_EXTERNAL            = 0x9000,
    CANMAT_EMCY_CODE_ADDITIONAL_FUNC     = 0xF000,
    CANMAT_EMCY_CODE_DEVICE_SPECIFIC     = 0xFF00
} canmat_emcy_class_t;

static inline uint16_t canmat_frame_emcy_get_eec( const struct can_frame *frame ) {
    return canmat_byte_ldle16( frame->data );
}

static inline uint8_t canmat_frame_emcy_get_er( const struct can_frame *frame ) {
    return frame->data[2];
}



#ifdef __cplusplus
}
#endif

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */

#endif //SOCANMATIC_EMCY_H
