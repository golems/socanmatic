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
#include "socanmatic_private.h"


#ifdef __GNUC__
#define ATTR_PRINTF(m,n) __attribute__((format(printf, m, n)))
#else
#define ATTR_PRINTF(m,n)
#endif


const char *opt_cmd = NULL;
const char *opt_verbosity = 0;

uint16_t opt_canid = 0;
uint8_t opt_can_dlc = 0;
uint8_t opt_can_data[8];

canmat_sdo_msg_t opt_sdo = {0};

typedef int (*cmd_fun_t)(void);

int (*opt_command)(void) = NULL;

uint8_t opt_node;
const char *opt_dict_param_name;
const char *opt_dict_param_value;

char const ** opt_ifaces = NULL;
size_t opt_n_ifaces;

void hard_assert( _Bool test , const char fmt[], ...)          ATTR_PRINTF(2,3);

int cmd_send( void );
int cmd_dump( void );
int cmd_ul( void );
int cmd_ul_resp( void );
int cmd_dl( void );
int cmd_dl_resp( void );
int cmd_dict_dl( void );
int cmd_dict_ul( void );
int cmd_display( void );


typedef struct can_set {
    size_t n;
    int *fd;
    const char **iface;
    struct pollfd *pfd;
} can_set_t;

/***********/
/* HELPERS */
/***********/

void hard_assert( _Bool test , const char fmt[], ...)  {
    if( ! test ) {
        va_list argp;
        va_start( argp, fmt );
        vfprintf( stderr, fmt, argp );
        va_end( argp );
        exit(EXIT_FAILURE);
    }
}

int try_open( const char *iface) {
    int s = canmat_can_open( iface );
    if( opt_verbosity ) {
        if( s >= 0 ) {
            fprintf(stderr, "Opened iface %s\n", iface);
        }
    }
    return s;
}

void can_open( can_set_t *canset ) {
    if( opt_ifaces ) {

        canset->fd = (int*)malloc(sizeof(int)*opt_n_ifaces);
        canset->n = opt_n_ifaces;
        canset->iface = opt_ifaces;

        for( size_t i = 0; i < canset->n; i ++ ) {
            canset->fd[i] = try_open( canset->iface[i] );
            hard_assert( canset->fd[i] >= 0, "Couldn't open %s (%d): %s\n",
                         canset->iface[i],
                         canset->fd[i], strerror(errno) );
        }
    } else {
        // try some defaults
        canset->n = 1 ;
        canset->iface = (const char**)malloc(sizeof(char*));
        canset->fd = (int*)malloc(sizeof(int)*1);

        if( (canset->fd[0] = try_open( "can0" ))  >= 0 ) {
            canset->iface[0] = "can0";
        } else if( (canset->fd[0] = try_open( "vcan0" ))  >= 0 ) {
            canset->iface[0] = "vcan0";
        } else {
            hard_assert( 0, "Couldn't open CAN (%d): %s\n", canset->fd[0], strerror(errno) );
        }
    }

    // allocate poll fds
    canset->pfd = (struct pollfd*) malloc( canset->n * sizeof(canset->pfd[0]) );
    memset( canset->pfd, 0, canset->n*sizeof(canset->pfd[0]) );
    for( size_t i = 0; i < canset->n; i ++ ) {
        canset->pfd[i].fd = canset->fd[i];
    }

}

static void set_cmd( int (*cmd_fun)(void) ) {
    if( NULL == opt_command ) {
        opt_command = cmd_fun;
    }else {
        fprintf(stderr, "Can only specify one command\n");
        exit(EXIT_FAILURE);
    }
}

/***************/
/* ARG PARSING */
/***************/

unsigned long parse_uhex( const char *arg, uint64_t max ) {
    char *endptr;
    errno = 0;
    unsigned long u  = strtoul( arg, &endptr, 16 );

    hard_assert( 0 == errno, "Invalid hex argument: %s (%s)\n", arg, strerror(errno) );
    hard_assert( u <= max, "Argument %s too big\n", arg );

    return u;
}

void invalid_arg( const char *arg ) {
    hard_assert( 0, "Invalid argument: %s\n", arg );
}

void posarg_send( const char *arg, int i ) {
    if( 1 == i) {
        opt_canid = (uint16_t)parse_uhex( arg, 0x7FF );
    } else if (i > 1 && i < 10 ) {
        opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
    } else {
        invalid_arg( arg );
    }
}

void posarg_dict( const char *arg, int i ) {
    switch(i) {
    case 1:
        opt_node = (uint8_t)parse_uhex( arg, CANMAT_NODE_MASK );
        break;
    case 2:
        opt_dict_param_name = strdup( arg );
        break;
    case 3:
        if( opt_command == cmd_dict_dl ) {
            opt_dict_param_name = strdup( arg );
        } else {
            invalid_arg( arg );
        }
        break;
    default:
        invalid_arg( arg );
    }
}

