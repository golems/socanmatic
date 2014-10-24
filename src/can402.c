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


#include <syslog.h>
#include <math.h>

#include "socanmatic.h"
#include "socanmatic_private.h"

#include "socanmatic/dict402.h"

#include <sns.h>

/**
 * @struct canmat_402_set
 * @brief Contains api interface for CAN, number of drives and each drive's object
 */
struct canmat_402_set {
    struct canmat_iface *cif;
    uint8_t n;
    struct canmat_402_drive drive[CANMAT_NODE_MASK+1];
} canmat_402_set_t;

/**
 * @struct can402_cx
 * @brief Communication channels, current time, mode of operation and halt boolean
 */
struct can402_cx {
    struct canmat_402_set drive_set;
    struct sns_msg_motor_ref *msg_ref;
    struct sns_msg_motor_state *msg_state;

    enum canmat_402_op_mode op_mode;
    _Bool halt;

    ach_channel_t chan_ref;
    ach_channel_t chan_state;
    ach_channel_t chan_event;
    struct timespec now;
};

/*********************/
/** GLOBAL VARIABLES */
/*********************/
const char *opt_chan_ref = "motor-ref";
const char *opt_chan_state = "motor-state";
const char *opt_chan_event = NULL;
const char *opt_cmd = NULL;
const char *opt_api = "socketcan";
const char **opt_pos = NULL;
size_t opt_npos = 0;
int opt_rpdo_ctrl = 0;
int opt_rpdo_user = 1;
int opt_tpdo_user = 0;
int opt_tpdo_stat = -1;

/** Frequency */
double opt_timeout_sec = 0.01; // 100 Hz

// FIXME: 1
//@ntd: Did you read this from the Schunk Motion software. Should be in its own object */
double opt_vel_factor = 180/M_PI*1000;
double opt_pos_factor = 180/M_PI*1000;

/**********/
/* PROTOS */
/**********/
static void init( struct can402_cx *cx );
static void run( struct can402_cx *cx );
static void process( struct can402_cx *cx );
static void halt( struct can402_cx *cx, _Bool is_halt );
static void stop( struct can402_cx *cx );
static void parse( struct can402_cx *cx, int argc, char **argv );

/* Feedback thread */
static void feedback_recv( struct can402_cx *cx );
static void *feedback_recv_start( void *cx );

/* Called from main thread */
static void update_feedback( struct can402_cx *cx );
static void send_feedback( struct can402_cx *cx );

