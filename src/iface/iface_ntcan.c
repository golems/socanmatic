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

#include <ntcan.h>
#include <assert.h>

#include <stdlib.h>

#include "socanmatic.h"
#include "socanmatic_private.h"



static canmat_status_t v_open( struct canmat_iface *cif, const char *name );
static canmat_status_t v_send( struct canmat_iface *cif, const struct can_frame *frame );
static canmat_status_t v_recv( struct canmat_iface *cif, struct can_frame *frame );
static canmat_status_t v_destroy( struct canmat_iface *cif );
static const char *v_strerror( struct canmat_iface *cif );
static canmat_status_t v_set_kbps( struct canmat_iface *cif, unsigned kbps );

typedef struct canmat_iface_ntcan {
    struct canmat_iface cif;
    NTCAN_HANDLE handle;
} canmat_iface_ntcan_t;

static struct canmat_iface_vtable vtable = {
    .open=v_open,
    .send=v_send,
    .recv=v_recv,
    .destroy=v_destroy,
    .strerror=v_strerror,
    .set_kbps=v_set_kbps
};

struct canmat_iface_ntcan *canmat_iface_ntcan_new() {
    canmat_iface_ntcan_t *cif = (canmat_iface_ntcan_t*)malloc(sizeof(canmat_iface_ntcan_t));
    cif->cif.vtable = &vtable;
    return cif;
}

canmat_iface_t* canmat_iface_new_module( ) {
    canmat_iface_ntcan_t *cif = (canmat_iface_ntcan_t*)malloc(sizeof(canmat_iface_ntcan_t));
    cif->cif.vtable = &vtable;
    return (canmat_iface_t*) cif;
}

static canmat_status_t check( struct canmat_iface *cif, NTCAN_RESULT r ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;
    if( NTCAN_SUCCESS == r ) {
        return CANMAT_OK;
    } else {
        cif->err = r;
        return CANMAT_ERR_OS;
    }
}

static canmat_status_t v_open( struct canmat_iface *cif, const char *name ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;

    canmat_iface_ntcan_t *ntcif = (canmat_iface_ntcan_t*)cif;
    // parse name
    errno = 0;
    int32_t net = (int32_t)strtol(name, NULL, 0);
    if( 0 != errno ) return CANMAT_ERR_PARAM;

    // open can handle
    NTCAN_RESULT r  = canOpen( net,           //net
                               0,             //flags
                               10,            //txqueue
                               100,           //rxqueue
                               1000,          //txtimeout
                               1000,          //rxtimeout
                               &(ntcif->handle)  // handle
        );

    //TODO: bind ids
    return check( cif, r );
}

static canmat_status_t v_send( struct canmat_iface *cif, const struct can_frame *frame ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;

    CMSG msg;
    msg.id = (int32_t)frame->can_id;
    msg.len = frame->can_dlc;
    memcpy(msg.data, frame->data, msg.len);

    int num = 1;
    NTCAN_RESULT r  = canWrite( ((canmat_iface_ntcan_t*)cif)->handle, &msg, &num, NULL );

    return check( cif, r );
}

static canmat_status_t v_recv( struct canmat_iface *cif, struct can_frame *frame ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;

    CMSG msg;
    int num = 1;
    NTCAN_RESULT r = canRead( ((canmat_iface_ntcan_t*)cif)->handle, &msg, &num, NULL );
    if( NTCAN_SUCCESS != r ) {
        cif->err = r;
        return CANMAT_ERR_OS;
    }

    frame->can_id = (canid_t)msg.id;
    frame->can_dlc = msg.len;
    memcpy(frame->data, msg.data, frame->can_dlc);

    return CANMAT_OK;
}

static canmat_status_t v_destroy( struct canmat_iface *cif ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;
    NTCAN_RESULT r = canClose( ((canmat_iface_ntcan_t*)cif)->handle );
    return check(cif,r);
}

static const char *v_strerror( struct canmat_iface *cif ) {
    if( cif->vtable != &vtable ) return "?";
    switch( cif->err ) {
    case NTCAN_SUCCESS: return "SUCCESS";
    case NTCAN_RX_TIMEOUT: return "RX_TIMEOUT";
    case NTCAN_TX_TIMEOUT: return "TX_TIMEOUT";
    case NTCAN_TX_ERROR: return "TX_ERROR";
    case NTCAN_CONTR_OFF_BUS: return "CONTR_OFF_BUS";
    case NTCAN_CONTR_BUSY: return "CONTR_BUSY";
    case NTCAN_CONTR_WARN: return "CONTR_WARN";
    case NTCAN_NO_ID_ENABLED: return "_NO_ID_ENABLED";
    case NTCAN_ID_ALREADY_ENABLED: return "ID_ALREADY_ENABLED";
    case NTCAN_INVALID_FIRMWARE: return "INVALID_FIRMWARE";
    case NTCAN_MESSAGE_LOST: return "MESSAGE_LOST";
    case NTCAN_INVALID_HARDWARE: return "INVALID_HARDWARE";
    case NTCAN_PENDING_WRITE: return "PENDING_WRITE";
    case NTCAN_PENDING_READ: return "PENDING_READ";
    case NTCAN_INVALID_DRIVER: return "INVALID_DRIVER";
    case NTCAN_SOCK_CONN_TIMEOUT: return "SOCK_CONN_TIMEOUT";
    case NTCAN_SOCK_CMD_TIMEOUT: return "SOCK_CMD_TIMEOUT";
    case NTCAN_SOCK_HOST_NOT_FOUND: return "SOCK_HOST_NOT_FOUND";
    case NTCAN_INVALID_PARAMETER: return "INVALID_PARAMETER";
    case NTCAN_INVALID_HANDLE: return "INVALID_HANDLE";
    case NTCAN_NET_NOT_FOUND: return "NET_NOT_FOUND";
    case NTCAN_INSUFFICIENT_RESOURCES: return "INSUFFICIENT_RESOURCES";
    case NTCAN_OPERATION_ABORTED: return "OPERATION_ABORTED";
    case NTCAN_WRONG_DEVICE_STATE: return "WRONG_DEVICE_STATE";
    case NTCAN_HANDLE_FORCED_CLOSE: return "HANDLE_FORCED_CLOSE";
    case NTCAN_NOT_IMPLEMENTED: return "NOT_IMPLEMENTED";
    case NTCAN_NOT_SUPPORTED: return "NOT_SUPPORTED";
    case NTCAN_CONTR_ERR_PASSIVE: return "CONTR_ERR_PASSIVE";
    default: return "?";
    }
}

static canmat_status_t v_set_kbps( struct canmat_iface *cif, unsigned kbps ) {
    uint32_t baudcode;
    switch(kbps) {
    case 1000: baudcode = NTCAN_BAUD_1000;  break;
    case 800:  baudcode = NTCAN_BAUD_800;   break;
    case 500:  baudcode = NTCAN_BAUD_500;   break;
    case 250:  baudcode = NTCAN_BAUD_250;   break;
    case 125:  baudcode = NTCAN_BAUD_125;   break;
    case 100:  baudcode = NTCAN_BAUD_100;   break;
    case 50:   baudcode = NTCAN_BAUD_50;    break;
    case 20:   baudcode = NTCAN_BAUD_20;    break;
    case 10:   baudcode = NTCAN_BAUD_10;    break;
    default: return CANMAT_ERR_PARAM;
    }

    NTCAN_RESULT r = canSetBaudrate( ((canmat_iface_ntcan_t*)cif)->handle, baudcode );

    return check(cif,r);
}


/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
