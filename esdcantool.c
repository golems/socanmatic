/*
 * Copyright (c) 2008, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the Georgia Tech Research Corporation nor
 *       the names of its contributors may be used to endorse or
 *       promote products derived from this software without specific
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEORGIA TECH RESEARCH CORPORATION ''AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GEORGIA
 * TECH RESEARCH CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \file esdcantool.c
 *
 * \brief A shell tool to interact with esd can cards.
 *
 * \author Neil Dantam
 *
 * \bug Only works on gnu/linux.  This may be a feature.
 *
 * \bug Seems to break under the PREEMPT kernel.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>
#include "ntcan.h"

// verbosity output levels
#define WARN 0
#define INFO 1
#define DEBUG 2

#define DEFAULT_BAUD 1000

#define DEBUGF(level, fmt, a... )                                       \
    (((args.verbosity) >= level )?fprintf( stderr, (fmt), ## a ) : 0);

// ARG_KEYS
#define ARG_READ_SDO 300
#define ARG_CAN_STATUS 301
#define ARG_LISTEN 302
#define ARG_WRITE 303
#define ARG_BAUD 304
#define ARG_NET 305

/* -- ARG PARSING JUNK --*/
static struct argp_option options[] = {
    {
        .name = "verbose",
        .key = 'v',
        .arg = NULL,
        .flags = 0,
        .doc = "Produce verbose output"
    },
    {
        .name = "node",
        .key = 'n',
        .arg = "NODE-ID",
        .flags = 0,
        .doc = "The CAN Open node ID we want to talk to"
    },
    {
        .name = "listen",
        .key = ARG_LISTEN,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Listen on the bus, print messages"
    },
    {
        .name = "range",
        .key = 'r',
        .arg = "MIN:MAX",
        .flags = 0,
        .doc = "Range of CAN ID's to listen for, as \"MIN:MAX\" in hex"
    },
    {
        .name = "readsdo",
        .key = ARG_READ_SDO,
        .arg = "SDO",
        .flags = 0,
        .doc = "Mode Setting: SDO to read, specified in hex"
    },
    {
        .name = "canstatus",
        .key = ARG_CAN_STATUS,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Print CAN Status Information"
    },
    {
        .name = "byte",
        .key = 'b',
        .arg = "DATA",
        .flags = 0,
        .doc = "A byte of data for Write mode"
    },
    {
        .name = "id",
        .key = 'i',
        .arg = "CAN_ID",
        .flags = 0,
        .doc = "CAN ID in hex"
    },
    {
        .name = "baud",
        .key = ARG_BAUD,
        .arg = "BAUD",
        .flags = 0,
        .doc = "can baud in kbps"
    },
    {
        .name = "write",
        .key = ARG_WRITE,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Write CAN Message"
    },
    {
        .name = "net",
        .key = ARG_NET,
        .arg = NULL,
        .flags = 0,
        .doc = "CAN Network to use, default 0"
    },
    {0}
};

static error_t parse_opt( int key, char *arg, struct argp_state *state);

const char *argp_program_version = "sparky-0";
static char doc[] = "Control Program for Sparky the Robot";
static char args_doc[] = "...";
static struct argp argp = {options, parse_opt, args_doc, doc };

typedef struct {
    int dostatus;
    int dowrite;
    int verbosity;
    int node;
    int listen;
    int canid_range[2];
    int32_t id;
    uint8_t data_bytes[8];
    uint8_t length;
    int baud_kbps;
    int net;
} args_t;

static args_t args = {
    .dostatus = 0,
    .dowrite = 0,
    .verbosity = 0,
    .node = -1,
    .listen = 0,
    .canid_range = {-1, -1},
    .length = 0,
    .id = -1,
    .data_bytes = {0},
    .baud_kbps = DEFAULT_BAUD,
    .net = 0
};

/*-- END ARG PARSING --*/




void fail(char *msg) {
    fprintf(stderr, "\%s\n", msg);
    exit(EXIT_FAILURE);
}

int ntcan_baud_code( int kbps ) {
    if( kbps >= 1000 )
        return NTCAN_BAUD_1000;
    if( kbps >= 800 )
        return NTCAN_BAUD_800;
    if( kbps >= 500 )
        return NTCAN_BAUD_500;
    if( kbps >= 250 )
        return NTCAN_BAUD_250;
    if( kbps >= 125 )
        return NTCAN_BAUD_125;
    if( kbps >= 100 )
        return NTCAN_BAUD_100;
    if( kbps >= 50 )
        return NTCAN_BAUD_50;
    if( kbps >= 20 )
        return NTCAN_BAUD_20;
    if( kbps >= 10 )
        return NTCAN_BAUD_10;
    return NTCAN_BAUD_1000;
}

