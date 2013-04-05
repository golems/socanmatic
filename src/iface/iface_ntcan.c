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
static canmat_status_t v_print_info( struct canmat_iface *cif, FILE *fptr );

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
    .set_kbps=v_set_kbps,
    .print_info=v_print_info
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

    cif->fd = -1;
    canmat_iface_ntcan_t *ntcif = (canmat_iface_ntcan_t*)cif;

    // parse name
    errno = 0;
    char *endptr = NULL;
    int32_t net = (int32_t)strtol(name, &endptr, 0);
    if( 0 != errno || endptr == name || !endptr || '\0' != endptr[0] ) return CANMAT_ERR_PARAM;

    // open can handle
    {
        NTCAN_RESULT r  = canOpen( net,           //net
                                   0,             //flags
                                   256,           //txqueue
                                   256,           //rxqueue
                                   0,             //txtimeout, forever
                                   0,             //rxtimeout, forever
                                   &(ntcif->handle)  // handle
            );
        if( NTCAN_SUCCESS != r ) {
            cif->err = r;
            return CANMAT_ERR_OS;
        }
    }

    // bind ids
    for( int i = 0; i <= CANMAT_COB_ID_MAX_BASE; i ++ ) {
        NTCAN_RESULT r  = canIdAdd( ntcif->handle, i );
        if( NTCAN_SUCCESS != r ) {
            cif->err = r;
            canClose( ntcif->handle ); // try to close
            return CANMAT_ERR_OS;
        }
    }

    return CANMAT_OK;
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
    case NTCAN_SUCCESS: return "NTCAN_SUCCESS";
    case NTCAN_RX_TIMEOUT: return "NTCAN_RX_TIMEOUT";
    case NTCAN_TX_TIMEOUT: return "NTCAN_TX_TIMEOUT";
    case NTCAN_TX_ERROR: return "NTCAN_TX_ERROR";
    case NTCAN_CONTR_OFF_BUS: return "NTCAN_CONTR_OFF_BUS";
    case NTCAN_CONTR_BUSY: return "NTCAN_CONTR_BUSY";
    case NTCAN_CONTR_WARN: return "NTCAN_CONTR_WARN";
    case NTCAN_NO_ID_ENABLED: return "NTCAN__NO_ID_ENABLED";
    case NTCAN_ID_ALREADY_ENABLED: return "NTCAN_ID_ALREADY_ENABLED";
    case NTCAN_INVALID_FIRMWARE: return "NTCAN_INVALID_FIRMWARE";
    case NTCAN_MESSAGE_LOST: return "NTCAN_MESSAGE_LOST";
    case NTCAN_INVALID_HARDWARE: return "NTCAN_INVALID_HARDWARE";
    case NTCAN_PENDING_WRITE: return "NTCAN_PENDING_WRITE";
    case NTCAN_PENDING_READ: return "NTCAN_PENDING_READ";
    case NTCAN_INVALID_DRIVER: return "NTCAN_INVALID_DRIVER";
    case NTCAN_SOCK_CONN_TIMEOUT: return "NTCAN_SOCK_CONN_TIMEOUT";
    case NTCAN_SOCK_CMD_TIMEOUT: return "NTCAN_SOCK_CMD_TIMEOUT";
    case NTCAN_SOCK_HOST_NOT_FOUND: return "NTCAN_SOCK_HOST_NOT_FOUND";
    case NTCAN_INVALID_PARAMETER: return "NTCAN_INVALID_PARAMETER";
    case NTCAN_INVALID_HANDLE: return "NTCAN_INVALID_HANDLE";
    case NTCAN_NET_NOT_FOUND: return "NTCAN_NET_NOT_FOUND";
    case NTCAN_INSUFFICIENT_RESOURCES: return "NTCAN_INSUFFICIENT_RESOURCES";
    case NTCAN_OPERATION_ABORTED: return "NTCAN_OPERATION_ABORTED";
    case NTCAN_WRONG_DEVICE_STATE: return "NTCAN_WRONG_DEVICE_STATE";
    case NTCAN_HANDLE_FORCED_CLOSE: return "NTCAN_HANDLE_FORCED_CLOSE";
    case NTCAN_NOT_IMPLEMENTED: return "NTCAN_NOT_IMPLEMENTED";
    case NTCAN_NOT_SUPPORTED: return "NTCAN_NOT_SUPPORTED";
    case NTCAN_CONTR_ERR_PASSIVE: return "NTCAN_CONTR_ERR_PASSIVE";
    default: return "?";
    }
}

