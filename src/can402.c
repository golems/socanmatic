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


#include <syslog.h>

#include "socanmatic.h"
#include "socanmatic/util.h"
#include "socanmatic_private.h"

#include "socanmatic/dict402.h"

#include "sns.h"

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

/**********/
/* PROTOS */
/**********/

static void init( struct can402_cx *cx );
static void run( struct can402_cx *cx );
static void stop( struct can402_cx *cx );
static void parse( struct can402_cx *cx, int argc, char **argv );

static void error( int level , const char fmt[], ...)          ATTR_PRINTF(2,3);

int main( int argc, char ** argv ) {
    struct can402_cx cx;
    memset( &cx, 0, sizeof(cx));

    //parse
    parse( &cx, argc, argv );

    if( opt_verbosity ) {
        for( size_t i = 0; i < cx.drive_set.n; i ++ ) {
            verbf( 1, "node: 0x%x\n", cx.drive_set.drive[i].node_id );
        }
    }

    // init
    init(&cx);

    // run
    run(&cx);

    // stop
    stop(&cx);

    return 0;
}


static void parse( struct can402_cx *cx, int argc, char **argv )
{
    assert( 0 == cx->drive_set.n );
    uint8_t node[CANMAT_NODE_MASK+1];
    for( int c; -1 != (c = getopt(argc, argv, "vhH?Vf:a:n:")); ) {
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
            invalid_arg( optarg );
        }
    }
    if( optind < argc ) {
        invalid_arg(argv[optind]);
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


static void init( struct can402_cx *cx ) {

    enum canmat_status r;

    // init
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_init( cx->drive_set.cif, cx->drive_set.drive[i].node_id, &cx->drive_set.drive[i] );

        verbf( 1, "drive 0x%x: statusword 0x%x, state '%s' (0x%x) \n",
               cx->drive_set.drive[i].node_id, cx->drive_set.drive[i].stat_word,
               canmat_402_state_string( canmat_402_state(&cx->drive_set.drive[i]) ),
               canmat_402_state(&cx->drive_set.drive[i]) );
        hard_assert( CANMAT_OK == r, "can402: couldn't init drive 0x%x: %s\n",
                     cx->drive_set.drive[i].node_id, canmat_iface_strerror( cx->drive_set.cif, r) );
    }

    // set mode
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_set_op_mode( cx->drive_set.cif, &cx->drive_set.drive[i], CANMAT_402_OP_MODE_VELOCITY );
        if( r != CANMAT_OK ) {
            error( LOG_ERR, "can402: couldn't set op mode: '%s'\n",
                   canmat_iface_strerror( cx->drive_set.cif, r) );
            goto FAIL;
        }
    }

    // start
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_start( cx->drive_set.cif, &cx->drive_set.drive[i] );
        if( CANMAT_OK != r ) {
            error( LOG_ERR, "can402: couldn't start drive 0x%x: '%s', state: '%s'\n",
                   cx->drive_set.drive[i].node_id, canmat_iface_strerror( cx->drive_set.cif, r),
                   canmat_402_state_string( canmat_402_state(&cx->drive_set.drive[i]) ) );
            goto FAIL;
        }

        verbf( 1, "drive 0x%x: statusword 0x%x, state '%s' (0x%x) \n",
               cx->drive_set.drive[i].node_id, cx->drive_set.drive[i].stat_word,
               canmat_402_state_string( canmat_402_state(&cx->drive_set.drive[i]) ),
               canmat_402_state(&cx->drive_set.drive[i]) );
    }
    return;

FAIL:
    stop(cx);
    exit(EXIT_FAILURE);
}

static void run( struct can402_cx *cx ) {
    while(1) {
        sleep(1);
        printf("tick\n");
    }
}

static void stop( struct can402_cx *cx ) {
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        canmat_status_t r = canmat_402_stop( cx->drive_set.cif, & (cx->drive_set.drive[i]) );
        if( CANMAT_OK != r ) {
            error( LOG_ERR, "Couldn't stop node 0x%x: %s\n",
                   cx->drive_set.drive[i].node_id,
                   canmat_iface_strerror( cx->drive_set.cif, r) );
        }
    }
}

static void error( int level , const char fmt[], ...) {
    va_list argp;
    va_start( argp, fmt );
    vfprintf( stderr, fmt, argp );
    va_end( argp );
}
