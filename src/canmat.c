/* -*- mode: C; c-basic-offset: 4 -*- */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*
 * Copyright (c) 2008, Georgia Tech Research Corporation
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


#include "config.h"

#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <poll.h>

#include "socanmatic.h"
#include "socanmatic/dict402.h"
#include "socanmatic_private.h"
#include "socanmatic/util.h"




int opt_verbosity = 0;
static const char *opt_api = "socketcan";

static const char **opt_pos = NULL;
static size_t opt_npos = 0;

//uint16_t opt_canid = 0;
//uint8_t opt_can_dlc = 0;
//uint8_t opt_can_data[8];
//
//canmat_sdo_msg_t opt_sdo = {0};



//uint8_t opt_node;
//const char *opt_dict_param_name;
//const char *opt_dict_param_value;

//char const ** opt_ifaces = NULL;
//size_t opt_n_ifaces;




struct iface_list {
    struct iface_list *next;
};

typedef struct can_set {
    size_t n;
    canmat_iface_t **cif;
    const char **name;
    struct pollfd *pfd;
} can_set_t;

typedef int (*cmd_fun_t)(can_set_t*,size_t, const char**);

static cmd_fun_t opt_command = NULL;

static int cmd_send( can_set_t *canset, size_t n, const char **args );
static int cmd_dump( can_set_t *canset, size_t n, const char **args );
static int cmd_ul( can_set_t *canset, size_t n, const char **args );
static int cmd_ul_resp( can_set_t *canset, size_t n, const char **args );
static int cmd_dl( can_set_t *canset, size_t n, const char **args );
static int cmd_dl_resp( can_set_t *canset, size_t n, const char **args );
static int cmd_dict_dl( can_set_t *canset, size_t n, const char **args );
static int cmd_dict_ul( can_set_t *canset, size_t n, const char **args );
static int cmd_display( can_set_t *canset, size_t n, const char **args );
static int cmd_info( can_set_t *canset, size_t n, const char **args );
static int cmd_set( can_set_t *canset, size_t n, const char **args );
static int cmd_nmt( can_set_t *canset, size_t n, const char **args );
static int cmd_probe( can_set_t *canset, size_t n, const char **args );
static int cmd_map_rpdo( can_set_t *canset, size_t n, const char **args );


/***************/
/* ARG PARSING */
/***************/



static cmd_fun_t posarg_cmd( const char *arg ) {
    static const struct  {
        const char *name;
        cmd_fun_t fun;
    } cmds[] = { {"dump", cmd_dump},
                 {"display", cmd_display},
                 {"send", cmd_send},
                 {"dl", cmd_dl},
                 {"dl-resp", cmd_dl_resp},
                 {"ul", cmd_ul},
                 {"ul-resp", cmd_ul_resp},
                 {"dict-dl", cmd_dict_dl},
                 {"dict-ul", cmd_dict_ul},
                 {"info", cmd_info},
                 {"set", cmd_set},
                 {"nmt", cmd_nmt},
                 {"probe", cmd_probe},
                 {"map-rpdo", cmd_map_rpdo },
                 {NULL, NULL} };
    size_t i;
    for( i = 0; cmds[i].name != NULL; i ++ ) {
        if( 0 == strcasecmp( arg, cmds[i].name ) ) {
            return cmds[i].fun;
        }
    }
    invalid_arg( arg );
    assert(0);
}

static void posarg( const char *arg, int i ) {
    if( 0 == i ) {
        verbf( 1, "Setting command: %s\n", arg );
        opt_command = posarg_cmd(arg);
    } else {
        opt_pos = (const char**) realloc( opt_pos, sizeof(opt_pos[0]) * (opt_npos+1) );
        opt_pos[opt_npos++] = arg;
    }
}

static void set_iface( can_set_t *canset, const char *type, const char *name ) {
    canset->cif = (canmat_iface_t**) realloc( canset->cif, sizeof(canset->cif[0]) * (canset->n+1) );
    canset->name = (const char**) realloc( (void*)canset->name, sizeof(char*) * (canset->n+1) );
    canset->cif[canset->n] = open_iface(type, name );
    canset->name[ canset->n ] = strdup(name);
    canset->n++;
}

