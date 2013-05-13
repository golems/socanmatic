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


/* CAN402: Driver daemon for CANopen servo drives
 *
 * Reads reference commands from Ach channel, sends to drives.
 * Reads feedback from drives, posts to Ach channel
 *
 * PDO Usage:
 *   - Needs two RPDOs, one for control word and one for reference value
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
#include <pthread.h>
#include <signal.h>


#include <syslog.h>
#include <math.h>

#include "socanmatic.h"
#include "socanmatic_private.h"

#include "socanmatic/dict402.h"



static inline struct timespec sns_time_add_ns( struct timespec ts, int64_t ns ) {
    int64_t ns1 = ns + ts.tv_nsec;
    struct timespec r = { .tv_nsec = ns1 % 1000000000,
                          .tv_sec = ts.tv_sec + ns1 / 1000000000 };
    return r;
}


#define SNS_LOG( x, ... ) (fprintf(stderr, __VA_ARGS__))
#define sns_die( x, ... ) fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE);
#define SNS_REQUIRE( test, ... ) if( !(test) ) { sns_die(0, __VA_ARGS__); }

struct canmat_402_set {
    struct canmat_iface *cif;
    uint8_t n;
    struct canmat_402_drive drive[CANMAT_NODE_MASK+1];
} canmat_402_set_t;


struct can402_cx {
    struct canmat_402_set drive_set;
    struct sns_msg_motor_ref *msg_ref;
    struct sns_msg_motor_state *msg_state;

    enum canmat_402_op_mode op_mode;
    _Bool halt;

    struct timespec now;
};

const char *opt_chan_ref = "motor-ref";
const char *opt_chan_state = "motor-state";
const char *opt_cmd = NULL;
const char *opt_api = "socketcan";
const char **opt_pos = NULL;
size_t opt_npos = 0;
int opt_rpdo_ctrl = 0;
int opt_rpdo_user = 1;

// FIXME: 1
double opt_vel_factor = 180/M_PI*1000;
double opt_pos_factor = 180/M_PI*1000;

sig_atomic_t sig_shutdown = 0;

/**********/
/* PROTOS */
/**********/

static void init( struct can402_cx *cx );
static void run( struct can402_cx *cx );
static void stop( struct can402_cx *cx );
static void parse( struct can402_cx *cx, int argc, char **argv );

/* Feedback thread */
static void feedback_recv( struct can402_cx *cx );
static void *feedback_recv_start( void *cx );

/* Called from main thread */
static void update_feedback( struct can402_cx *cx );

static unsigned long parse_u( const char *arg, int base, uint64_t max );
//static unsigned long parse_u( const char *arg, uint64_t max );
struct canmat_iface *open_iface( const char *type, const char *name );

//static void error( int level , const char fmt[], ...)          ATTR_PRINTF(2,3);

int main( int argc, char ** argv ) {
    struct can402_cx cx;
    memset( &cx, 0, sizeof(cx));

    // defaults
    cx.op_mode = CANMAT_402_OP_MODE_VELOCITY;
    cx.halt = 1;

    //parse
    parse( &cx, argc, argv );

    for( size_t i = 0; i < cx.drive_set.n; i ++ ) {
        SNS_LOG( LOG_INFO, "node: 0x%x\n", cx.drive_set.drive[i].node_id );
    }

    // init
    init(&cx);

    for( size_t i = 0; i < cx.drive_set.n; i ++ ) {
        SNS_LOG( LOG_INFO, "limits 0x%x: %f (%f) -- %f (%f)\n",
                 cx.drive_set.drive[i].node_id,
                 cx.drive_set.drive[i].pos_min_soft,
                 cx.drive_set.drive[i].pos_min_hard,
                 cx.drive_set.drive[i].pos_max_soft,
                 cx.drive_set.drive[i].pos_max_hard );
    }

    // run
    pthread_t feedback_thread;
    if( pthread_create( &feedback_thread, NULL, feedback_recv_start, &cx ) ) {
        sns_die( 0, "Couldn't create feedback thread: %s\n", strerror(errno) );
        exit(EXIT_FAILURE);
    }
    run(&cx);

    // stop
    if( pthread_join( feedback_thread, NULL ) ) {
        SNS_LOG( LOG_ERR, "Couldn't join feedback thread: %s\n", strerror(errno) );
    }
    stop(&cx);

    return 0;
}