int ntcan_baud_kbps( int ntcan_baud ) {
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

char *ntcan_result_to_str( NTCAN_RESULT ntr ) {
    switch( ntr ) {
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
    default: return "unknown";
    }
}

void ntcan_success_or_die(char *op_name, NTCAN_RESULT ntr ) {
    if( ntr != NTCAN_SUCCESS ){
        fprintf(stderr,
                "Encountered error on NTCAN operation %s\n"
                "NTCAN_RESULT: %s\n",
                op_name, ntcan_result_to_str(ntr));
        exit(EXIT_FAILURE);
    }
}

static void ntcan_debug(int level, char *op, NTCAN_RESULT ntr ) {
    if( ntr != NTCAN_SUCCESS) level++;
    DEBUGF(level, "%s: %s\n", op, ntcan_result_to_str( ntr ));
}


void ntcan_dump( CMSG *pmsg ) {
    unsigned int u = pmsg->id;
    printf("%x[%d] ", u, pmsg->len) ;
    int i;
    for( i = 0; i < pmsg->len; i++) {
        u = pmsg->data[i];
        printf("%x:", u);
    }
    printf("\n");
}

/* Loop that listens for CAN message and prints to stdout */
void dolisten() {
    NTCAN_HANDLE h;
    NTCAN_RESULT ntr;
    //check for specified range range
    if( !(args.canid_range[0] >= 0 &&
          args.canid_range[1] >= args.canid_range[0] )) {
        fail("Must specify valid id range");
    }
    // open can handle
    ntr = canOpen( args.net,   //net
                   0,   //flags
                   10,  //txqueue
                   100,  //rxqueue
                   1000, //txtimeout
                   2000, //rxtimeout
                   &h // handle
        );
    ntcan_success_or_die("Open", ntr);
    ntcan_debug(INFO, "open", ntr);
    // bind id's
    {
        int i;
        for(i = args.canid_range[0]; i < args.canid_range[1]; i++ ){
            canIdAdd( h, i );
            ntcan_debug(INFO, "IdAdd", ntr);
            ntcan_success_or_die("IdAdd", ntr);
        }

    }
    // read-print loop
    { CMSG msg;
        int num = 1;
        while(1) {
            num = 1;
            ntr = canRead( h, &msg, &num, NULL );
            if( NTCAN_RX_TIMEOUT == ntr ) {
                DEBUGF(INFO,"timeout\n");
                continue;
            }
            ntcan_debug(DEBUG, "read", ntr);
            if( NTCAN_SUCCESS == ntr ) {
                ntcan_dump(&msg);
            }
        }
    }

    //close
    ntr = canClose( h );
    ntcan_debug( INFO, "canClose", ntr );
    ntcan_success_or_die("Close", ntr);
    return;
}

void ntcan_print_status(int net) {
    NTCAN_HANDLE h;
    NTCAN_RESULT ntr;
    CAN_IF_STATUS status;
    uint32_t baud;

    int bitset( uint16_t u, int bit ) {
        return (u >> bit) & 1;
    }
    void try_feature( int bit, char *name){
        if( bitset( status.features, bit ) )
            printf(" %s", name );
    }
    void print_version(uint16_t u){
        int rev = u & 0xff;
        int min = (u >> 8) & 0xf;
        int maj = (u >> 12) & 0xf;
        printf("%d.%d.%d", maj, min, rev);
    }


    //open
    ntr = canOpen( net,   //net
                   0,   //flags
                   10,  //txqueue
                   100,  //rxqueue
                   1000, //txtimeout
                   5000, //rxtimeout
                   &h // handle
        );
    ntcan_success_or_die("Open", ntr);
    ntcan_debug(INFO, "open", ntr);

    // status
    ntr =  canStatus(h, &status);
    ntcan_success_or_die("Status", ntr);
    ntcan_debug(INFO, "Status", ntr);

    // baud
    ntr =  canGetBaudrate(h, &baud);
    ntcan_success_or_die("GetBaudrate", ntr);
    ntcan_debug(INFO, "GetBaudrate", ntr);

    //print
    printf( " ESD CAN STATUS\n"
            "----------------\n");

    printf( "status:   %u (%s)\n", status.boardstatus,
            (0==status.boardstatus)?"OK":"ERROR" );
    printf( "Baudrate: %d kbps\n", ntcan_baud_kbps( baud ) );
    printf( "id:       %s\n", status.boardid );
    printf( "hardware: "); print_version(status.hardware); printf("\n");
    printf( "firmware: "); print_version(status.firmware); printf("\n");
    printf( "driver:   "); print_version(status.driver);   printf("\n");
    printf( "dll:      "); print_version(status.dll);      printf("\n");
    printf( "features: 0x%x -", status.features);
    try_feature(0, "FULL_CAN");
    try_feature(1, "CAN_20B");
    try_feature(2, "DEVICE_NET");
    try_feature(3, "CYCLIC_TX");
    try_feature(4, "RX_OBJECT_MODE");
    try_feature(5, "TIMESTAMP");
    try_feature(6, "LISTEN_ONLY_MODE");
    try_feature(7, "SMART_DISCONNECT");
    printf("\n\n");



    //close
    ntr = canClose( h );
    ntcan_debug( INFO, "canClose", ntr );
    ntcan_success_or_die("Close", ntr);
    return;
}

void dowrite() {
    //check args
    if( args.id < 0 )
        fail("Must Specify ID to send message");


    NTCAN_RESULT ntr;
    NTCAN_HANDLE h;
    CMSG msg;
    int baudcode = ntcan_baud_code( args.baud_kbps );

    //make message
    msg.id = args.id;
    memcpy( msg.data, args.data_bytes, 8 );
    msg.len = args.length;

    if( args.verbosity >= DEBUG )  {
        printf("Sending message: ");
        ntcan_dump( &msg );
        printf(" at %d kbps\n", ntcan_baud_kbps(baudcode) );
    }

    //open

    ntr = canOpen( args.net,   //net
                   0,   //flags
                   10,  //txqueue
                   100,  //rxqueue
                   1000, //txtimeout
                   5000, //rxtimeout
                   & h // handle
        );
    ntcan_debug( INFO, "canOpen", ntr );
    ntcan_success_or_die("Open", ntr);

    //set baud
    ntr = canSetBaudrate( h, baudcode );
    ntcan_debug( INFO, "canSetBaudrate", ntr );
    ntcan_success_or_die("SetBaudrate", ntr);


    //send message
    int num = 1; //sending 1 message
    ntr = canWrite( h, &msg, &num, NULL );
    ntcan_debug( INFO, "canWrite", ntr );
    ntcan_success_or_die("Write", ntr);

    //close
    ntr = canClose( h );
    ntcan_debug( INFO, "canClose", ntr );
    ntcan_success_or_die("Close", ntr);
    return;
}

int main( int argc, char **argv ) {
    argp_parse (&argp, argc, argv, 0, NULL, NULL);
    if( args.dostatus ) {
        ntcan_print_status(args.net);
    }
    if( args.listen ) {
        dolisten();
    }
    if( args.dowrite ) {
        dowrite();
    }

    return EXIT_SUCCESS;
}

static error_t parse_opt( int key, char *arg, struct argp_state *state) {
    //printf("parse_opt: %c->%s\n", key, arg);

    unsigned int parse_hex() {
        unsigned int u;
        sscanf(arg,"%x", &u);
        return u;
    }

    switch( key ) {
    case 'v': args.verbosity++;
        break;
    case 'n': args.node = atoi(arg);
        break;
    case 'b':
        args.data_bytes[args.length++] = parse_hex();
        break;
    case 'i':
        args.id = parse_hex();
        break;
    case 'r':
    {
        unsigned int min, max;
        sscanf(arg, "%x:%x", &min, &max );
        args.canid_range[0] = min;
        args.canid_range[1] = max;
    }
    break;
    case ARG_CAN_STATUS: args.dostatus++;
        break;
    case ARG_LISTEN: args.listen++;
        break;
    case ARG_WRITE: args.dowrite++;
        break;
    case ARG_BAUD: args.baud_kbps = atoi(arg);
        break;
    case ARG_NET: args.net = atoi(arg);
        break;
    case 0: //noflag arg
        break;
    }
    return 0;
}
