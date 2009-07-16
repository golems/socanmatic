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
 *
 * \bug Killing process during a read will kernel panic.  We use a
 * signal handler to a more orderly shutdown.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>
#include <assert.h>
#include <stdint.h>
#include <signal.h>
#include "ntcan.h"
#include "ntcanopen.h"

// verbosity output levels

#define WARN 0  ///< verbosity level for warnings
#define INFO 1  ///< verbosity level for info messages
#define DEBUG 2 ///< verbosity level for debug messages

#define DEFAULT_BAUD 1000 ///< default can baud rate in kbps

/// print a debug messages at some level
#define DEBUGF(level, fmt, a... )                                       \
    (((args.verbosity) >= level )?fprintf( stderr, (fmt), ## a ) : 0);

// ARG_KEYS
#define ARG_READ_SDO 300     ///< an argp key
#define ARG_CAN_STATUS 301   ///< an argp key
#define ARG_LISTEN 302       ///< an argp key
#define ARG_WRITE 303        ///< an argp key
#define ARG_BAUD 304         ///< an argp key
#define ARG_NET 305          ///< an argp key
#define ARG_WRITE_SDO 306    ///< an argp key
#define ARG_NMT 307    ///< an argp key

/* -- ARG PARSING JUNK --*/
static struct argp_option options[] = {
    // mode flags
    {
        .name = "listen",
        .key = ARG_LISTEN,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Listen on the bus, print messages"
    },
    {
        .name = "readsdo",
        .key = ARG_READ_SDO,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Read an SDO"
    },
    {
        .name = "writesdo",
        .key = ARG_WRITE_SDO,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Write an SDO"
    },
    {
        .name = "canstatus",
        .key = ARG_CAN_STATUS,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Print CAN Status Information"
    },
    {
        .name = "write",
        .key = ARG_WRITE,
        .arg = NULL,
        .flags = 0,
        .doc = "Mode Setting: Write CAN Message"
    },
    {
        .name = "nmt",
        .key = ARG_NMT,
        .arg = "[start_remote|stop_remote|pre_op|reset_node|reset_com]",
        .flags = 0,
        .doc = "Mode Setting: send NMT message"
    },
    // option flags
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
        .name = "range",
        .key = 'r',
        .arg = "MIN:MAX",
        .flags = 0,
        .doc = "Range of CAN ID's to listen for, as \"MIN:MAX\" in hex"
    },
    {
        .name = "byte",
        .key = 'b',
        .arg = "DATA",
        .flags = 0,
        .doc = "A byte of data for Write mode, hex, wire order"
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
        .name = "net",
        .key = ARG_NET,
        .arg = NULL,
        .flags = 0,
        .doc = "CAN Network to use, default 0"
    },
    {
        .name = "index",
        .key = 'X',
        .arg = "INDEX",
        .flags = 0,
        .doc = "CANopen SDO index"
    },
    {
        .name = "subindex",
        .key = 'x',
        .arg = "SUBINDEX",
        .flags = 0,
        .doc = "CANopen SDO subindex"
    },
    {0}
};

/// argp parsing function
static error_t parse_opt( int key, char *arg, struct argp_state *state);
/// argp program version
const char *argp_program_version = "esdcantool-0.1";
/// argp program doc line
static char doc[] = "Shell tool to interact with esd CAN cards";
/// argp program arguments documention
static char args_doc[] = "...";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc };

/// struct type declaration for parsed arguments
struct {
    int mode;

    int verbosity;
    int node;
    int canid_range[2];
    int32_t id;
    uint8_t data_bytes[8];

    int baud_kbps;
    int net;

    uint8_t length;

    int32_t index;
    int16_t subindex;

    canopen_nmt_msg_t nmt_msg;
} args = {
    //.dostatus = 0,
    //.dowrite = 0,
    .mode = -1,
    .verbosity = 0,
    .node = -1,
    .canid_range = {-1, -1},
    .length = 0,
    .id = -1,
    .data_bytes = {0},
    .baud_kbps = DEFAULT_BAUD,
    .net = 0,
    .index = -1,
    .subindex = -1,
    .nmt_msg = CANOPEN_NMT_INVAL
};

