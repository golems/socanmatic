/* -*- mode: C; c-basic-offset: 2  -*- */
/*
 * Copyright (c) 2010, Georgia Tech Research Corporation
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

/** \file amcdrived_commander.c
 *
 *  Shell tool to interact with pciod motor command and state channels
 *
 *  \author Jon Scholz
 */

#include <argp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <somatic.h>
#include <ach.h>

#include <somatic/util.h>
#include <somatic.pb-c.h>

#include "include/amcdrived.h"
#include <somatic/msg/motor.h>

/* ----------- */
/* GLOBAL VARS */
/* ----------- */


/* ---------- */
/* ARGP Junk  */
/* ---------- */
/* Option Vars */
static int opt_verbosity = 0;
static int opt_create = 0;
static const char *opt_cmd_chan = AMCDRIVED_CMD_CHANNEL_NAME;
static const char *opt_state_chan = AMCDRIVED_STATE_CHANNEL_NAME;

/* ---------- */
/* ARGP Junk  */
/* ---------- */
static struct argp_option options[] = {
    {
        .name = "verbose",
        .key = 'v',
        .arg = NULL,
        .flags = 0,
        .doc = "Causes verbose output"
    },
    {
        .name = "commands",
        .key = 'c',
        .arg = "channel",
        .flags = 0,
        .doc = "ach channel name for sending amcdrived commands"
    },
    {
		.name = "states",
		.key = 's',
		.arg = "channel",
		.flags = 0,
		.doc = "ach channel name for receiving amcdrive state messages"
    },
    {
        .name = "Create",
        .key = 'C',
        .arg = NULL,
        .flags = 0,
        .doc = "Create channel with specified name (off by default)"
    },
    {
        .name = NULL,
        .key = 0,
        .arg = NULL,
        .flags = 0,
        .doc = NULL
    }
};


/// argp parsing function
static int parse_opt( int key, char *arg, struct argp_state *state);
/// argp program version
const char *argp_program_version = "amcdrived_commander v0.0.1";
/// argp program arguments documention
static char args_doc[] = "";
/// argp program doc line
static char doc[] = "Reads commands from stdin to generate motor command\n"
					"messages, and reports motor state to stdout\n";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL };


static int parse_opt( int key, char *arg, struct argp_state *state) {
    (void) state; // ignore unused parameter
    switch(key) {
    case 'v':
        opt_verbosity++;
        break;
	case 'c':
		opt_cmd_chan = strdup(arg);
		break;
	case 's':
		opt_state_chan = strdup(arg);
		break;
    case 'C':
    	opt_create = 1;
    case 0:
        break;
    }
    return 0;
}

/* ------- */
/* GLOBALS */
/* ------- */


/* --------------------- */
/* FUNCTION DECLARATIONS */
/* --------------------- */

/**
 * Parses cmdbuf for command char and some set of ints
 */
Somatic__MotorParam parse_command(char *str, double *vals)
{
	Somatic__MotorParam param;
	char *tok = strtok(str, " ");
	if (tok != NULL) {
		switch (*tok) {
		case 'c':
			param = SOMATIC__MOTOR_PARAM__MOTOR_CURRENT;
			break;
		case 'v':
			param = SOMATIC__MOTOR_PARAM__MOTOR_VELOCITY;
			break;
		case 'p':
			param = SOMATIC__MOTOR_PARAM__MOTOR_POSITION;
			break;
		default:
			fprintf(stderr,"Unrecognized command \"%s\"\n", tok);
			return(-1);
		}
	}
	// Loop through the rest assuming they're ints
	tok = strtok(NULL," ");
	int i = 0;
	while (tok != NULL) {
		vals[i] = atof(tok);
		tok = strtok(NULL," ");
		i++;
	}

	return param;
}

/* ---- */
/* MAIN */
/* ---- */
int main(int argc, char **argv) {

	argp_parse(&argp, argc, argv, 0, NULL, NULL);

	// install signal handler
	somatic_sighandler_simple_install();

	// Create channels if requested
	if (opt_create == 1) {
		somatic_create_channel(opt_cmd_chan, 10, 30);
		somatic_create_channel(opt_state_chan, 10, 30);
	}

	// Ach channels for pciod
	ach_channel_t *motor_cmd_channel = somatic_open_channel(opt_cmd_chan);
	ach_channel_t *motor_state_channel = somatic_open_channel(opt_state_chan);

	if (opt_verbosity) {
		fprintf(stderr, "\n* JSD *\n");
		fprintf(stderr, "Verbosity:    %d\n", opt_verbosity);
		fprintf(stderr, "command channel:      %s\n", opt_cmd_chan);
		fprintf(stderr, "state channel:      %s\n", opt_state_chan);
		fprintf(stderr, "-------\n");
	}

	const char *usage_str =
			"Usage: command_parameter [list of values]\n"
			"       The list of values should match the length of the \n"
			"       channel that pciod_commander is writing to.\n"
			"       If only one value is provided, pciod_commander will\n"
			"       write this value to all modules in the group\n"
			"\n"
			"Valid commands:\n"
			"  c \t  -set current \n"
			"  v \t  -set velocity \n"
			"  p \t  -set position \n"
			"\n"
			"e.g.: >> v 5 5 5 0 0 0 0\n";
	printf("%s\n",usage_str);

	/*
	 *  used "size_t size = somatic__motorstate__get_packed_size(msg);"
	 *  in pciod to find the size after packing a message
	 */
	size_t msg_size = 128; // the size of the state message
	int ach_result;
	while (!somatic_sig_received) {
		char cmdbuf[40];
		fputs(">> ", stdout);
		fflush(stdout);

		if (fgets(cmdbuf, sizeof(cmdbuf), stdin) != NULL) {
			char *newline = strchr(cmdbuf, '\n'); /* search for newline character */
			if (newline != NULL)
				*newline = '\0'; /* overwrite trailing newline */

			// Parse if input was provided
			if (strlen(cmdbuf)) {
				size_t n_vals = (strlen(cmdbuf) - 1) / 2; // expected array size user provided proper string
				double vals[n_vals];
				Somatic__MotorParam param = parse_command(cmdbuf, vals);

				if ((int)param != -1) {
					// write motor message to motor channel
					somatic_generate_motorcmd(motor_cmd_channel, vals, n_vals, param);

					// read current state from state channel
					Somatic__MotorState *state = somatic_motorstate_receive(motor_state_channel, &ach_result, msg_size, NULL, &protobuf_c_system_allocator);
					somatic_hard_assert(ach_result == ACH_OK,"Ach wait failure\n");

					somatic_motorstate_print(state);
					somatic__motor_state__free_unpacked( state, &protobuf_c_system_allocator );
				}
			}
		}
	}

	somatic_close_channel(motor_cmd_channel);
	somatic_close_channel(motor_state_channel);

	return 0;
}