static void parse( struct can402_cx *cx, int argc, char **argv )
{
    assert( 0 == cx->drive_set.n );
    for( int c; -1 != (c = getopt(argc, argv, "c:s:hH?Vf:a:n:R:C:" )); ) {
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
        case 'a':   /* api  */
            opt_api = optarg;
            break;
        case 'f':   /* interface  */
            cx->drive_set.cif = open_iface( opt_api, optarg );
            break;
        case 'c':   /* reference channel  */
            opt_chan_ref = strdup(optarg);
            break;
        case 's':   /* state channel  */
            opt_chan_state = strdup(optarg);
            break;
        case 'n':   /* node  */
            SNS_REQUIRE( cx->drive_set.n < CANMAT_NODE_MASK-1, "Too many nodes\n" );
            cx->drive_set.drive[ cx->drive_set.n ].node_id = (uint8_t) parse_u( optarg, 16, CANMAT_NODE_MASK );
            cx->drive_set.drive[ cx->drive_set.n ].rpdo_ctrl = opt_rpdo_ctrl;
            cx->drive_set.drive[ cx->drive_set.n ].rpdo_user = opt_rpdo_user;
            cx->drive_set.drive[ cx->drive_set.n ].pos_factor = opt_pos_factor;
            cx->drive_set.drive[ cx->drive_set.n ].vel_factor = opt_vel_factor;
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


    for( size_t i = 0; i < cx->drive_set.n; i++ ) {
        SNS_REQUIRE( cx->drive_set.drive[i].rpdo_user != cx->drive_set.drive[i].rpdo_ctrl,
                     "Need distinct user and controlword PDOs.  Drive %"PRIuPTR" using RPDO %d for both\n",
                     i, cx->drive_set.drive[i].rpdo_user );
    }

}




static void sighandler (int sig, siginfo_t *siginfo, void *context)
{
    (void) context;
    (void) siginfo;
    SNS_LOG( LOG_DEBUG, "Received Signal: %d, Sending PID: %ld, UID: %ld\n",
             sig, (long)siginfo->si_pid, (long)siginfo->si_uid);

    switch( sig) {
    case SIGINT:
    case SIGTERM:
        sig_shutdown = 1;
    }
}


static void init( struct can402_cx *cx ) {

    // install signal handler
    {
        struct sigaction act;
        memset(&act, 0, sizeof(act));

        act.sa_sigaction = &sighandler;

        /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field,
           not sa_handler. */
        act.sa_flags = SA_SIGINFO;

        int r;
        r = sigaction(SIGTERM, &act, NULL);
        SNS_REQUIRE( 0 == r, "sigaction failed: %s", strerror(errno) );

        r = sigaction(SIGINT, &act, NULL);
        SNS_REQUIRE( 0 == r, "sigaction failed: %s", strerror(errno) );
    }



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

    // Map the control
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        const struct canmat_obj *obj[1] =  {CANMAT_402_OBJ_CONTROLWORD};
        r = canmat_pdo_remap( cx->drive_set.cif, cx->drive_set.drive[i].node_id,
                              (uint8_t)(cx->drive_set.drive[i].rpdo_ctrl), CANMAT_DL,
                              CANMAT_PDO_TRANSMISSION_TYPE_EVENT_DRIVEN, -1, -1,
                              1, obj, &cx->drive_set.drive[i].abort_code );
        if( r != CANMAT_OK ) {
            SNS_LOG( LOG_EMERG, "can402: couldn't map control rpdo: '%s'\n",
                     canmat_iface_strerror( cx->drive_set.cif, r) );
            goto FAIL;
        }
    }

    // map the feedback
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        const canmat_obj_t *objs[2] = { CANMAT_402_OBJ_POSITION_ACTUAL_VALUE,
                                        CANMAT_402_OBJ_VELOCITY_ACTUAL_VALUE };
        r = canmat_pdo_remap( cx->drive_set.cif, cx->drive_set.drive[i].node_id,
                              (uint8_t)(cx->drive_set.drive[i].tpdo_user), CANMAT_UL,
                              0xFE, -1, 10,
                              2, objs, &cx->drive_set.drive[i].abort_code );
        if( r != CANMAT_OK ) {
            SNS_LOG( LOG_EMERG, "can402: couldn't map control rpdo: '%s'\n",
                     canmat_iface_strerror( cx->drive_set.cif, r) );
            goto FAIL;
        }
    }


    // set mode and PDOs
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_set_op_mode( cx->drive_set.cif, &cx->drive_set.drive[i], CANMAT_402_OP_MODE_VELOCITY );
        if( r != CANMAT_OK ) {
            SNS_LOG( LOG_EMERG, "can402: couldn't set op mode: '%s'\n",
                   canmat_iface_strerror( cx->drive_set.cif, r) );
            goto FAIL;
        }
    }

    // start
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_start( cx->drive_set.cif, &cx->drive_set.drive[i] );
        if( CANMAT_OK != r ) {
            SNS_LOG( LOG_EMERG, "can402: couldn't start drive 0x%x: '%s', state: '%s'\n",
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
static double pos_limit( struct canmat_402_drive *drive, double val ) {
    double pos = drive->actual_pos;
    double off = 0;
    if ( pos > drive->pos_max_soft  ) {
        SNS_LOG(LOG_DEBUG, "limit+ 0x%x, %f > %f \n",
                 drive->node_id,
                 pos, drive->pos_max_soft );
        if( pos > drive->pos_max_hard ) {
            return -HUGE_VAL;
        } else {
            off = ((pos - drive->pos_max_soft) /
                   (pos - drive->pos_max_hard));
        }
    } else if ( pos < drive->pos_min_soft  ) {
        SNS_LOG( LOG_DEBUG, "limit- 0x%x %f < %f \n", drive->node_id,
                 pos, drive->pos_min_soft );
        if( pos < drive->pos_min_hard ) {
            return HUGE_VAL;
        } else {
            off = -((pos - drive->pos_min_soft) /
                    (pos - drive->pos_min_hard));
        }
    }

    return val + off;
}



static void run( struct can402_cx *cx ) {
    clock_gettime( CLOCK_MONOTONIC, &cx->now );
    double T = 0;
    while( ! sig_shutdown ) {
        update_feedback(cx);
        for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
            double val = pos_limit( &cx->drive_set.drive[i], .1*sin(T) );
            val *= cx->drive_set.drive[i].vel_factor;
            int16_t vl_target = 0;
            if( val > INT16_MAX ) {
                vl_target = INT16_MAX;
                SNS_LOG(LOG_DEBUG, "clamp+ %f -> %d 0x%x\n", val, vl_target, cx->drive_set.drive[i].node_id);
            } else if (val < INT16_MIN ) {
                vl_target = INT16_MIN;
                SNS_LOG(LOG_DEBUG, "clamp- %f -> %d 0x%x\n", val, vl_target, cx->drive_set.drive[i].node_id);
            } else vl_target = (int16_t) val;
            // check if update necessary to save bandwidth
            if( vl_target != cx->drive_set.drive[i].target_vel_raw ) {
                // send pdo
                canmat_status_t cr = canmat_rpdo_send_i16( cx->drive_set.cif,
                                                           cx->drive_set.drive[i].node_id,
                                                           (uint8_t)cx->drive_set.drive[i].rpdo_user,
                                                           vl_target );
                if( CANMAT_OK == cr ) {
                    cx->drive_set.drive[i].target_vel_raw = vl_target;
                } else {
                    SNS_LOG( LOG_ERR, "Couldn't send PDO: %s\n",
                             canmat_iface_strerror( cx->drive_set.cif, cr) );
                }
            }

        }

        T += .001 * 2 * M_PI;

        cx->now = sns_time_add_ns(cx->now, 1000*1000*5 );
        clock_nanosleep( CLOCK_MONOTONIC, TIMER_ABSTIME,
                         &cx->now, NULL );
    }
}


/* static void halt( struct can402_cx *cx, _Bool is_halt ) { */
/*     for( size_t i = 0; i < cx->drive_set.n; i ++ ) { */
/*         _Bool halted = cx->drive_set.drive[i].ctrl_word & CANMAT_402_CTRLMASK_HALT; */
/*         if( (is_halt && !halted) || (!is_halt && halted) ) { */
/*             uint16_t new_ctrl = (uint16_t)(is_halt ? */
/*                                            (cx->drive_set.drive[i].ctrl_word | CANMAT_402_CTRLMASK_HALT ) : */
/*                                            (cx->drive_set.drive[i].ctrl_word & ~CANMAT_402_CTRLMASK_HALT )); */
/*             canmat_status_t r = canmat_rpdo_send_u16( cx->drive_set.cif, */
/*                                                       cx->drive_set.drive[i].node_id, */
/*                                                       (uint8_t)cx->drive_set.drive[i].rpdo_ctrl, */
/*                                                       new_ctrl ); */
/*             if( CANMAT_OK != r ) { */
/*                 SNS_LOG( LOG_EMERG, "Couldn't send halting PDO: %s\n", */
/*                          canmat_iface_strerror( cx->drive_set.cif, r) ); */
/*                 // try to halt (probably will fail again) */
/*                 if( !is_halt ) {  halt(cx,1); return; } */
/*             } else { */
/*                 cx->drive_set.drive[i].ctrl_word = new_ctrl; */
/*             } */
/*         } */
/*     } */
/*     cx->halt = is_halt; */
/* } */

static void update_feedback( struct can402_cx *cx ) {
    for( size_t i = 0; i < cx->drive_set.n; i++ ) {
        // compute MKS values
        struct canmat_402_drive *drive = &cx->drive_set.drive[i];
        int32_t pos_raw = __atomic_load_n( &drive->actual_pos_raw , __ATOMIC_RELAXED );
        int32_t vel_raw = __atomic_load_n( &drive->actual_vel_raw , __ATOMIC_RELAXED );
        drive->actual_pos =  pos_raw / drive->pos_factor;
        drive->actual_vel =  vel_raw / drive->vel_factor;
    }
}

/* Feedback thread:
 * - Only write (atomically) the raw values for velocity and position.
 * - Main thread will do unit conversions and Ach posting
 */
static void *feedback_recv_start( void *cx ) {
    feedback_recv( (struct can402_cx*)cx );
    return NULL;
}
static void feedback_recv( struct can402_cx *cx ) {
    while(sig_shutdown) {
        struct can_frame can;
        // FIXME: add a timeout in case we need to terminate and node isn't responding
        enum canmat_status i = canmat_iface_recv( cx->drive_set.cif, &can);
        if( CANMAT_OK != i ) {
            SNS_LOG( LOG_ERR, "Error receiving CAN frame: %s\n",
                     canmat_iface_strerror(cx->drive_set.cif, i) );
        }
        // TODO: Binary search is better (but this array is tiny)
        // filter non-TPDOs
        if( can.can_id >= CANMAT_TPDO_COBID( 0, 0 ) &&
            can.can_id <= CANMAT_TPDO_COBID( CANMAT_NODE_MASK, 0xFF ) )
        { // got a TPDO
            for( size_t j = 0; j < cx->drive_set.n; j ++ ) {
                struct canmat_402_drive *drive = & cx->drive_set.drive[j];
                if( CANMAT_TPDO_COBID( drive->node_id, drive->tpdo_user ) ==
                    (int)can.can_id ) {
                    // found matching drive
                    // validate
                    if( 8 == can.can_dlc ) {
                        canmat_scalar_t pos, vel;
                        pos.u32 = canmat_byte_ldle32( &can.data[0] );
                        vel.u32 = canmat_byte_ldle32( &can.data[4] );
                        /* FIXME: portability */
                        __atomic_store_n( &drive->actual_pos_raw, pos.i32, __ATOMIC_RELAXED );
                        __atomic_store_n( &drive->actual_vel_raw, vel.i32, __ATOMIC_RELAXED );
                    } else {
                        SNS_LOG(LOG_WARNING, "PDO message to short: %d, expected 8\n", can.can_dlc);
                    }
                }
            }
        }
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