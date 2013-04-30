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
#include <math.h>

#include "socanmatic.h"
#include "socanmatic_private.h"

#include "socanmatic/dict402.h"

#include <sns.h>

struct canmat_402_set {
    struct canmat_iface *cif;
    uint8_t n;
    struct canmat_402_drive drive[CANMAT_NODE_MASK+1];
} canmat_402_set_t;


struct can402_cx {
    struct canmat_402_set drive_set;
    struct sns_msg_motor_ref *msg_ref;

    ach_channel_t chan_ref;
};

const char *opt_chan = "motor-ref";
const char *opt_cmd = NULL;
const char *opt_api = "socketcan";
const char **opt_pos = NULL;
size_t opt_npos = 0;
int opt_rpdo_ctrl = 0;
int opt_rpdo_user = 1;

// FIXME: 1
double opt_vel_factor = 180/M_PI*1000;

/**********/
/* PROTOS */
/**********/

static void init( struct can402_cx *cx );
static void run( struct can402_cx *cx );
static void process( struct can402_cx *cx );
static void stop( struct can402_cx *cx );
static void parse( struct can402_cx *cx, int argc, char **argv );

static unsigned long parse_u( const char *arg, int base, uint64_t max );
//static unsigned long parse_u( const char *arg, uint64_t max );
struct canmat_iface *open_iface( const char *type, const char *name );

//static void error( int level , const char fmt[], ...)          ATTR_PRINTF(2,3);

