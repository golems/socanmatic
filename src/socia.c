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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>
#include <assert.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>

#include <linux/can.h>
#include <linux/can/raw.h>


#include "socia.h"
#include "socia_private.h"


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
int (*opt_command)(void) = NULL;

void hard_assert( _Bool test , const char fmt[], ...)          ATTR_PRINTF(2,3);

void hard_assert( _Bool test , const char fmt[], ...)  {
    if( ! test ) {
        va_list argp;
        va_start( argp, fmt );
        vfprintf( stderr, fmt, argp );
        va_end( argp );
        exit(EXIT_FAILURE);
    }
}

int can_open() {
    // try some defaults
    int s = socia_can_open( "vcan0" );
    if( s >= 0 ) return s;

    s = socia_can_open( "vcan0" );


    hard_assert( s >= 0, "Couldn't open CAN (%d): %s\n", s, strerror(errno) );
    return s;

}

static void set_cmd( int (*cmd_fun)(void) ) {
    if( NULL == opt_command ) {
        opt_command = cmd_fun;
    }else {
        fprintf(stderr, "Can only specify one command\n");
        exit(EXIT_FAILURE);
    }
}

void dump_frame (struct can_frame *can ) {
    printf("%03x[%d] ", can->can_id, can->can_dlc );
    int i;
    for( i = 0; i < can->can_dlc; i++) {
        uint8_t u = can->data[i];
        printf("%02x%s", u, (i < can->can_dlc - 1)?":":"" );
    }
    printf("\n");
    fflush(stdout);
}

int cmd_dump( ) {
    int fd = can_open();
    while(1) {
        struct can_frame can;
        ssize_t r = socia_can_recv( fd, &can );
        if( r > 0 ) {
            dump_frame(&can);
        } else {
            perror("Couldn't recv frame");
            exit(EXIT_FAILURE);
        }
    }

}

int cmd_send( ) {
    struct can_frame can;
    can.can_id = opt_canid;
    can.can_dlc = opt_can_dlc;
    memcpy(can.data, opt_can_data, opt_can_dlc);
    if(opt_verbosity) {
        fprintf(stderr, "Sending ");
        dump_frame(&can);
    }

    int fd = can_open();
    ssize_t r = socia_can_send( fd, &can );
    hard_assert( socia_can_ok(r), "Couldn't send frame: %s\n", strerror(errno) );
    return 0;
}

unsigned long parse_uhex( const char *arg, uint64_t max ) {
    char *endptr;
    errno = 0;
    unsigned long u  = strtoul( arg, &endptr, 16 );

    hard_assert( 0 == errno, "Invalid hex argument: %s (%s)\n", arg, strerror(errno) );
    hard_assert( u <= max, "Argument %s too big\n", arg );

    return u;
}

void posarg( const char *arg, int i ) {
    switch(i) {
    case 0:
        if( opt_verbosity ) {
            fprintf( stderr, "Setting command: %s\n", arg );
        }
        if( 0 == strcasecmp(arg, "dump") ) {
            set_cmd( cmd_dump );
        } else if( 0 == strcasecmp(arg, "send") ) {
            set_cmd( cmd_send );
        } else {
            goto INVALID;
        }
        break;
    case 1:
        if( cmd_send == opt_command ) {
            opt_canid = (uint16_t)parse_uhex( arg, 0x7FF );
        }
        break;
    case 2:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    case 3:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    case 4:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    case 5:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    case 6:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    case 7:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    case 8:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    case 9:
        if( cmd_send == opt_command ) {
            opt_can_data[opt_can_dlc++] = (uint8_t)parse_uhex( arg, 0xFF );
        }
        break;
    default:
    INVALID:
        hard_assert( 0, "Invalid argument: %s\n", arg );
    }
}

int main( int argc, char ** argv ) {

    int c, i = 0;
    while( (c = getopt( argc, argv, "vhH?V")) != -1 ) {
        switch(c) {
        case 'V':   /* version     */
            puts( "socia " PACKAGE_VERSION "\n"
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
        case '?':   /* help     */
        case 'h':
        case 'H':
            puts( "Usage: socia [OPTION...] [dump]\n"
                  "Shell tool for CANopen\n"
                  "\n"
                  "Options:\n"
                  "  -v,                       Make output more verbose\n"
                  "  -?,                       Give program help list\n"
                  "  -V,                       Print program version\n"
                  "\n"
                  "Examples:\n"
                  "  socia dump                Print CAN messages to standard output\n"
                  "  socia send id xx xx ..    Send a can message\n"
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

    hard_assert( opt_command, "Must specify a command. Say `socia -H' for help.\n");


    if( opt_command ) {
        return opt_command();
    } else {
        assert(0);
    }

    return 0;
}