int main( int argc, char ** argv ) {

    can_set_t canset = {0};

    int c, i = 0;
    while( (c = getopt( argc, argv, "vhH?Vf:a:")) != -1 ) {
        switch(c) {
        case 'V':   /* version     */
            puts( "canmat " PACKAGE_VERSION "\n"
                  "\n"
                  "Copyright (c) 2008-2013, Georgia Tech Research Corporation\n"
                  "This is free software; see the source for copying conditions.  There is NO\n"
                  "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
                  "\n"
                  "Written by Neil T. Dantam"
                );
            exit(EXIT_SUCCESS);
        case 'v':   /* verbose  */
            opt_verbosity++;
            break;
        case 'a':   /* api  */
            opt_api = optarg;
            break;
        case 'f':   /* interface  */
            set_iface( &canset, opt_api, optarg );
            break;
        case '?':   /* help     */
        case 'h':
        case 'H':
            puts( "Usage: canmat [OPTIONS...] COMMAND [command-args...]\n"
                  "Shell tool for CANopen\n"
                  "\n"
                  "Options:\n"
                  "  -v,                       Make output more verbose\n"
                  "  -a api_type,              CAN API, e.g, socketcan, ntcan\n"
                  "  -f interface,             CAN interface (multiple allowed)\n"
                  "  -?,                       Give program help list\n"
                  "  -V,                       Print program version\n"
                  "\n"
                  "Examples:\n"
                  "  canmat dump                                  Print CAN messages to standard output\n"
                  "  canmat info                                  Print info about interface\n"
                  "  canmat display                               Pretty-print CAN messages to standard output\n"
                  "  canmat send id b0 ... b7                     Send a can message (values in hex)\n"
                  "  canmat dict-dl node param-name value         Download SDO to node\n"
                  "  canmat dict-ul node param-name               Upload SDO from node\n"
                  "  canmat dl node idx subidx bytes-or-val       Download SDO to node\n"
                  "  canmat dl-resp node idx subidx               Simulate node download response\n"
                  "  canmat ul node idx subidx                    Upload SDO from node\n"
                  "  canmat ul-resp node idx subidx bytes-or-val  Simulate node upload response\n"
                  "  canmat set bitrate value                     Set bitrate in kbps\n"
                  "  canmat nmt node (start|stop|preop|reset-(node|com))\n"
                  "                                               Send an NMT message\n"
                  "  canmat map-rpdo node pdo-num param-name      Establish RPDO mapping\n"
                  "\n"
                  "Report bugs to <ntd@gatech.edu>"
                );
            exit(EXIT_SUCCESS);
            break;
        default:
            posarg(optarg, i++);
        }
    }
    while( optind < argc ) {
        posarg(argv[optind++], i++);
    }

    hard_assert( opt_command, "canmat: missing command.\nTry `canmat -H' for more information.\n");


    // TODO: check fds if multiple interfaces

    // allocate poll fds
    if( canset.n > 1 ) {
        canset.pfd = (struct pollfd*) calloc( canset.n , sizeof(canset.pfd[0]) );
        for( size_t j = 0; j < canset.n; j ++ ) {
            canset.pfd[j].fd = canset.cif[j]->fd;
            hard_assert( canset.pfd[j].fd >= 0, "Invalid fd for interface %s (%d)\n",
                         canset.name[j], canset.pfd[j].fd );
        }
    }

    hard_assert( canset.n, "canmat: missing interface.\nTry `canmat -H' for more information.\n");
    return opt_command(&canset, opt_npos, opt_pos);

    return 0;
}


/************/
/* COMMANDS */
/************/

static void pollin_raw( struct can_frame *can ) {
    canmat_dump_frame( stdout, can );
    fflush( stdout );
}

static void pollin_display( struct can_frame *can ) {
    canmat_display( &canmat_dict402, can );
}

static void pollin1( const char *name, canmat_iface_t *cif, void (printer)(struct can_frame*) ) {
    struct can_frame can;
    // read the actual message
    canmat_status_t r = canmat_iface_recv( cif, &can );

    hard_assert( CANMAT_OK == r, "Couldn't recv frame: %s\n", canmat_iface_strerror(cif, r) );

    if( CANMAT_OK == r ) {
        fprintf( stdout, "%s: ", name );
        printer( &can );
    }
}

