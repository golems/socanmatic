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

#include "socanmatic/dict402.h"

#include "sns.h"

#ifdef __GNUC__
#define ATTR_PRINTF(m,n) __attribute__((format(printf, m, n)))
#else
#define ATTR_PRINTF(m,n)
#endif

struct canmat_402_set {
    struct canmat_iface *cif;
    uint8_t n;
    struct canmat_402_drive *drive;
} canmat_402_set_t;


struct can402_cx {
    struct canmat_402_set drive_set;
    struct sns_msg_motor_ref *ref;
};

const char *opt_cmd = NULL;
int opt_verbosity = 0;
const char *opt_api = "socketcan";

const char **opt_pos = NULL;
size_t opt_npos = 0;

void hard_assert( _Bool test , const char fmt[], ...)          ATTR_PRINTF(2,3);


static void verbf( int level , const char fmt[], ...)          ATTR_PRINTF(2,3);

typedef int (*cmd_fun_t)(struct canmat_iface*,size_t, const char**);
cmd_fun_t opt_command = NULL;

/**********/
/* PROTOS */
/**********/

//static void init( struct can402_cx *cx );
//static void run( struct can402_cx *cx );
//static void stop( struct can402_cx *cx );
static void parse( struct can402_cx *cx, int argc, char **argv );

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


static void verbf( int level , const char fmt[], ...) {
    if( level <= opt_verbosity ) {
        fputs("# ", stderr);
        va_list argp;
        va_start( argp, fmt );
        vfprintf( stderr, fmt, argp );
        va_end( argp );
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


unsigned long parse_u( const char *arg, uint64_t max ) {
    char *endptr;
    errno = 0;
    unsigned long u  = strtoul( arg, &endptr, 0 );

    hard_assert( 0 == errno, "Invalid hex argument: %s (%s)\n", arg, strerror(errno) );
    hard_assert( u <= max, "Argument %s too big\n", arg );

    return u;
}

void invalid_arg( const char *arg ) {
    hard_assert( 0, "Invalid argument: %s\n", arg );
}

cmd_fun_t posarg_cmd( const char *arg ) {
    static const struct  {
        const char *name;
        cmd_fun_t fun;
    } cmds[] = { {NULL, NULL} };
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
        verbf( 1, "Setting command: %s\n", arg );
        opt_command = posarg_cmd(arg);
    } else {
        opt_pos = (const char**) realloc( opt_pos, sizeof(opt_pos[0]) * (opt_npos+1) );
        opt_pos[opt_npos++] = arg;
    }
}

struct canmat_iface *open_iface( const char *type, const char *name ) {
    struct canmat_iface *cif = canmat_iface_new( type );
    hard_assert( cif, "Couldn't create interface of type: %s\n", type );

    canmat_status_t r =  canmat_iface_open( cif, name);
    hard_assert( CANMAT_OK == r, "Couldn't open: %s, %s\n",
                 name, canmat_iface_strerror( cif, r ) );

    verbf( 1, "Opened interface %s, type %s\n", name, type);

    return cif;
}

int main( int argc, char ** argv ) {
    //struct canmat_402_set drive_set = {0};
    struct can402_cx cx;
    memset( &cx, 0, sizeof(cx));

    // argument pargs
    parse( &cx, argc, argv );


    if( opt_verbosity ) {
        for( size_t i = 0; i < cx.drive_set.n; i ++ ) {
            verbf( 1, "node: 0x%x\n", cx.drive_set.drive[i].node_id );
        }
    }



    exit(0);
    //hard_assert( opt_command, "can402: missing command.\nTry `can402 -H' for more information.\n");




    struct canmat_402_drive drive;

    // init

    enum canmat_status r = canmat_402_init( cx.drive_set.cif, 0xa, &drive );

    verbf( 1, "drive 0x%x: statusword 0x%x, state '%s' (0x%x) \n", drive.node_id, drive.stat_word,
           canmat_402_state_string( canmat_402_state(&drive) ), canmat_402_state(&drive) );
    hard_assert( CANMAT_OK == r, "can402: couldn't init drive 0x%x: %s\n",
                 drive.node_id, canmat_iface_strerror( cx.drive_set.cif, r) );

    // start
    r = canmat_402_start( cx.drive_set.cif, &drive );
    hard_assert( CANMAT_OK == r, "can402: couldn't start drive 0x%x: '%s', state: '%s'\n",
                 drive.node_id, canmat_iface_strerror( cx.drive_set.cif, r),
                 canmat_402_state_string( canmat_402_state(&drive) ) );

    verbf( 1, "drive 0x%x: statusword 0x%x, state '%s' (0x%x) \n", drive.node_id, drive.stat_word,
           canmat_402_state_string( canmat_402_state(&drive) ), canmat_402_state(&drive) );

    // set mode
    r = canmat_402_dl_modes_of_operation( cx.drive_set.cif, drive.node_id, CANMAT_402_OP_MODE_VELOCITY,
                                          &(drive.abort_code) );
    hard_assert( CANMAT_OK == r, "can402: couldn't set op mode: '%s'\n",
                 canmat_iface_strerror( cx.drive_set.cif, r) );

    r = canmat_402_dl_controlword( cx.drive_set.cif, drive.node_id, 0x7F,
                                   &(drive.abort_code) );
    hard_assert( CANMAT_OK == r, "can402: couldn't set control word: '%s'\n",
                 canmat_iface_strerror( cx.drive_set.cif, r) );

    //return opt_command(&canset, opt_npos, opt_pos);

    return 0;
}


static void parse( struct can402_cx *cx, int argc, char **argv )
{
    assert( 0 == cx->drive_set.n );
    uint8_t node[CANMAT_NODE_MASK+1];
    {
        int c, i = 0;
        while( (c = getopt( argc, argv, "vhH?Vf:a:n:")) != -1 ) {
            switch(c) {
            case 'V':   /* version     */
                puts( "can402 " PACKAGE_VERSION "\n"
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
                cx->drive_set.cif = open_iface( opt_api, optarg );
                break;
            case 'n':   /* node  */
                hard_assert( cx->drive_set.n < CANMAT_NODE_MASK-1, "Too many nodes\n" );
                node[cx->drive_set.n] = (uint8_t) parse_uhex( optarg, CANMAT_NODE_MASK );
                cx->drive_set.n++;
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
                      "  -f interface,             CAN interface\n"
                      "  -f n,                     Node (multiple allowed)\n"
                      "  -?,                       Give program help list\n"
                      "  -V,                       Print program version\n"
                      "\n"
                      "Examples:\n"
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
    }


    hard_assert( cx->drive_set.cif, "can402: missing interface.\nTry `can402 -H' for more information.\n");
    hard_assert( cx->drive_set.n, "can402: missing node IDs.\nTry `can402 -H' for more information.\n");

    // create drive struct
    cx->drive_set.drive = (struct canmat_402_drive*) malloc( cx->drive_set.n * sizeof(struct canmat_402_drive) );
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        cx->drive_set.drive[i].node_id = node[i];
    }

    cx->ref = sns_msg_motor_ref_alloc ( cx->drive_set.n );

}

/* static void init( struct canmat_402_set *drive_set ) { */

/* } */

/* static void run( struct canmat_402_set *drive_set ) { */

/* } */

/* static void stop( struct canmat_402_set *drive_set ) { */

/* } */