/*-- END ARG PARSING --*/


static int shutdown = 0;


void sigshutdown(int sig) {
    switch(sig) {
    case SIGINT:
        printf("\nShutting Down...\n");
        shutdown = 1;
        break;
    default:
        shutdown = 1;
        fprintf(stderr, "Uknown Signal, exiting\n");
    }
}

/// print failure message and exit
void fail(char *msg) {
    fprintf(stderr, "\%s\n", msg);
    exit(EXIT_FAILURE);
}

/// convert baud in kbps to NTCAN buad code
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

/// convert NTCAN baud code to baud in kbps
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

/// return a char[] describing the ntcan result
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

/// exit if ntr is not NTCAN_SUCCESS
void ntcan_success_or_die(char *op_name, NTCAN_RESULT ntr ) {
    if( ntr != NTCAN_SUCCESS ){
        fprintf(stderr,
                "Encountered error on NTCAN operation %s\n"
                "NTCAN_RESULT: %s\n",
                op_name, ntcan_result_to_str(ntr));
        exit(EXIT_FAILURE);
    }
}

/// print debug message about the NTCAN_RESULT
static void ntcan_debug(int level, char *op, NTCAN_RESULT ntr ) {
    if( ntr != NTCAN_SUCCESS) level++;
    DEBUGF(level, "%s: %s\n", op, ntcan_result_to_str( ntr ));
}

/// opens can handle according to parsed command line parameters
static void tool_canOpen(NTCAN_HANDLE *ph) {
    NTCAN_RESULT ntr;
    NTCAN_HANDLE h;
    int baudcode = ntcan_baud_code( args.baud_kbps );

    // open can handle
    ntr = canOpen( args.net,   //net
                   0,   //flags
                   10,  //txqueue
                   100,  //rxqueue
                   1000, //txtimeout
                   1000, //rxtimeout
                   &h // handle
        );
    ntcan_success_or_die("Open", ntr);
    ntcan_debug(INFO, "open", ntr);

    //set baud
    ntr = canSetBaudrate( h, baudcode );
    ntcan_debug( INFO, "canSetBaudrate", ntr );
    ntcan_success_or_die("SetBaudrate", ntr);

    *ph = h;
}

/// prints can message to stdout
void ntcan_dump( CMSG *pmsg ) {
    unsigned int u = pmsg->id;
    printf("%03x[%d] ", u, pmsg->len) ;
    int i;
    for( i = 0; i < pmsg->len; i++) {
        u = pmsg->data[i];
        printf("%02x%s", u, (i < pmsg->len - 1)?":":"" );
    }
}

/** Loop that listens for CAN message and prints to stdout */
void dolisten() {
    NTCAN_HANDLE h;
    NTCAN_RESULT ntr;
    int baudcode = ntcan_baud_code( args.baud_kbps );
    //check for specified range range
    if( !(args.canid_range[0] >= 0 &&
          args.canid_range[1] >= args.canid_range[0] )) {
        fail("Must specify valid id range");
    }

    // open can handle

    tool_canOpen( &h );

    // bind id's
    {
        int i;
        for(i = args.canid_range[0]; i < args.canid_range[1]; i++ ){
            ntr = canIdAdd( h, i );
            ntcan_debug(INFO, "IdAdd", ntr);
            ntcan_success_or_die("IdAdd", ntr);
        }

    }
    // read-print loop
    { CMSG msg;
        int num = 1;
        while(!shutdown) {
            num = 1;
            ntr = canRead( h, &msg, &num, NULL );
            if( NTCAN_RX_TIMEOUT == ntr ) {
                DEBUGF(INFO,"timeout\n");
                continue;
            }
            ntcan_debug(DEBUG, "read", ntr);
            if( NTCAN_SUCCESS == ntr ) {
                ntcan_dump(&msg);
                printf("\n");
            }
        }
    }

    //close
    ntr = canClose( h );
    ntcan_debug( INFO, "canClose", ntr );
    ntcan_success_or_die("Close", ntr);
    fprintf(stderr, "done\n");
    return;
}