int main( int argc, char ** argv ) {
    struct can402_cx cx;
    memset( &cx, 0, sizeof(cx));

    //parse
    parse( &cx, argc, argv );

    if( sns_cx.verbosity ) {
        for( size_t i = 0; i < cx.drive_set.n; i ++ ) {
            SNS_LOG( LOG_INFO, "node: 0x%x\n", cx.drive_set.drive[i].node_id );
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
    //static uint8_t node[CANMAT_NODE_MASK+1];
    //static uint8_t rpdo_ctrl[CANMAT_NODE_MASK+1];
    //static uint8_t rpdo_user[CANMAT_NODE_MASK+1];
    for( int c; -1 != (c = getopt(argc, argv, "c:hH?Vf:a:n:R:C:" SNS_OPTSTRING)); ) {
        switch(c) {
            SNS_OPTCASES
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
        case 'a':   /* api  */
            opt_api = optarg;
            break;
        case 'f':   /* interface  */
            cx->drive_set.cif = open_iface( opt_api, optarg );
            break;
        case 'c':   /* interface  */
            opt_chan = strdup(optarg);
            break;
        case 'n':   /* node  */
            SNS_REQUIRE( cx->drive_set.n < CANMAT_NODE_MASK-1, "Too many nodes\n" );
            cx->drive_set.drive[ cx->drive_set.n ].node_id = (uint8_t) parse_u( optarg, 16, CANMAT_NODE_MASK );
            cx->drive_set.drive[ cx->drive_set.n ].rpdo_ctrl = opt_rpdo_ctrl;
            cx->drive_set.drive[ cx->drive_set.n ].rpdo_user = opt_rpdo_user;
            cx->drive_set.n++;
            break;
        case 'C':   /* RPDO-Ctrl  */
            opt_rpdo_ctrl = (uint8_t) parse_u( optarg, 0, 255 );
            if( cx->drive_set.n ) {
                cx->drive_set.drive[ cx->drive_set.n - 1 ].rpdo_ctrl = opt_rpdo_ctrl;
            }
            break;
        case 'R':   /* RPDO-User  */
            opt_rpdo_user = (uint8_t) parse_u( optarg, 0, 255 );
            if( cx->drive_set.n ) {
                cx->drive_set.drive[ cx->drive_set.n - 1 ].rpdo_user = opt_rpdo_user;
            }
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
                  "  -n id,                    Node (multiple allowed)\n"
                  "  -c name,                  Ach Channel name\n"
                  "  -R number,                User RPDO (from zero)\n"
                  "  -C number,                Control RPDO (from zero)\n"
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
            sns_die( 0, "Invalid argument: %s\n", optarg );
        }
    }
    if( optind < argc ) {
        sns_die( 0, "Invalid argument: %s\n", argv[optind] );
    }


    SNS_REQUIRE( cx->drive_set.cif, "can402: missing interface.\nTry `can402 -H' for more information.\n");
    SNS_REQUIRE( cx->drive_set.n, "can402: missing node IDs.\nTry `can402 -H' for more information.\n");

    cx->msg_ref = sns_msg_motor_ref_alloc ( cx->drive_set.n );
}


static void init( struct can402_cx *cx ) {

    sns_start();

    sns_chan_open( &cx->chan_ref, opt_chan, NULL );


    enum canmat_status r;

    // init
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_init( cx->drive_set.cif, cx->drive_set.drive[i].node_id, &cx->drive_set.drive[i] );

        SNS_LOG( LOG_DEBUG, "drive 0x%x: statusword 0x%x, state '%s' (0x%x) \n",
                 cx->drive_set.drive[i].node_id, cx->drive_set.drive[i].stat_word,
                 canmat_402_state_string( canmat_402_state(&cx->drive_set.drive[i]) ),
                 canmat_402_state(&cx->drive_set.drive[i]) );
        SNS_REQUIRE( CANMAT_OK == r, "can402: couldn't init drive 0x%x: %s\n",
                     cx->drive_set.drive[i].node_id, canmat_iface_strerror( cx->drive_set.cif, r) );
    }

    // set mode
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_set_op_mode( cx->drive_set.cif, &cx->drive_set.drive[i], CANMAT_402_OP_MODE_VELOCITY );
        if( r != CANMAT_OK ) {
            sns_die( 0, "can402: couldn't set op mode: '%s'\n",
                   canmat_iface_strerror( cx->drive_set.cif, r) );
            goto FAIL;
        }
    }

    // start
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_start( cx->drive_set.cif, &cx->drive_set.drive[i] );
        if( CANMAT_OK != r ) {
            sns_die( 0, "can402: couldn't start drive 0x%x: '%s', state: '%s'\n",
                     cx->drive_set.drive[i].node_id, canmat_iface_strerror( cx->drive_set.cif, r),
                     canmat_402_state_string( canmat_402_state(&cx->drive_set.drive[i]) ) );
            goto FAIL;
        }

        SNS_LOG( LOG_DEBUG, "drive 0x%x: statusword 0x%x, state '%s' (0x%x) \n",
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
    while( ! sns_cx.shutdown ) {
        size_t frame_size;
        cx->msg_ref->n = cx->drive_set.n;
        const size_t expected_size = sns_msg_motor_ref_size(cx->msg_ref);
        ach_status_t r = ach_get( &cx->chan_ref, cx->msg_ref,
                                  expected_size,
                                  &frame_size, NULL, ACH_O_WAIT | ACH_O_LAST );
        switch(r) {
        case ACH_MISSED_FRAME: /* This is probably OK */
        case ACH_OK:
            // validate
            if( cx->msg_ref->n == cx->drive_set.n &&
                frame_size == expected_size )
            {
                process(cx);
            } else {
                SNS_LOG( LOG_ERR, "bogus message: n: %"PRIu32", expected: %"PRIu32", "
                         "size: %"PRIuPTR", expected: %"PRIuPTR"\n",
                         cx->msg_ref->n, cx->drive_set.n,
                         frame_size, expected_size );
            }
            break;
        case ACH_TIMEOUT:
            /* TODO: halt and post feedback */

            break;
            /* Really bad things we just give up on */
        case ACH_BUG:
        case ACH_CORRUPT:
            sns_die( 0, "ach_get failed badly, aborting: '%s'\n", ach_result_to_string(r) );
            break;
        default:
            SNS_LOG( LOG_ERR, "ach_get failed: '%s'\n", ach_result_to_string(r) );

        }
    }
}

static void process( struct can402_cx *cx ) {
    if( SNS_LOG_PRIORITY(LOG_DEBUG + 1) ) {
        sns_msg_motor_ref_dump( stderr, cx->msg_ref );
    }
    // process message


    // Check if expired

    // TODO: op mode switching
    switch( cx->msg_ref->mode ) {
    case SNS_MOTOR_MODE_VEL:
        // TODO: check that mode is supported
        for( size_t i = 0; i < cx->msg_ref->n; i ++ ) {
            double val = cx->msg_ref->u[i] * opt_vel_factor;
            int16_t vl_target = 0;
            // clamp value
            if( val > INT16_MAX ) vl_target = INT16_MAX;
            else if (val < INT16_MIN ) vl_target = INT16_MIN;
            else vl_target = (int16_t) val;
            // build can frame
            struct can_frame can;
            can.can_dlc = 2;
            can.can_id = CANMAT_RPDO_COBID( cx->drive_set.drive[i].node_id,
                                            cx->drive_set.drive[i].rpdo_user );
            can.data[0] = (uint8_t)(vl_target & 0xFF);
            can.data[1] = (uint8_t)((vl_target >> 8) & 0xFF);
            // send frame
            canmat_status_t cr = canmat_iface_send( cx->drive_set.cif, &can );
            SNS_CHECK( CANMAT_OK == cr, LOG_ERR, 0, "Couldn't send PDO: %s\n",
                       canmat_iface_strerror( cx->drive_set.cif, cr) );

        }
        break;
    default:
        SNS_LOG( LOG_ERR, "unhandled op mode in motor_ref msg: '%d'\n", cx->msg_ref->mode );
    }
}


static void stop( struct can402_cx *cx ) {
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        canmat_status_t r = canmat_402_stop( cx->drive_set.cif, & (cx->drive_set.drive[i]) );
        if( CANMAT_OK != r ) {
            SNS_LOG( LOG_ERR, "Couldn't stop node 0x%x: %s\n",
                     cx->drive_set.drive[i].node_id,
                     canmat_iface_strerror( cx->drive_set.cif, r) );
        }
    }
    sns_end();
}

static unsigned long parse_u( const char *arg, int base, uint64_t max ) {
    char *endptr;
    errno = 0;
    unsigned long u  = strtoul( arg, &endptr, base );

    SNS_REQUIRE( 0 == errno, "Invalid hex argument: %s (%s)\n", arg, strerror(errno) );
    SNS_REQUIRE( u <= max, "Argument %s too big\n", arg );

    return u;
}


/* static unsigned long parse_u( const char *arg, uint64_t max ) { */
/*     char *endptr; */
/*     errno = 0; */
/*     unsigned long u  = strtoul( arg, &endptr, 0 ); */

/*     SNS_REQUIRE( 0 == errno, "Invalid hex argument: %s (%s)\n", arg, strerror(errno) ); */
/*     SNS_REQUIRE( u <= max, "Argument %s too big\n", arg ); */

/*     return u; */
/* } */

struct canmat_iface *open_iface( const char *type, const char *name ) {
    struct canmat_iface *cif = canmat_iface_new( type );
    SNS_REQUIRE( cif, "Couldn't create interface of type: %s\n", type );

    canmat_status_t r =  canmat_iface_open( cif, name);
    SNS_REQUIRE( CANMAT_OK == r, "Couldn't open: %s, %s\n",
                 name, canmat_iface_strerror( cif, r ) );

    SNS_LOG( LOG_INFO, "Opened interface %s, type %s\n", name, type);

    return cif;
}
