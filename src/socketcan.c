
/* Copyright (c) 2013, Georgia Tech Research Corporation
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


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "socanmatic.h"
#include "socanmatic_private.h"

_Bool canmat_can_ok( ssize_t r ) {
    return r >= 0;
}

ssize_t canmat_can_send( int fd, const struct can_frame *f ) {
    ssize_t bytes = 0;
    do {
        ssize_t r = write( fd, (uint8_t*)f + bytes, (sizeof(*f) - (size_t)bytes) );
        if( r < 0 ) {
            if( EINTR == errno ) {
                continue;
            } else {
                return r;
            }
        } else {
            bytes += r;
        }
    } while( bytes < (ssize_t)sizeof(*f) );

    return bytes;
}

ssize_t canmat_can_recv( int fd, struct can_frame *f ) {
    ssize_t bytes = 0;
    do {
        ssize_t r = read( fd, (uint8_t*)f + bytes, (sizeof(*f) - (size_t)bytes) );
        if( r < 0 ) {
            if( EINTR == errno ) {
                continue;
            } else {
                return r;
            }
        } else {
            bytes += r;
        }
    } while( bytes < (ssize_t)sizeof(*f) );

    return bytes;
}

int canmat_can_open ( const char *name ) {
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if( s < 0 ) return s;

    struct sockaddr_can addr;
    struct ifreq ifr;

    addr.can_family = AF_CAN;
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    int r = ioctl(s, SIOCGIFINDEX, &ifr);
    if( r ) return -10;

    addr.can_ifindex = ifr.ifr_ifindex;
    r = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if( r ) return -20;

    return s;
}


/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