/// print status of can interface to stdout
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


    int i=1;
    printf("net id is: %d\n", i);
    //open
    ntr = canOpen( i,   //net
                   0,   //flags
                   10,  //txqueue
                   100,  //rxqueue
                   1000, //txtimeout
                   5000, //rxtimeout
                   &h // handle
        );

    printf("canOpen returned: %d\n", ntr);
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

/// write a single can message
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


    // open
    tool_canOpen( &h );

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

/// read a CANopen SDO
void doreadsdo() {
    sdo_msg_t sdo, rsdo;
    CMSG msg;
    NTCAN_RESULT ntr;
    NTCAN_HANDLE h;
    int baudcode = ntcan_baud_code( args.baud_kbps );

    // check args
    if( args.node < 0 || args.node > 127 )
        fail("Must specify valid node id");
    if( -1 == args.index )
        fail("Must specify valid index");
    if( -1 == args.subindex )
        fail("Must specify valid subindex");

    // build sdo request
    sdo.node = args.node;
    sdo.command = canOpenCommand( CANOPEN_EX_UL, 0, 1, 0);
    //sdo.command = 0x40;
    sdo.index = args.index;
    sdo.subindex = args.subindex;
    sdo.length = 0;
    canOpenTranslateSDO( &msg, &sdo, 0 );
    if( args.verbosity >= INFO ) {
        printf("Sending SDO: "); canOpenDumpSDO( &sdo ); printf("\n");
        printf("CMSG:        "); ntcan_dump( &msg ); printf("\n");
    }

    // open
    tool_canOpen( &h );

    // bind proper id
    canOpenIdAddSDOResponse( h, args.node );

    // communicate
    ntr = canOpenSDOWriteWait( h, &sdo, &rsdo );
    ntcan_debug( INFO, "write,wait", ntr );
    ntcan_success_or_die("write,wait", ntr);

    //close
    ntr = canClose( h );
    ntcan_debug( INFO, "canClose", ntr );
    ntcan_success_or_die("Close", ntr);

    // print
    canOpenTranslateSDO( &msg, &rsdo, 1 );
    if( args.verbosity >= INFO ) {
        printf("Received CMSG: "); ntcan_dump( &msg ); printf("\n");
        printf("Received SDO:  "); canOpenDumpSDO( &sdo ); printf("\n");
    }else {
        canOpenDumpSDO( &sdo ); printf("  -->  ");
        canOpenDumpSDO( &rsdo ); printf("\n");
    }

}

/// write and sdo object
void dowritesdo() {
    sdo_msg_t sdo, rsdo;
    CMSG msg;
    NTCAN_RESULT ntr;
    NTCAN_HANDLE h;

    //check params
    if( args.node < 0 || args.node > 127 )
        fail("Must specify valid node id");
    if( -1 == args.index )
        fail("Must specify valid index");
    if( -1 == args.subindex )
        fail("Must specify valid subindex");
    if( 0 == args.length || args.length > 4 )
        fail("Must specify valid number of data bytes");


    //build sdo
    sdo.node = args.node;
    sdo.command = canOpenCommand( CANOPEN_EX_DL, 0, 1, 0);
    sdo.index = args.index;
    sdo.subindex = args.subindex;
    sdo.length = args.length;
    memcpy( sdo.data, args.data_bytes, sdo.length );
    canOpenTranslateSDO( &msg, &sdo, 0 );
    if( args.verbosity >= INFO ) {
        printf("Sending SDO: "); canOpenDumpSDO( &sdo ); printf("\n");
        printf("CMSG:        "); ntcan_dump( &msg ); printf("\n");
    }

    // open
    tool_canOpen( &h );

    // bind proper id
    canOpenIdAddSDOResponse( h, args.node );

    // communicate
    ntr = canOpenSDOWriteWait( h, &sdo, &rsdo );
    ntcan_debug( INFO, "write,wait", ntr );
    ntcan_success_or_die("write,wait", ntr);

    //close
    ntr = canClose( h );
    ntcan_debug( INFO, "canClose", ntr );
    ntcan_success_or_die("Close", ntr);

    // print
    canOpenTranslateSDO( &msg, &sdo, 1 );
    if( args.verbosity >= INFO ) {
        printf("Received CMSG: "); ntcan_dump( &msg ); printf("\n");
        printf("Received SDO:  "); canOpenDumpSDO( &sdo ); printf("\n");
    }else {
        canOpenDumpSDO( &sdo ); printf("  -->  ");
        canOpenDumpSDO( &rsdo ); printf("\n");
    }
}