static int cmd_pollin( can_set_t *canset, void (printer)(struct can_frame*) ) {

    // TODO: use threads for interfaces without a file descriptor

    if( 1 == canset->n ) {
        while(1) {
            pollin1( canset->name[0], canset->cif[0], printer );
        }
    }

    // set events
    for( size_t i = 0; i < canset->n; i ++ ) {
        canset->pfd[i].events = POLLIN;
    }

    // FIXME: if network goes down and up, we don't detect till poll
    // sees new data.  Then read() fails

    while(1) {
        // wait for data
        int rp = poll( canset->pfd, canset->n , -1 );
        hard_assert(rp > 0, "Couldn't poll interfaces (%d): %s (%d)\n", rp, strerror(errno), errno);

        // print all available data
        for( size_t i = 0; i < canset->n; i ++ ) {
            hard_assert( 0 == ((canset->pfd[i].revents & POLLERR) ||
                               (canset->pfd[i].revents & POLLHUP) ||
                               (canset->pfd[i].revents & POLLNVAL) ),
                         "Error on iface %s\n", canset->name[i] );
            if( canset->pfd[i].revents & POLLIN ) {
                pollin1( canset->name[i], canset->cif[i], printer );
            }
        }
    }
    return 0;
}


static int cmd_dump( can_set_t *canset, size_t n, const char **arg ) {
    hard_assert( 0 == n && NULL == arg, "Extra arguments\n");

    cmd_pollin( canset, pollin_raw );
    return 0;
}

static int cmd_display( can_set_t *canset, size_t n, const char **arg ) {
    hard_assert( 0 == n && NULL == arg, "Extra arguments\n");
    cmd_pollin( canset, pollin_display );
    return 0;
}

static int send_frame( can_set_t *canset, struct can_frame *can ) {
    if(opt_verbosity) {
        verbf(1, "Sending ");
        canmat_dump_frame( stdout, can );
    }
    for( size_t i = 0; i < canset->n; i ++  ) {
        canmat_status_t r = canmat_iface_send( canset->cif[i], can );
        if( CANMAT_OK != r ) {
            fprintf( stderr, "Couldn't send frame on %s: %s\n",
                     canset->name[i], strerror(errno) );
        }
    }
    return 0;
}


static int cmd_send( can_set_t *canset, size_t n, const char **arg ) {
    hard_assert( n >= 1, "Insufficient arguments\n");
    hard_assert( n <= 1 + 8, "Extra arguments\n");

    struct can_frame can;
    can.can_id = (uint16_t)parse_uhex( arg[0], 0x7FF );
    can.can_dlc = (uint8_t)(n - 1);
    for( size_t i = 1; i < n; i ++ ) {
        can.data[i-1] = (uint8_t)parse_uhex(arg[i], 0xFF);
    }

    return send_frame( canset, &can );
}

static void parse_arg_sdo( size_t n, const char **arg, canmat_sdo_msg_t *sdo ) {
    hard_assert( n >= 3, "Insufficient arguments\n");
    hard_assert( n <= 3 + 4, "Extra arguments\n");

    sdo->node = (uint8_t)parse_uhex( arg[0], CANMAT_NODE_MASK );
    sdo->index = (uint16_t)parse_uhex( arg[1], 0xFFFF );
    sdo->subindex = (uint8_t)parse_uhex( arg[2], 0xFF );

    sdo->length = (uint8_t)(n - 3);

    for( size_t i = 3; i < n; i ++ ) {
        hard_assert( (cmd_dl == opt_command || cmd_ul_resp == opt_command),
                     "Extra arguments: %s\n", arg[i] );
        sdo->data.byte[ i-3 ] = (uint8_t)parse_uhex( arg[i], 0xFF );
    }
}

static int cmd_ul( can_set_t *canset, size_t n, const char **arg ) {
    // FIXME
    assert(0);
}

static int cmd_ul_resp( can_set_t *canset, size_t n, const char **arg ) {
    canmat_sdo_msg_t sdo;
    parse_arg_sdo( n, arg, &sdo );
    sdo.cmd_spec = CANMAT_SCS_EX_UL;
    struct can_frame can;

    canmat_status_t r = canmat_sdo2can( &can, &sdo, 1 );
    hard_assert( CANMAT_OK == r, "Couldn't build CAN frame: %s\n", canmat_strerror(r) );
    return send_frame( canset, &can );
}

static int cmd_dl( can_set_t *canset, size_t n, const char **arg ) {
    // FIXME
    assert(0);
}