/***********************/
/*  MAIN FUNCTION      */
/***********************/
int main( int argc, char ** argv ) {


	sns_init();
    struct can402_cx cx;
    memset( &cx, 0, sizeof(cx));

    /** Set default: Velocity mode, halted */
    cx.op_mode = CANMAT_402_OP_MODE_VELOCITY;
    //cx.op_mode = CANMAT_402_OP_MODE_PROFILE_POSITION;
    cx.halt = 1;

    /** Parse */
    parse( &cx, argc, argv );

    if( sns_cx.verbosity ) {
        for( size_t i = 0; i < cx.drive_set.n; i ++ ) {
            SNS_LOG( LOG_INFO, "node: 0x%x\n", cx.drive_set.drive[i].node_id );
        }
    }

    /** Initialize drives */
    init(&cx);

    if( sns_cx.verbosity ) {
        for( size_t i = 0; i < cx.drive_set.n; i ++ ) {
            SNS_LOG( LOG_INFO, "limits 0x%x: %f (%f) -- %f (%f)\n",
                     cx.drive_set.drive[i].node_id,
                     cx.drive_set.drive[i].pos_min_soft,
                     cx.drive_set.drive[i].pos_min_hard,
                     cx.drive_set.drive[i].pos_max_soft,
                     cx.drive_set.drive[i].pos_max_hard );
        }
    }

    ////////////////////////////////////////////////////
    for( int i = 0; i < cx.drive_set.n; ++i ) {
    	printf("[%d] POS MIN: Soft:%f Hard:%f POS MAX:  Soft:%f Hard: %f \n",
    			cx.drive_set.drive[i].node_id,
    			cx.drive_set.drive[i].pos_min_soft,
    			cx.drive_set.drive[i].pos_min_hard,
    			cx.drive_set.drive[i].pos_max_soft,
    			cx.drive_set.drive[i].pos_max_hard);
        int32_t pos_raw; uint32_t error;
        canmat_402_ul_position_actual_value( cx.drive_set.cif, cx.drive_set.drive[i].node_id,
        									 &pos_raw,
        									 &error );
        double pos_double = pos_raw /cx.drive_set.drive[i].pos_factor;
        printf("[%d] Position raw: hex: %x real: %f \n", cx.drive_set.drive[i].node_id,
        		pos_raw, pos_double);

        int16_t vel_raw;
        canmat_402_ul_vl_target_velocity( cx.drive_set.cif, cx.drive_set.drive[i].node_id,
        								  &vel_raw, &error );
        double vel_double = vel_raw / cx.drive_set.drive[i].vel_factor;
        printf("[%d] Velocity raw: hex: %x float: %f \n", cx.drive_set.drive[i].node_id,
        		vel_raw, vel_double );

        // Profile velocity
    	uint32_t prof_vel;
    	canmat_402_ul_profile_velocity( cx.drive_set.cif, cx.drive_set.drive[i].node_id,
				  &prof_vel, &error );
    	double prof_vel_double = prof_vel / cx.drive_set.drive[i].vel_factor;
    	printf("[%d] Profile Velocity raw: hex: %x float: %f \n", cx.drive_set.drive[i].node_id,
    	        		prof_vel, prof_vel_double );

        // End velocity
    	uint32_t end_vel;
    	canmat_402_ul_end_velocity( cx.drive_set.cif, cx.drive_set.drive[i].node_id,
				  &end_vel, &error );
    	double end_vel_double = end_vel / cx.drive_set.drive[i].vel_factor;
    	printf("[%d] End Velocity raw: hex: %x float: %f \n", cx.drive_set.drive[i].node_id,
    	        		end_vel, end_vel_double );

        // Profile acc
    	uint32_t prof_acc;
    	canmat_402_ul_profile_acceleration( cx.drive_set.cif, cx.drive_set.drive[i].node_id,
				  &prof_acc, &error );
    	double prof_acc_double = prof_acc / cx.drive_set.drive[i].vel_factor;
    	printf("[%d] Profile Acceleration raw: hex: %x float: %f \n", cx.drive_set.drive[i].node_id,
    	        		prof_acc, prof_acc_double );

        // End acc
    	uint32_t profile_dec;
    	canmat_402_ul_profile_deceleration( cx.drive_set.cif, cx.drive_set.drive[i].node_id,
				  &profile_dec, &error );
    	double profile_dec_double = profile_dec / cx.drive_set.drive[i].vel_factor;
    	printf("[%d] Prof decc raw: hex: %x float: %f \n", cx.drive_set.drive[i].node_id,
    	        		profile_dec, profile_dec_double );

    	// Position notation

    	int8_t pos_notation;
    	canmat_402_ul_position_notation_index( cx.drive_set.cif, cx.drive_set.drive[i].node_id,
				  &pos_notation, &error );
    	printf("[%d] Position notation raw: hex: %x \n", cx.drive_set.drive[i].node_id,
    	        		pos_notation);


    }

    ////////////////////////////////////////////////////

    // run
    pthread_t feedback_thread;
    if( pthread_create( &feedback_thread, NULL, feedback_recv_start, &cx ) ) {
        SNS_DIE( "Couldn't create feedback thread: %s\n", strerror(errno) );
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

/**
 * @function parse
 * @brief Parse daemon options
 */
static void parse( struct can402_cx *cx, int argc, char **argv )
{
    assert( 0 == cx->drive_set.n );
    for( int c; -1 != (c = getopt(argc, argv, "c:s:hH?Vf:a:n:R:C:d:e:" SNS_OPTSTRING)); ) {
        switch(c) {
            SNS_OPTCASES
        case 'V':   /* version     */
            puts( "can402 " PACKAGE_VERSION "\n"
                  "\n"
                  "Copyright (c) 2008-2014, Georgia Tech Research Corporation\n"
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
        case 'e': /* event channel */
            opt_chan_event = strdup(optarg);
            break;
        case 'n':   /* node  */
            SNS_REQUIRE( cx->drive_set.n < CANMAT_NODE_MASK-1, "Too many nodes\n" );
            cx->drive_set.drive[ cx->drive_set.n ].node_id = (uint8_t) parse_u( optarg, 16, CANMAT_NODE_MASK );
            cx->drive_set.drive[ cx->drive_set.n ].rpdo_ctrl = opt_rpdo_ctrl;
            cx->drive_set.drive[ cx->drive_set.n ].rpdo_user = opt_rpdo_user;
            cx->drive_set.drive[ cx->drive_set.n ].tpdo_user = opt_tpdo_user;
            cx->drive_set.drive[ cx->drive_set.n ].tpdo_stat = opt_tpdo_stat;
            cx->drive_set.drive[ cx->drive_set.n ].pos_factor = opt_pos_factor;
            cx->drive_set.drive[ cx->drive_set.n ].vel_factor = opt_vel_factor;
            cx->drive_set.n++;
            break;
        case 'd': /* offset */
            SNS_REQUIRE( cx->drive_set.n > 0, "Must give node ID before offset\n" );
            cx->drive_set.drive[ cx->drive_set.n -1 ].pos_offset = atof(optarg) * M_PI / 180.0;
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
            puts( "Usage: can402 [OPTIONS...] \n"
                  "Driver daemon for CANopen servo drivies\n"
                  "\n"
                  "Options:\n"
                  "  -v,                       Make output more verbose\n"
                  "  -a api_type,              CAN API, e.g, socketcan, ntcan\n"
                  "  -f interface,             CAN interface\n"
                  "  -n id,                    Node (multiple allowed)\n"
                  "  -d degrees,               Position offset of last node\n"
                  "  -c ref_channel,           Reference Ach Channel name (last-message only)\n"
                  "  -s state_channel,         State Ach Channel name\n"
                  "  -e event_channel,         Event Ach Channel name (all messages)\n"
                  "  -R number,                User RPDO (from zero)\n"
                  "  -C number,                Control RPDO (from zero)\n"
                  "  -?,                       Give program help list\n"
                  "  -V,                       Print program version\n"
                  "\n"
                  "Examples:\n"
                  " can402 -f can0 -R 1 -C 0 -n 3 -n 4 -c ref -s state    Interface with nodes 3 and 4\n"
                  "\n"
                  "Report bugs to <ntd@gatech.edu>"
                );
            exit(EXIT_SUCCESS);
            break;
        default:
            SNS_DIE( "Invalid argument: %s\n", optarg );
        }
    }
    if( optind < argc ) {
        SNS_DIE( "Invalid argument: %s\n", argv[optind] );
    }


    SNS_REQUIRE( cx->drive_set.cif, "can402: missing interface.\nTry `can402 -H' for more information.\n");
    SNS_REQUIRE( cx->drive_set.n, "can402: missing node IDs.\nTry `can402 -H' for more information.\n");

    cx->msg_ref = sns_msg_motor_ref_heap_alloc ( cx->drive_set.n );
    cx->msg_state = sns_msg_motor_state_heap_alloc ( cx->drive_set.n );

    for( size_t i = 0; i < cx->drive_set.n; i++ ) {
        SNS_REQUIRE( cx->drive_set.drive[i].rpdo_user != cx->drive_set.drive[i].rpdo_ctrl,
                     "Need distinct user and controlword PDOs.  Drive %"PRIuPTR" using RPDO %d for both\n",
                     i, cx->drive_set.drive[i].rpdo_user );
    }

}

/**
 * @function init 
 * @brief Open reference/state channels.
 */
static void init( struct can402_cx *cx ) {

    // Send signal to ACHCOP
    sns_start();

    // Open channels
    sns_chan_open( &cx->chan_ref, opt_chan_ref, NULL );
    sns_chan_open( &cx->chan_state, opt_chan_state, NULL );
    {
        ach_channel_t *chans[] = {&cx->chan_ref, NULL};
        sns_sigcancel( chans, sns_sig_term_default );
    }
    if( opt_chan_event )
        sns_chan_open( &cx->chan_event, opt_chan_event, NULL );


    enum canmat_status r;

    // Initialize drives
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_init( cx->drive_set.cif, cx->drive_set.drive[i].node_id, &cx->drive_set.drive[i] );
        ///////////////////////////////////////////
        printf("[%d] Control word: %x \n", i, cx->drive_set.drive[i].ctrl_word );
        printf("[%d] Status word: %x \n", i, cx->drive_set.drive[i].stat_word );
        printf("[%d] Raw position: %x \n", i, cx->drive_set.drive[i].actual_pos_raw );
        printf("[%d] Raw velocity: %x \n", i, cx->drive_set.drive[i].actual_vel_raw );
        printf("[%d] Target velocity raw: %x \n", i, cx->drive_set.drive[i].target_vel_raw );
        printf("[%d] Position factor: %f \n", i, cx->drive_set.drive[i].pos_factor );
        printf("[%d] Velocity factor: %f \n", i, cx->drive_set.drive[i].vel_factor );
        printf("[%d] Current factor: %f \n", i, cx->drive_set.drive[i].cur_factor );
        // Get position notation index
        canmat_status_t r;
        uint8_t val;
        uint32_t err;
        /*
        r = canmat_402_ul_position_dimension_index( cx->drive_set.cif,
        											cx->drive_set.drive[i].node_id,
        											&val,
        											&err );
        if( r != CANMAT_OK) { printf("OH CRAP, drive [%d] could not upload dim index \n", i);}
*/
        ///////////////////////////////////////////
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

    // Map the feedback (TPDO Message for motor states and status word)
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        const canmat_obj_t *objs[2] = { CANMAT_402_OBJ_POSITION_ACTUAL_VALUE,
                                        CANMAT_402_OBJ_VELOCITY_ACTUAL_VALUE };
        // user TPDO
        r = canmat_pdo_remap( cx->drive_set.cif, cx->drive_set.drive[i].node_id,
                              (uint8_t)(cx->drive_set.drive[i].tpdo_user), CANMAT_UL,
                              0xFE, -1, 10,
                              2, objs, &cx->drive_set.drive[i].abort_code );
        if( r != CANMAT_OK ) {
            SNS_LOG( LOG_EMERG, "can402: couldn't map user tpdo: '%s'\n",
                     canmat_iface_strerror( cx->drive_set.cif, r) );
            goto FAIL;
        }
        // status TPDO
        if( 0 <= cx->drive_set.drive[i].tpdo_stat ) {
            const canmat_obj_t *stat_obj[1] = { CANMAT_402_OBJ_STATUSWORD };
            r = canmat_pdo_remap( cx->drive_set.cif, cx->drive_set.drive[i].node_id,
                                  (uint8_t)(cx->drive_set.drive[i].tpdo_stat), CANMAT_UL,
                                  0xFE, -1, 10,
                                  1, stat_obj, &cx->drive_set.drive[i].abort_code );
            if( r != CANMAT_OK ) {
                SNS_LOG( LOG_EMERG, "can402: couldn't map status tpdo: '%s'\n",
                         canmat_iface_strerror( cx->drive_set.cif, r) );
                goto FAIL;
            }
        }
    }


    // Set operation mode (position / velocity) and PDOs
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        r = canmat_402_set_op_mode( cx->drive_set.cif, &cx->drive_set.drive[i], CANMAT_402_OP_MODE_VELOCITY );
        if( r != CANMAT_OK ) {
            SNS_LOG( LOG_EMERG, "can402: couldn't set op mode: '%s'\n",
                   canmat_iface_strerror( cx->drive_set.cif, r) );
            goto FAIL;
        }
    }

    // Start: Set all drives to Operational mode
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

    //////////////////////
    printf("\t * After Starting all drives. I expect to see 0x27 for all the drives' status words \n");
    for( size_t i = 0; i < cx->drive_set.n; ++i ) {
    	printf("\t * Status of drive %d: %x \n", cx->drive_set.drive[i].node_id, cx->drive_set.drive[i].stat_word );
    }
    //////////////////////


    return;

FAIL:
    stop(cx);
    exit(EXIT_FAILURE);
}

/**
 * @function get_msg
 * @brief Attempts to read a message in channel within the timeout specified
 */
static void get_msg( struct can402_cx *cx, ach_channel_t *channel,
                     struct timespec *timeout, int options  )
{
    const size_t expected_size = sns_msg_motor_ref_size_n(cx->drive_set.n);
    size_t frame_size = 0;

    ach_status_t r = ach_get( channel, cx->msg_ref,
                              expected_size,
                              &frame_size, timeout, options );

    if( ACH_TIMEOUT == r ) {
        /* If it's a timeout, use the previously gotten time */
        memcpy(&cx->now, timeout, sizeof(*timeout));
    } else {
        clock_gettime( ACH_DEFAULT_CLOCK, &cx->now );
    }
    update_feedback(cx);
    switch(r) {
    case ACH_TIMEOUT:
    	if( sns_msg_is_expired(&cx->msg_ref->header, &cx->now) ) {
            //SNS_LOG( LOG_NOTICE, "Reference timeout\n");
            halt(cx, 1);
            break;
        }
    break;
    case ACH_MISSED_FRAME: /* This is probably OK */
    case ACH_OK:
        // validate
        if( 0 == sns_msg_motor_ref_check_size(cx->msg_ref, frame_size)  &&
            frame_size == expected_size )
        {
            process(cx);
        } else {
            SNS_LOG( LOG_ERR, "bogus message: n: %"PRIu32", expected: %"PRIu32", "
                     "size: %"PRIuPTR", expected: %"PRIuPTR"\n",
                     cx->msg_ref->header.n, cx->drive_set.n,
                     frame_size, expected_size );
        }
        break;
        /* Really bad things we just give up on */
    case ACH_BUG:
    case ACH_CORRUPT:
        SNS_DIE( "ach_get failed badly, aborting: '%s'\n", ach_result_to_string(r) );
        break;
    case ACH_CANCELED:
    case ACH_STALE_FRAMES:
        break;
    default:
        SNS_LOG( LOG_ERR, "ach_get failed: '%s'\n", ach_result_to_string(r) );
    }

}

/**
 * @function run
 * @brief Get motor command, send motor state
 */
static void run( struct can402_cx *cx ) {
    int64_t timeout_ns = (int64_t)(1e9*opt_timeout_sec);
    clock_gettime( ACH_DEFAULT_CLOCK, &cx->now );
    cx->msg_ref->header.n = cx->drive_set.n;
    while( ! sns_cx.shutdown ) {
        /*-- reference --*/
        struct timespec timeout = sns_time_add_ns( cx->now, timeout_ns );
        get_msg(  cx, &cx->chan_ref, &timeout, ACH_O_WAIT | ACH_O_LAST );
        /*-- event --*/
        if( opt_chan_event )
            get_msg(  cx, &cx->chan_event, &timeout, 0 );
        /*-- send_feedback --*/
        send_feedback(cx);
    }
}

static double pos_limit( struct canmat_402_drive *drive, double val ) {
    // TODO: should we consider the pos offset here?
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

// workaround for Schunk PRL+ 0.62 firmware
#define VEL_MIN (INT16_MIN+1)
#define VEL_MAX (INT16_MAX)

// JUST COPIED FROM ABOVE. DO NOT USE IT UNTIL CHECKED AGAIN
#define POS_MIN (INT16_MIN+1)
#define POS_MAX (INT16_MAX)


/**
 * @function process
 */
static void process( struct can402_cx *cx ) {
    if( SNS_LOG_PRIORITY(LOG_DEBUG + 1) ) {
        sns_msg_motor_ref_dump( stderr, cx->msg_ref );
    }
    // Check if expired
    if( sns_msg_is_expired(&cx->msg_ref->header, &cx->now) ) {
        SNS_LOG( LOG_WARNING, "Reference message expired\n");
        return;
    }

    // TODO: op mode switching
    // TODO: check that mode is supported
    /* TODO: maybe some congestion control
     *       If too many messages are sent, write() returns ENOBUFS
     *       Could prioritize messages based on difference from previous target
     */
    switch( cx->msg_ref->mode ) {
    /** Velocity mode */
    case SNS_MOTOR_MODE_VEL:

    	halt(cx, 0); // unhalt
        if( cx->halt ) return;  // make sure we unhalted

        for( size_t i = 0; i < cx->msg_ref->header.n; i ++ ) {
            // position limit
            double val = pos_limit( &cx->drive_set.drive[i], cx->msg_ref->u[i] );
            // clamp value
            val *= cx->drive_set.drive[i].vel_factor;
            int16_t vl_target = 0;
            if( val > VEL_MAX ) {
                vl_target = VEL_MAX;
                SNS_LOG(LOG_DEBUG, "clamp+ %f -> %d 0x%x\n", val, vl_target, cx->drive_set.drive[i].node_id);
            } else if (val < VEL_MIN ) {
                vl_target = VEL_MIN;
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
        break;
    /** Offset position */
    case SNS_MOTOR_MODE_POS_OFFSET:
        for( size_t i = 0; i < cx->msg_ref->header.n; i ++ ) {
            cx->drive_set.drive[i].pos_offset = cx->msg_ref->u[i];
        }
        break;
    /** Profile position mode */
    case SNS_MOTOR_MODE_POS:
    	halt(cx,0); // unhalt
    	if( cx->halt ) return; // Make sure we unhalted
    	for( size_t i = 0; i < cx->msg_ref->header.n; ++i ) {
    		// Check position limit
    		double pos = pos_limit( &cx->drive_set.drive[i], cx->msg_ref->u[i] );
    		// Clamp value
    		pos *= cx->drive_set.drive[i].pos_factor;
    		int16_t target_position = 0;
    		if( pos > POS_MAX ) {

    		} else if( pos < POS_MIN ) {

    		} else {
    			target_position = (int16_t) pos;
    		}

    		// Check if update is necessary
    		if( target_position != cx->drive_set.drive[i].target_pos_raw ) {
    			// Send PDO
    			canmat_status_t cr = canmat_rpdo_send_i16( cx->drive_set.cif,
    													   cx->drive_set.drive[i].node_id,
    													   (uint8_t)cx->drive_set.drive[i].rpdo_user,
    													   target_position );

    			if( CANMAT_OK == cr ) {
    				cx->drive_set.drive[i].target_pos_raw = target_position;
    			} else {
    				SNS_LOG( LOG_ERR, "Couldn't send PDO: %s \n",
    						canmat_iface_strerror(cx->drive_set.cif, cr) );
    			}
    		}

    	}
        break;
    default:
        SNS_LOG( LOG_ERR, "unhandled op mode in motor_ref msg: '%d'\n", cx->msg_ref->mode );
    }
}

/**
 * @function halt
 * @brief Set all motors in drive_set to is_halt (0:unhalt, 1:halt)
 */
static void halt( struct can402_cx *cx, _Bool is_halt ) {
    for( size_t i = 0; i < cx->drive_set.n; i ++ ) {
        _Bool halted = cx->drive_set.drive[i].ctrl_word & CANMAT_402_CTRLMASK_HALT;
        if( (is_halt && !halted) || (!is_halt && halted) ) {
            uint16_t old_ctrl = cx->drive_set.drive[i].ctrl_word;
            uint16_t new_ctrl = (uint16_t)(is_halt ?
                                           (old_ctrl | CANMAT_402_CTRLMASK_HALT) :
                                           (old_ctrl & ~CANMAT_402_CTRLMASK_HALT) );
            printf("drive: %x, old_ctrl: %x, new_ctrl: %x\n",
                   cx->drive_set.drive[i].node_id, old_ctrl, new_ctrl );
            canmat_status_t r = canmat_rpdo_send_u16( cx->drive_set.cif,
                                                      cx->drive_set.drive[i].node_id,
                                                      (uint8_t)cx->drive_set.drive[i].rpdo_ctrl,
                                                      new_ctrl );
            if( CANMAT_OK != r ) {
                SNS_LOG( LOG_EMERG, "Couldn't send halting PDO: %s\n",
                         canmat_iface_strerror( cx->drive_set.cif, r) );
                // try to halt (probably will fail again)
                if( !is_halt ) {  halt(cx,1); return; }
            } else {
                cx->drive_set.drive[i].ctrl_word = new_ctrl;
            }
        }
    }
    cx->halt = is_halt;
}

/**
 * @function update_feedback
 * @brief Read the latest motor states (raw and then convert them accordingly)
 */
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

static void send_feedback( struct can402_cx *cx ) {
    for( size_t i = 0; i < cx->drive_set.n; i++ ) {
        struct canmat_402_drive *drive = &cx->drive_set.drive[i];
        // build message
        cx->msg_state->X[i].pos = drive->actual_pos + drive->pos_offset;
        cx->msg_state->X[i].vel = drive->actual_vel;
    }
    cx->msg_state->header.seq++;
    sns_msg_set_time( &cx->msg_state->header, &cx->now, (int64_t)(opt_timeout_sec*1e9*2) );
    // send message
    ach_status_t r = ach_put( &cx->chan_state, cx->msg_state,
                              sns_msg_motor_state_size(cx->msg_state) );
    if( ACH_OK != r ) {
        SNS_LOG( LOG_ERR, "Couldn't put ach frame: %s\n", ach_result_to_string(r) );
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
    while(!sns_cx.shutdown) {
        struct can_frame can;
        enum canmat_status i = canmat_iface_recv( cx->drive_set.cif, &can);
        if( CANMAT_OK != i ) {
            if( !(CANMAT_ERR_OS == i &&
                  EINTR == cx->drive_set.cif->err ))
            {
                SNS_LOG( LOG_ERR, "Error receiving CAN frame: %s\n",
                         canmat_iface_strerror(cx->drive_set.cif, i) );
            }
            continue;
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
                        SNS_LOG(LOG_WARNING, "PDO message too short: %d, expected 8\n", can.can_dlc);
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
    sns_end();
}