/// send nmt message
void donmt() {
    NTCAN_HANDLE h;

    if( -1 == args.node )
        fail("Must specify target node of NMT message");
    assert(CANOPEN_NMT_INVAL != args.nmt_msg );
    // open
    tool_canOpen( &h );

    // send
    NTCAN_HANDLE ntr = canOpenWriteNMT( h, args.node, args.nmt_msg );
    ntcan_debug( INFO, "nmt", ntr );
    ntcan_success_or_die("nmt", ntr);

    //close
    ntr = canClose( h );
    ntcan_debug( INFO, "canClose", ntr );
    ntcan_success_or_die("Close", ntr);
}

/// main
int main( int argc, char **argv ) {
    argp_parse (&argp, argc, argv, 0, NULL, NULL);
    switch(args.mode) {
    case ARG_CAN_STATUS:
        ntcan_print_status(args.net);
        break;
    case ARG_LISTEN:
        signal(SIGINT, sigshutdown );
        dolisten();
        break;
    case ARG_WRITE:
        dowrite();
        break;
    case ARG_READ_SDO:
        doreadsdo();
        break;
    case ARG_WRITE_SDO:
        dowritesdo();
        break;
    case ARG_NMT:
        donmt();
        break;
    default:
        fail("Must specify valid mode");
    }

    return EXIT_SUCCESS;
}

/// argp parsing function
static error_t parse_opt( int key, char *arg, struct argp_state *state) {
    //printf("parse_opt: %c->%s\n", key, arg);

    unsigned int parse_hex() {
        unsigned int u;
        if( sscanf(arg,"%x", &u) != 1 )
            fail( "Invalid number format" );
        return u;
    }

    switch( key ) {
    case 'v': args.verbosity++;
        break;
    case 'n': args.node = parse_hex();
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
        if( sscanf(arg, "%x:%x", &min, &max ) != 2 )
            fail( "Invalid range format" );
        args.canid_range[0] = min;
        args.canid_range[1] = max;
    }
    break;
    case 'X': args.index = parse_hex();
        break;
    case 'x': args.subindex = parse_hex();
        break;
    case ARG_BAUD: args.baud_kbps = atoi(arg);
        break;
    case ARG_NET: args.net = atoi(arg);
        break;

        // MODES
    case ARG_NMT:
        if( strcasecmp( arg, "start_remote") == 0 )
            args.nmt_msg = CANOPEN_NMT_START_REMOTE;
        else if( strcasecmp( arg, "stop_remote") == 0 )
            args.nmt_msg = CANOPEN_NMT_STOP_REMOTE;
        else if( strcasecmp( arg, "pre_op") == 0 )
            args.nmt_msg = CANOPEN_NMT_PRE_OP;
        else if( strcasecmp( arg, "reset_node") == 0 )
            args.nmt_msg = CANOPEN_NMT_RESET_NODE;
        else if( strcasecmp( arg, "reset_com") == 0 )
            args.nmt_msg = CANOPEN_NMT_RESET_COM;
        else
            fail("Must specify valid NMT message type");
        // don't break
    case ARG_CAN_STATUS:
    case ARG_LISTEN:
    case ARG_WRITE:
    case ARG_READ_SDO:
    case ARG_WRITE_SDO:
        if( -1 == args.mode ) args.mode = key;
        else fail("Cannot specify multiple modes");
    case 0: //noflag arg
        break;
    }

    return 0;
}
