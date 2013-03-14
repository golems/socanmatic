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

#ifndef SOCIA_BYTEORDER_H
#define SOCIA_BYTEORDER_H

#ifdef __cplusplus
extern "C" {
#endif


typedef union socia_byte8  {
    int8_t i;
    uint8_t u;
} socia_byte8_t;

typedef union socia_byte16  {
    int16_t i;
    uint16_t u;
} socia_byte16_t;

typedef union socia_byte32  {
    int32_t i;
    uint32_t u;
    float f;
} socia_byte32_t;

typedef union socia_byte64  {
    int64_t i;
    uint64_t u;
    double f;
} socia_byte64_t;


static inline void socia_byte_stle32( void *p, uint32_t u) {
    uint8_t *q = (uint8_t*)p;
    q[0] = (uint8_t)(u & 0xFF);
    q[1] = (uint8_t)((u >>  8) & 0xFF);
    q[2] = (uint8_t)((u >> 16) & 0xFF);
    q[3] = (uint8_t)((u >> 24) & 0xFF);
}

static inline void socia_byte_stle16( void *p, uint16_t u) {
    uint8_t *q = (uint8_t*)p;
    q[0] = (uint8_t)(u & 0xFF);
    q[1] = (uint8_t)((u >>  8) & 0xFF);
}

static inline uint32_t socia_byte_ldle32( void *p ) {
    uint8_t *q = (uint8_t*)p;
    return  (uint32_t)( (uint32_t)(q[0])        |
                        (uint32_t)(q[1] << 8)   |
                        (uint32_t)(q[2] << 16)  |
                        (uint32_t)(q[3] << 24) );
}

static inline uint16_t socia_byte_ldle16( void *p ) {
    uint8_t *q = (uint8_t*)p;
    return (uint16_t)( (uint16_t)(q[0])       |
                       (uint16_t)(q[1] << 8) );
}

#ifdef __cplusplus
extern "C" {
#endif

static inline uint8_t socia_byte_ldle8( void *p ) {
    return ((uint8_t*)p)[0];
}

static inline void socia_byte_stle8( void *p, uint8_t u) {
    ((uint8_t*)p)[0] = u;
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

#endif //SOCIA_BYTEORDER_H