static canmat_status_t v_set_kbps( struct canmat_iface *cif, unsigned kbps ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;
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


static void print_version(FILE *f, uint16_t u){
    int rev = u & 0xff;
    int min = (u >> 8) & 0xf;
    int maj = (u >> 12) & 0xf;
    fprintf(f, "%d.%d.%d", maj, min, rev);
}


static int bitset( uint16_t u, int bit ) {
    return (u >> bit) & 1;
}
static void try_feature( FILE *f, CAN_IF_STATUS *status, int bit, const char *name){
    if( bitset( status->features, bit ) )
        fprintf(f, " %s", name );
}


/// convert NTCAN baud code to baud in kbps
static int ntcan_baud_kbps( uint32_t ntcan_baud ) {
    switch(ntcan_baud) {
    case NTCAN_BAUD_1000:
        return 1000;
    case NTCAN_BAUD_800:
        return 800;
    case NTCAN_BAUD_500:
        return 500;
    case NTCAN_BAUD_250:
        return 250;
    case NTCAN_BAUD_125:
        return 125;
    case NTCAN_BAUD_100:
        return 100;
    case NTCAN_BAUD_50:
        return 50;
    case NTCAN_BAUD_10:
        return 10;
    default:
        return -1;
    }
}

static canmat_status_t v_print_info( struct canmat_iface *cif, FILE *fptr ) {
    if( cif->vtable != &vtable ) return CANMAT_ERR_PARAM;

    CAN_IF_STATUS status = {0};
    cif->err =  canStatus(((canmat_iface_ntcan_t*)cif)->handle, &status);
    if( NTCAN_SUCCESS != cif->err ) return CANMAT_ERR_OS;

    uint32_t baud;
    cif->err =  canGetBaudrate(((canmat_iface_ntcan_t*)cif)->handle, &baud);
    if( NTCAN_SUCCESS != cif->err ) return CANMAT_ERR_OS;


    fprintf( fptr,
             " ESD CAN STATUS\n"
             "---------------\n");

    fprintf( fptr, "status:   %u (%s)\n", status.boardstatus,
            (0==status.boardstatus)?"OK":"ERROR" );
    fprintf( fptr, "Baudrate: %d kbps\n", ntcan_baud_kbps( baud ) );
    fprintf( fptr, "id:       %s", status.boardid );
    fprintf( fptr, "\nhardware: "); print_version(fptr,status.hardware);
    fprintf( fptr, "\nfirmware: "); print_version(fptr,status.firmware);
    fprintf( fptr, "\ndriver:   "); print_version(fptr,status.driver);
    fprintf( fptr, "\ndll:      "); print_version(fptr,status.dll);
    fprintf( fptr, "\nfeatures: 0x%x -", status.features);
    try_feature(fptr, &status, 0, "FULL_CAN");
    try_feature(fptr, &status, 1, "CAN_20B");
    try_feature(fptr, &status, 2, "DEVICE_NET");
    try_feature(fptr, &status, 3, "CYCLIC_TX");
    try_feature(fptr, &status, 4, "RX_OBJECT_MODE");
    try_feature(fptr, &status, 5, "TIMESTAMP");
    try_feature(fptr, &status, 6, "LISTEN_ONLY_MODE");
    try_feature(fptr, &status, 7, "SMART_DISCONNECT");
    fprintf(fptr, "\n\n");

    return CANMAT_OK;
}


/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
