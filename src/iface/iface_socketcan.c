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



static canmat_status_t v_open( struct canmat_iface *cif, const char *name );
static canmat_status_t v_send( struct canmat_iface *cif, const struct can_frame *frame );
static canmat_status_t v_recv( struct canmat_iface *cif, struct can_frame *frame );
static canmat_status_t v_destroy( struct canmat_iface *cif );
static canmat_status_t v_filter( struct canmat_iface *cif, const struct can_filter *filters, size_t n );
static const char *v_strerror( struct canmat_iface *cif );
static canmat_status_t v_set_kbps( struct canmat_iface *cif, unsigned kbps );
static canmat_status_t v_print_info( struct canmat_iface *cif, FILE *fptr );

static struct canmat_iface_vtable vtable = {
    .open=v_open,
    .send=v_send,
    .recv=v_recv,
    .destroy=v_destroy,
    .filter=v_filter,
    .strerror=v_strerror,
    .set_kbps=v_set_kbps,
    .print_info=v_print_info
};


canmat_iface_t * canmat_iface_new_socketcan( void ) {
    canmat_iface_t *cif = (canmat_iface_t*)malloc(sizeof(canmat_iface_t));
    cif->vtable = &vtable;
    return cif;
}

canmat_iface_t* canmat_iface_new_module( void ) {
    canmat_iface_t *cif = (canmat_iface_t*)malloc(sizeof(canmat_iface_t));
    cif->vtable = &vtable;
    return cif;
}

static inline canmat_status_t set_err( struct canmat_iface *cif ) {
    cif->err = errno;
    return CANMAT_ERR_OS;;
}

static canmat_status_t v_send( struct canmat_iface *cif, const struct can_frame *frame ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;
    ssize_t bytes = 0;
    do {
        ssize_t r = write( cif->fd, (uint8_t*)frame + bytes, (sizeof(*frame) - (size_t)bytes) );
        if( r < 0 ) {
            if( EINTR == errno ) {
                continue;
            } else {
                return set_err(cif);
            }
        } else {
            bytes += r;
        }
    } while( bytes < (ssize_t)sizeof(*frame) );
    return CANMAT_OK;
}

static canmat_status_t v_recv( struct canmat_iface *cif, struct can_frame *frame ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;
    ssize_t bytes = 0;
    do {
        ssize_t r = read( cif->fd, (uint8_t*)frame + bytes, (sizeof(*frame) - (size_t)bytes) );
        if( r < 0 ) {
            if( EINTR == errno ) {
                continue;
            } else {
                return set_err(cif);
            }
        } else {
            bytes += r;
        }
    } while( bytes < (ssize_t)sizeof(*frame) );
    return CANMAT_OK;
}

static canmat_status_t v_destroy( struct canmat_iface *cif ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;
    if( close(cif->fd) ) {
        return set_err(cif);
    }
    return CANMAT_OK;
}

static const char *v_strerror( struct canmat_iface *cif ) {
    if( cif->vtable != &vtable ) return "?";
    return strerror(cif->err);
}

static canmat_status_t v_open( struct canmat_iface *cif, const char *name ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if( s < 0 ) {
        return set_err(cif);
    }

    struct sockaddr_can addr = {0};
    struct ifreq ifr;

    addr.can_family = AF_CAN;
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    int r = ioctl(s, SIOCGIFINDEX, &ifr);
    if( r ) return set_err(cif);

    addr.can_ifindex = ifr.ifr_ifindex;
    r = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if( r ) return set_err(cif);

    cif->fd = s;
    cif->err = 0;

    return CANMAT_OK;
}

static canmat_status_t v_set_kbps( struct canmat_iface *cif, unsigned kbps ) {
    (void)cif; (void)kbps;
    // TODO
    return CANMAT_ERR_NOT_SUP;
}

static canmat_status_t v_print_info( struct canmat_iface *cif, FILE *fptr ) {
    (void)cif; (void)fptr;
    return CANMAT_ERR_NOT_SUP;
}

static canmat_status_t v_filter( struct canmat_iface *cif, const struct can_filter *filters, size_t n ) {
    int i = setsockopt( cif->fd, SOL_CAN_RAW, CAN_RAW_FILTER,
                        filters, (socklen_t)(n*sizeof(filters[0])) );
    return i ? CANMAT_ERR_OS : CANMAT_OK;

}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
