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

#include <dlfcn.h>

#include "socanmatic.h"
#include "socanmatic_private.h"


struct fun_desc {
    const char *type;
    canmat_iface_new_fun *fun;
    struct fun_desc *next;
};

static struct fun_desc *desc = NULL;

static struct fun_desc *lookup( const char *type ) {
    for( struct fun_desc *d = desc; NULL != d;  d = d->next ) {
        if( 0 == strcmp(type, d->type) ) return d;
    }
    return NULL;
}

/**
 * @function load
 * @brief
 */
static void load( const char *type ) {
    const char prefix[] = "libsocanmatic_iface_";
    const char suffix[] = ".so";
    char buf[strlen(prefix) + strlen(suffix) + strlen(type) + 1];

    strcpy(buf,prefix);
    strcat(buf,type);
    strcat(buf,suffix);

    void *p = dlopen(buf, RTLD_NOW);

    if( !p ) return;

    canmat_iface_new_fun *fun;
    fun = (canmat_iface_new_fun*)dlsym( p, "canmat_iface_new_module" );

    if( ! fun ) return ;

    struct fun_desc *d = (struct fun_desc*)malloc(sizeof(struct fun_desc));
    d->type = strdup(type);
    d->fun = fun;
    d->next = desc;
    desc = d;

    return;
}

/**
 * @function canmat_iface_new
 * @brief Create a new object of iface (SocketCAN/NTCAN)
 */
canmat_iface_t* canmat_iface_new( const char *type ) {
    struct fun_desc *d = lookup(type);
    if( ! d ) {
        load(type);
        d = lookup(type);
    }

    if( d ) {
        return d->fun();
    } else {
        return NULL;
    }
}

/**
 * @function canmat_iface_strerror
 * @brief Return string of error
 */
const char *canmat_iface_strerror( canmat_iface_t *cif, canmat_status_t status ) {
    if( CANMAT_ERR_OS == status ) {
        return cif->vtable->strerror(cif);
    } else {
        return canmat_strerror(status);
    }
}


/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
