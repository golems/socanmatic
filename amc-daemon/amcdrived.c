/* -*- mode: C; c-basic-offset: 2  -*- */
/** \file amcdrived.c
 *
 *  \author Jon Scholz
 *  \author Neil Dantam
 *  \author Evan Seguin
 */

/** \mainpage
 *  \brief Daemon to control a CAN network using motor and state messages
 *  from an ACH channel
 *
 *  amcdrived is an ACH front-end to amcdrive, the library for communicating with amcdrives
 *  on a CAN network.
 *
 *  NOTE:
 *  amcdrived reads state message in rad, rad/s
 *            write cmd message in N.m
 *
 */


#include <argp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include <somatic.h>
#include <ach.h>

#include <somatic/util.h>
#include <somatic.pb-c.h>
#include <somatic/msg/motor.h>

#include "include/amccan.h"
#include "include/amcdrive.h"
#include "include/amcdrived.h"

//static NTCAN_HANDLE handle;

/* ---------- */
/* ARGP Junk  */
/* ---------- */
//TODO: replace these with vars from parsing args
static const char *opt_cmd_chan = AMCDRIVED_CMD_CHANNEL_NAME;
static const char *opt_state_chan = AMCDRIVED_STATE_CHANNEL_NAME;
static int opt_create = 0;
static int opt_verbosity = 0;
static size_t n_modules = 2;