static int cmd_dl_resp( can_set_t *canset, size_t n, const char **arg ) {
    canmat_sdo_msg_t sdo;
    parse_arg_sdo( n, arg, &sdo );
    sdo.cmd_spec = CANMAT_SCS_EX_DL;
    struct can_frame can;
    canmat_status_t r = canmat_sdo2can( &can, &sdo, 1 );
    hard_assert( CANMAT_OK == r, "Couldn't build CAN frame: %s\n", canmat_strerror(r) );
    return send_frame( canset, &can );
}

static int cmd_dict_dl( can_set_t *canset, size_t n, const char **arg) {
    hard_assert( !(n < 3), "Insufficient arguments\n");
    hard_assert( !(n > 3), "Extra arguments\n");

    uint8_t node = (uint8_t)parse_uhex( arg[0], CANMAT_NODE_MASK );
    const char *param = arg[1];
    const char *val = arg[2];

    canmat_obj_t *obj = canmat_dict_search_name( &canmat_dict402, param );

    hard_assert( obj, "Object `%s' not found\n", param );
    hard_assert( 1 == canset->n, "Can only send on 1 interface\n" );

    uint32_t err;
    canmat_status_t r = canmat_obj_dl_str( canset->cif[0], node, obj, val, &err );
    verbf( 1, "dl status: %s\n", canmat_iface_strerror( canset->cif[0], r ) );

    if( CANMAT_ERR_ABORT == r ) {
        fail("Transfer aborted: '%s' (0x%08x)\n", canmat_sdo_strerror(err), err );
    }
    hard_assert( CANMAT_OK == r, "Failed download: %s\n", canmat_iface_strerror(canset->cif[0],r) );

    return 0;
}

static int cmd_dict_ul( can_set_t *canset, size_t n, const char **arg ) {
    hard_assert( !(n < 2), "Insufficient arguments\n");
    hard_assert( !(n > 2), "Extra arguments\n");

    uint8_t node = (uint8_t)parse_uhex( arg[0], CANMAT_NODE_MASK );
    const char *param = arg[1];

    canmat_obj_t *obj = canmat_dict_search_name( &canmat_dict402, param );
    hard_assert( obj, "Object `%s' not found\n", param );

    hard_assert( 1 == canset->n, "Can only send on 1 interface\n" );

    canmat_scalar_t val;
    canmat_status_t r = canmat_obj_ul( canset->cif[0], node, obj, &val, NULL );
    verbf( 1, "dl status: %s\n", canmat_iface_strerror( canset->cif[0], r ) );

    if( CANMAT_OK == r ) {
        printf( "%s: ", obj->parameter_name );
        canmat_obj_print( stdout, obj, &val );
    } else if ( CANMAT_ERR_ABORT == r ) {

        fprintf( stderr, "Transfer aborted: (0x%08"PRIx32") %s\n",
                 val.u32, canmat_sdo_strerror(val.u32) );
        exit( EXIT_FAILURE );
    } else {
        fprintf(stderr, "Failed upload: %s\n", canmat_iface_strerror(canset->cif[0],r) );
        exit(EXIT_FAILURE);
    }

    return 0;
}

static int cmd_info( can_set_t *canset, size_t n, const char **arg) {
    hard_assert( 0 == n && NULL == arg, "Extra arguments\n");
    for( size_t i = 0; i < canset->n; i++ ) {
        printf("> %s\n", canset->name[i] );
        canmat_status_t r = canmat_iface_print_info(canset->cif[i], stdout);
        hard_assert( CANMAT_OK == r, "Couldn't show info: %s\n", canmat_iface_strerror(canset->cif[i],r) );
    }
    return 0;
}

static int cmd_set( can_set_t *canset, size_t n, const char **arg) {
    hard_assert( !(n < 2), "Insufficient arguments\n");
    hard_assert( !(n > 2), "Extra arguments\n");

    const char *var = arg[0];
    const char *val = arg[1];

    if( 0 == strcasecmp( "bitrate", var ) ||
        0 == strcasecmp( "baud", var ) ||
        0 == strcasecmp( "kbps", var ) )
    {
        unsigned kbps = (unsigned)parse_u( val, 1000 );
        for( size_t i = 0; i < canset->n; i++ ) {
            canmat_status_t r = canmat_iface_set_kpbs( canset->cif[i], kbps );
            hard_assert( CANMAT_OK == r, "Couldn't set birate to %u kbps: %s\n",
                         kbps, canmat_iface_strerror(canset->cif[i],r) );
        }
    } else {
        fprintf( stderr, "Invalid variable: %s\n", var );
        exit(EXIT_FAILURE);
    }

    return 0;
}