void posarg_sdo( const char *arg, int i ) {
    switch(i) {
    case 1:
        opt_sdo.node = (uint8_t)parse_uhex( arg, CANMAT_NODE_MASK );
        break;
    case 2:
        opt_sdo.index = (uint16_t)parse_uhex( arg, 0xFFFF );
        break;
    case 3:
        opt_sdo.subindex = (uint8_t)parse_uhex( arg, 0xFF );
        break;
    default:
        if( (cmd_dl == opt_command || cmd_ul_resp == opt_command) &&
            i >= 4 && i <= 8 ) {
            assert( opt_sdo.length == i - 4 );
            opt_sdo.data[ opt_sdo.length++ ] = (uint8_t)parse_uhex( arg, 0xFF );
        } else {
            invalid_arg( arg );
        }
    }
}

cmd_fun_t posarg_cmd( const char *arg ) {
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

void posarg( const char *arg, int i ) {
    if( 0 == i ) {
        if( opt_verbosity ) {
            fprintf( stderr, "Setting command: %s\n", arg );
        }
        set_cmd( posarg_cmd(arg) );
    } else if( cmd_send == opt_command ) {
        posarg_send( arg, i );
    } else if( cmd_dl == opt_command ||
               cmd_ul == opt_command ||
               cmd_dl_resp == opt_command ||
               cmd_ul_resp == opt_command ) {
        posarg_sdo( arg, i );
    } else if( cmd_dict_dl == opt_command ||
               cmd_dict_ul == opt_command ) {
        posarg_dict( arg, i );
    } else {
        invalid_arg( arg );
    }
}

int main( int argc, char ** argv ) {

    int c, i = 0;
    while( (c = getopt( argc, argv, "vhH?Vf:")) != -1 ) {
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
        case 'f':   /* interface  */
            opt_ifaces = (const char**)realloc( (void*)opt_ifaces, sizeof(char*) * ++opt_n_ifaces );
            opt_ifaces[opt_n_ifaces-1] = strdup(optarg);
            break;
        case '?':   /* help     */
        case 'h':
        case 'H':
            puts( "Usage: canmat [OPTION...] [dump]\n"
                  "Shell tool for CANopen\n"
                  "\n"
                  "Options:\n"
                  "  -v,                       Make output more verbose\n"
                  "  -f interface,             CAN interface (multiple allowed)\n"
                  "  -?,                       Give program help list\n"
                  "  -V,                       Print program version\n"
                  "\n"
                  "Examples:\n"
                  "  canmat dump                                  Print CAN messages to standard output\n"
                  "  canmat display                               Pretty-print CAN messages to standard output\n"
                  "  canmat send id b0 ... b7                     Send a can message (values in hex)\n"
                  "  canmat dict-dl node param-name value         Download SDO to node\n"
                  "  canmat dict-ul node param-name               Upload SDO from node\n"
                  "  canmat dl node idx subidx bytes-or-val       Download SDO to node\n"
                  "  canmat dl-resp node idx subidx               Simulate node download response\n"
                  "  canmat ul node idx subidx                    Upload SDO from node\n"
                  "  canmat ul-resp node idx subidx bytes-or-val  Simulate node upload response\n"
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

    return opt_command();

    return 0;
}

/************/
/* COMMANDS */
/************/

static void pollin_raw( struct can_frame *can ) {
    canmat_dump_frame( stdout, can );
}

static void pollin_display( struct can_frame *can ) {
    canmat_display( &canmat_dict402, can );
}

int cmd_pollin( void (printer)(struct can_frame*) ) {
    // open fds
    can_set_t canset;
    can_open(&canset);

    // set events
    for( size_t i = 0; i < canset.n; i ++ ) {
        canset.pfd[i].events = POLLIN;
    }

    // FIXME: if network goes down and up, we don't detect till poll
    // sees new data.  Then read() fails

    while(1) {
        // wait for data
        int rp = poll( canset.pfd, canset.n , -1 );
        hard_assert(rp > 0, "Couldn't poll interfaces (%d): %s (%d)\n", rp, strerror(errno), errno);

        // print all available data
        for( size_t i = 0; i < canset.n; i ++ ) {
            hard_assert( 0 == ((canset.pfd[i].revents & POLLERR) ||
                               (canset.pfd[i].revents & POLLHUP) ||
                               (canset.pfd[i].revents & POLLNVAL) ),
                         "Error on iface %s\n", canset.iface[i] );
            if( canset.pfd[i].revents & POLLIN ) {
                struct can_frame can;
                // read the actual message
                ssize_t r = canmat_can_recv( canset.fd[i], &can );
                if( r > 0 ) {
                    fprintf( stdout, "%s: ", canset.iface[i] );
                    printer( &can );
                } else {
                    perror("Couldn't recv frame");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}


int cmd_dump( void ) {
    cmd_pollin( pollin_raw );
    return 0;
}

int cmd_display( void ) {
    cmd_pollin( pollin_display );
    return 0;
}

int send_frame( struct can_frame *can ) {
    if(opt_verbosity) {
        fprintf(stderr, "Sending ");
        canmat_dump_frame( stdout, can );
    }

    can_set_t canset;
    can_open(&canset);

    for( size_t i = 0; i < canset.n; i ++ ) {
        ssize_t r = canmat_can_send( canset.fd[i], can );
        if( ! canmat_can_ok(r) ) {
            fprintf( stderr, "Couldn't send frame on %s: %s\n",
                     opt_ifaces[i], strerror(errno) );
        }
    }
    return 0;
}

int query_sdo() {
    can_set_t canset;
    can_open(&canset);

    for( size_t i = 0; i < canset.n; i ++ ) {
        canmat_sdo_msg_t resp;

        ssize_t r = canmat_sdo_query( canset.fd[i], &opt_sdo, &resp );

        if( ! canmat_can_ok(r) ) {
            fprintf( stderr, "Couldn't send frame: %s\n", strerror(errno) );
        }
        canmat_sdo_print( stdout, &resp );
    }

    return 0;
}

int cmd_send( void ) {
    struct can_frame can;
    can.can_id = opt_canid;
    can.can_dlc = opt_can_dlc;
    memcpy(can.data, opt_can_data, opt_can_dlc);

    return send_frame( &can );
}

int cmd_ul( void ) {
    opt_sdo.cmd.ccs = CANMAT_EX_UL;
    opt_sdo.cmd.e = 1;
    opt_sdo.cmd.n = 0;
    opt_sdo.cmd.s = 0;

    return query_sdo();
}

int cmd_ul_resp( void ) {
    hard_assert( opt_sdo.length > 0 && opt_sdo.length < 5,
                 "Invalid data length\n" );

    struct can_frame can;

    opt_sdo.cmd.ccs = CANMAT_EX_UL;
    opt_sdo.cmd.e = 1;
    opt_sdo.cmd.n = (unsigned char)((4 - opt_sdo.length) & 0x3);
    opt_sdo.cmd.s = 1;

    canmat_sdo2can( &can, &opt_sdo, 1 );
    return send_frame( &can );
}

int cmd_dl( void ) {
    hard_assert( opt_sdo.length > 0 && opt_sdo.length < 5,
                 "Invalid data length\n" );

    canmat_sdo_set_ex_dl( &opt_sdo,
                         opt_sdo.node, opt_sdo.index, opt_sdo.subindex );

    return query_sdo();
}

int cmd_dl_resp( void ) {
    hard_assert( 0 == opt_sdo.length,
                 "Invalid data length\n" );

    opt_sdo.cmd.ccs = CANMAT_EX_DL;
    opt_sdo.cmd.e = 1;
    opt_sdo.cmd.n = 0;
    opt_sdo.cmd.s = 0;

    struct can_frame can;
    canmat_sdo2can( &can, &opt_sdo, 1 );
    return send_frame( &can );
}

int cmd_dict_dl() {
    canmat_obj_t *obj = canmat_dict_search_name( &canmat_dict402, opt_dict_param_name );

    hard_assert( obj, "Object `%s' not found\n", opt_dict_param_name );

    can_set_t canset;
    can_open(&canset);
    hard_assert( 1 == canset.n, "Can only send on 1 interface\n" );

    canmat_status_t r = canmat_obj_dl_str( canset.fd[0], opt_node, obj, opt_dict_param_value );
    hard_assert( CANMAT_OK == r, "Failed download: %s\n", canmat_strerror(r) );

    return 0;
}

int cmd_dict_ul() {
    canmat_obj_t *obj = canmat_dict_search_name( &canmat_dict402, opt_dict_param_name );
    hard_assert( obj, "Object `%s' not found\n", opt_dict_param_name );

    can_set_t canset;
    can_open(&canset);
    hard_assert( 1 == canset.n, "Can only send on 1 interface\n" );

    canmat_scalar_t val;
    canmat_status_t r = canmat_obj_ul( canset.fd[0], opt_node, obj, &val );
    hard_assert( CANMAT_OK == r, "Failed upload: %s\n", canmat_strerror(r) );
    switch(obj->data_type) {

    case CANMAT_DATA_TYPE_INTEGER8:
        printf("%"PRId8"\n", val.i8);
        break;
    case CANMAT_DATA_TYPE_INTEGER16:
        printf("%"PRId16"\n", val.i16);
        break;
    case CANMAT_DATA_TYPE_INTEGER32:
        printf("%"PRId32"\n", val.i32);
        break;
    case CANMAT_DATA_TYPE_UNSIGNED8:
        printf("0x%"PRIx8"\n", val.u8);
        break;
    case CANMAT_DATA_TYPE_UNSIGNED16:
        printf("0x%"PRIx16"\n", val.u16);
        break;
    case CANMAT_DATA_TYPE_UNSIGNED32:
        printf("0x%"PRIx32"\n", val.u32);
        break;
    default: return CANMAT_ERR_PARAM;
    }

    return 0;
}