static struct argp_option options[] = {
    {
        .name = "verbose",
        .key = 'v',
        .arg = NULL,
        .flags = 0,
        .doc = "Causes verbose output"
    },
    {
        .name = "cmd-chan",
        .key = 'c',
        .arg = "amcdrive_cmd_channel",
        .flags = 0,
        .doc = "ach channel to send amcdrive commands to"
    },
    {
		.name = "state-chan",
		.key = 's',
		.arg = "amcdrive_state_channel",
		.flags = 0,
		.doc = "ach channel to listen for commands on"
    },
    {
        .name = "Create",
        .key = 'C',
        .arg = NULL,
        .flags = 0,
        .doc = "Create channels with specified names (off by default)"
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
const char *argp_program_version = "amcdrived 0.0.1";
/// argp program arguments documention
static char args_doc[] = "";
/// argp program doc line
static char doc[] = "reads somatic messages and sends amcdrive motor commands";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL };

static int parse_opt(int key, char *arg, struct argp_state *state) {
	(void) state; // ignore unused parameter

	switch (key) {

	case 'c':
		opt_cmd_chan = strdup(arg);
		break;
	case 's':
		opt_state_chan = strdup(arg);
		break;
	case 'v':
		opt_verbosity++;
		break;
	case 'C':
	    opt_create = 1;
	    break;
	case 0:
		break;
	}
	return 0;
}


/**
 * Generate the amcdrive calls requested by the specified motor command message, and update the state
 */
int amcdrive_execute_and_update(servo_vars_t *servos, Somatic__MotorCmd *msg, ach_channel_t *state_chan)
{

	NTCAN_RESULT status;

	// amcdrive daemon currently accepts only current command
	if (msg->param != SOMATIC__MOTOR_PARAM__MOTOR_CURRENT) {
		fprintf(stdout, "ERROR: Accept only current command.\n");
		exit(0);
	}

    /**
	 * Receive command message from the command channel, and send to amcdrive
	 */

	// Print motor commands received from the command channel
	if (opt_verbosity) {
		size_t i;
		for (i = 0; i < msg->values->n_data; ++i)
			fprintf(stdout, "%lf::", msg->values->data[i]);
		fprintf(stdout, "]\n");
	}

	// Torque-to-current conversion
	double motorLeftCurrent = msg->values->data[0]/KT_LEFT;
	double motorRightCurrent = msg->values->data[1]/KT_RIGHT;

	// Current limit
    motorLeftCurrent = min(MAX_CURRENT, max(-MAX_CURRENT, motorLeftCurrent));
    motorRightCurrent = min(MAX_CURRENT, max(-MAX_CURRENT, motorRightCurrent));

	// Send current to amcdrive
	status = amcdrive_set_current(&servos[0], motorLeftCurrent);
	status = amcdrive_set_current(&servos[1], motorRightCurrent);
    somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot set current (Left)!\n");
    somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot set current (Right)!\n");

	// Update amcdrive state
	status =  amcdrive_update_drives(servos, (int)n_modules);
    somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot update drive states!\n");

	double position[2], velocity[2];
    position[0] = COUNT_TO_RAD(servos[0].position);
	position[1] = COUNT_TO_RAD(servos[1].position);
	velocity[0] = COUNT_TO_RAD(servos[0].vel_cps);
	velocity[1] = COUNT_TO_RAD(servos[1].vel_cps);

    /**
	 * Package a state message, and send/publish to state channel
	 */
	Somatic__MotorState state;
	somatic__motor_state__init(&state);

	// Set Status
	state.has_status = 0; // what do we want to do with this?
	state.status = SOMATIC__MOTOR_STATUS__MOTOR_OK;

	// Set Position
	state.position = SOMATIC_NEW(Somatic__Vector);
	somatic__vector__init(state.position);

	state.position->data = position;
	state.position->n_data = n_modules;

	// Set Velocity
	state.velocity = SOMATIC_NEW(Somatic__Vector);
	somatic__vector__init(state.velocity);

	state.velocity->data = velocity;
	state.velocity->n_data = n_modules;

	// Publish state
	return somatic_motorstate_publish(&state, state_chan);
}

int amcdrive_open(servo_vars_t *servos){
    NTCAN_RESULT status;

    uint32_t drives[2];
    drives[0] = 0x20; // Left
    drives[1] = 0x21; // Right

    status = amcdrive_open_drives(0, drives, 2,
        REQUEST_TPDO_VELOCITY|REQUEST_TPDO_POSITION|ENABLE_RPDO_CURRENT,
        1000, servos);
    if (status != NTCAN_SUCCESS) {
        perror("amcdrive_open_drives");
        return status;
    }
    servos[1].current_sign = -1;

    status = amcdrive_set_current(&servos[0], 0.0);
    status = amcdrive_set_current(&servos[1], 0.0);
    somatic_hard_assert(status == NTCAN_SUCCESS, "zero out 0\n");
    somatic_hard_assert(status == NTCAN_SUCCESS, "zero out 1\n");

	// Update amcdrive state
	status =  amcdrive_update_drives(servos, (int)n_modules);
    somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot update drive states!\n");

    return 0;
}

int main(int argc, char *argv[]) {

	/// Parse command line args
	argp_parse(&argp, argc, argv, 0, NULL, NULL);

	/// install signal handler
	somatic_sighandler_simple_install();

	/// amcdrive setup
	NTCAN_RESULT status;
	servo_vars_t servos[2];

	status = amcdrive_open(servos);
	somatic_hard_assert(status == NTCAN_SUCCESS, "amcdrive initialization failed\n");

	/// Create channels if requested
	if (opt_create == 1) {
		somatic_create_channel(opt_cmd_chan, 10, 30);
		somatic_create_channel(opt_state_chan, 10, 30);
	}

	/// Ach channels for amcdrived
	ach_channel_t *motor_cmd_channel = somatic_open_channel(opt_cmd_chan);
	ach_channel_t *motor_state_channel = somatic_open_channel(opt_state_chan);


	if (opt_verbosity) {
		fprintf(stderr, "\n* JSD *\n");
		fprintf(stderr, "Verbosity:    %d\n", opt_verbosity);
		fprintf(stderr, "command channel:      %s\n", opt_cmd_chan);
		fprintf(stderr, "state channel:      %s\n", opt_state_chan);
		fprintf(stderr, "-------\n");
	}


	/** \par Main loop
	 *
	 *  Listen on the motor command channel, and issue an amcdrive command for
	 *  each incoming message.  When an acknowledgment is received from
	 *  the module group, post it on the state channel.
	 *
	 *  TODO: In addition, set a blocking timeout, and when it expires issue a
	 *  state update request to the modules and update the state channel
	 */
	int ach_result;
	size_t msg_size = 128;

	while (!somatic_sig_received) {
		/// read current state from state channel
		Somatic__MotorCmd *cmd = somatic_motorcmd_receive(motor_cmd_channel, &ach_result, msg_size, NULL, &protobuf_c_system_allocator);
		somatic_hard_assert(ach_result == ACH_OK, "Ach wait failure!\n");

		if (opt_verbosity)
			somatic_motorcmd_print(cmd);

		/// Issue command, and update state
		int r = amcdrive_execute_and_update(servos, cmd, motor_state_channel);

		/// Cleanup
		somatic__motor_cmd__free_unpacked( cmd, &protobuf_c_system_allocator );

	}


	somatic_close_channel(motor_cmd_channel);
	somatic_close_channel(motor_state_channel);


	return 0;
}