static int cmd_nmt( can_set_t *canset, size_t n, const char **arg) {
    hard_assert( !(n < 2), "Insufficient arguments\n");
    hard_assert( !(n > 2), "Extra arguments\n");

    hard_assert( 1 == canset->n, "Only one CAN interface supported\n");

    uint8_t node = (uint8_t)parse_uhex( arg[0], CANMAT_NODE_MASK );
    const char *msg = arg[1];

    canmat_nmt_msg_t nmt;
    if( 0 == strcasecmp( "start", msg ) ) {
        nmt = CANMAT_NMT_START_REMOTE;
    } else if( 0 == strcasecmp( "stop", msg ) ) {
        nmt = CANMAT_NMT_STOP_REMOTE;
    } else if( 0 == strcasecmp( "preop", msg ) ) {
        nmt = CANMAT_NMT_PRE_OP;
    } else if( 0 == strcasecmp( "reset-node", msg ) ) {
        nmt = CANMAT_NMT_RESET_NODE;
    } else if( 0 == strcasecmp( "reset-com", msg ) ) {
        nmt = CANMAT_NMT_RESET_COM;
    } else {
        fprintf(stderr,
                "Unrecognized NMT message: '%s'\n"
                "Wanted one of {start,stop,preop,reset-node,reset-com} for NMT message\n",
                msg);
        exit(EXIT_FAILURE);
    }

    canmat_status_t r = canmat_send_nmt( canset->cif[0], node, nmt );
    hard_assert( CANMAT_OK == r, "Could not send NMT: %s\n", canmat_iface_strerror(canset->cif[0],r) );

    return 0;
}


static int probe_pdo( can_set_t *canset, size_t n, const char **arg) {
    hard_assert( !(n < 1), "Insufficient arguments\n");
    hard_assert( !(n > 1), "Extra arguments\n");
    uint8_t node = (uint8_t)parse_uhex( arg[0], CANMAT_NODE_MASK );

    verbf( 1, "Probing PDOs on node 0x%x\n", node );

    hard_assert( 1 == canset->n, "Only one CAN interface supported\n");

    canmat_status_t r = canmat_probe_pdo( &canmat_dict402, canset->cif[0], node );

    hard_assert( CANMAT_OK == r, "Probing failed: %s\n", canmat_iface_strerror(canset->cif[0], r) );

    return 0;
}

static int cmd_probe( can_set_t *canset, size_t n, const char **arg ) {
    hard_assert( n >= 1, "Insufficient arguments\n");

    if( 0 == strcasecmp( "pdo", arg[0] ) ) {
        return probe_pdo( canset, n-1, arg+1 );
    } else {
        fprintf(stderr, "Don't know how to probe '%s'\n", arg[0]);
        exit(EXIT_FAILURE);
    }
}

static int cmd_map_rpdo( can_set_t *canset, size_t n, const char **arg ) {
    hard_assert( !(n < 3), "Insufficient arguments\n");
    hard_assert( !(n > 3), "Extra arguments\n");
    hard_assert( 1 == canset->n, "Only one CAN interface supported\n");

    uint8_t node = (uint8_t)parse_uhex( arg[0], CANMAT_NODE_MASK );
    uint8_t pdo_num = (uint8_t)parse_u( arg[1], 0xFF );
    const char *param = arg[2];


    canmat_obj_t *obj = canmat_dict_search_name( &canmat_dict402, param );
    hard_assert( NULL != obj, "Unknown parameter: %s\n", param );

    uint32_t err;
    canmat_status_t r = canmat_rpdo_remap( canset->cif[0], node, pdo_num,
                                           1, obj, &err );
    if( CANMAT_ERR_ABORT == r ) {
        fail("Transfer aborted: '%s' (0x%08x)\n", canmat_sdo_strerror(err), err );
    }
    hard_assert( CANMAT_OK == r, "Remap failed: '%s'\n",
                 canmat_iface_strerror(canset->cif[0], r) );

    return 0;
}
