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

#ifndef SOCANMATIC_IFACE_H
#define SOCANMATIC_IFACE_H


#ifdef __cplusplus
extern "C" {
#endif

struct canmat_iface;

struct canmat_iface_vtable {
    canmat_status_t (*open)( struct canmat_iface *cif, const char *name );
    canmat_status_t (*send)( struct canmat_iface *cif, const struct can_frame *frame );
    canmat_status_t (*recv)( struct canmat_iface *cif, struct can_frame *frame );
    canmat_status_t (*destroy)( struct canmat_iface *cif );
    canmat_status_t (*set_kbps)( struct canmat_iface *cif, unsigned kbps );
    canmat_status_t (*print_info)( struct canmat_iface *cif, FILE *fptr );
    const char *(*strerror)( struct canmat_iface *cif );
};

typedef struct canmat_iface {
    struct canmat_iface_vtable *vtable;
    int fd;
    int err;
} canmat_iface_t;

typedef canmat_iface_t* canmat_iface_new_fun( void );

canmat_iface_t* canmat_iface_new( const char *type );
canmat_iface_t* canmat_iface_new_module( void );

canmat_iface_t * canmat_iface_new_socketcan( void );

#ifdef _ntcan_h
struct canmat_iface *canmat_iface_new_ntcan( void );
#endif //_ntcan_h


static inline canmat_status_t canmat_iface_open( struct canmat_iface *cif, const char *name ) {
    return cif->vtable->open(cif,name);
}
static inline canmat_status_t canmat_iface_send( struct canmat_iface *cif, const struct can_frame *frame ) {
    return cif->vtable->send(cif,frame);
}
static inline canmat_status_t canmat_iface_recv( struct canmat_iface *cif, struct can_frame *frame ) {
    return cif->vtable->recv(cif,frame);
}
static inline canmat_status_t canmat_iface_destroy( struct canmat_iface *cif ) {
    return cif->vtable->destroy(cif);
}
static inline canmat_status_t canmat_iface_print_info( struct canmat_iface *cif, FILE *fptr ) {
    return cif->vtable->print_info(cif, fptr);
}

const char *canmat_iface_strerror( struct canmat_iface *cif, canmat_status_t status );

#ifdef __cplusplus
}
#endif

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */

#endif //SOCANMATIC_IFACE_H
