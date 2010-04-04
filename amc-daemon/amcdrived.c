/* -*- mode: C; c-basic-offset: 2  -*- */
/** \file pciod.c
 *
 *  \author Jon Scholz
 *  \author Neil Dantam
 *  \author Evan Seguin
 */

/** \mainpage
 *  \brief Daemon to control a CAN network using motor and state messages
 *  from an ACH channel
 *
 *  pciod is an ACH front-end to pcio, the library for communicating with power-cubes
 *  on a CAN network.
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


#define eprintf(f, args...) fprintf(stderr, f, ## args)

static NTCAN_HANDLE handle;

/* ---------- */
/* ARGP Junk  */
/* ---------- */
//TODO: replace these with vars from parsing args
//#define MAX_BUSSES 4
static const char *opt_cmd_chan = AMCDRIVED_CMD_CHANNEL_NAME;
static const char *opt_state_chan = AMCDRIVED_STATE_CHANNEL_NAME;
static int opt_create = 0;
static int opt_verbosity = 0;
//static size_t n_busses = 0;
static size_t n_modules = 1;
//static size_t bus_count[MAX_BUSSES];	// number of modules per bus
//static int bus_ids[MAX_BUSSES]; // Array of bus ids (allows busses to be specified in any order)

// A list mapping motor indices to module information (needed to build the pcio_group_t)
//static pcio_module_list_t *pcio_mod_list;


static struct argp_option options[] = {
    {
        .name = "verbose",
        .key = 'v',
        .arg = NULL,
        .flags = 0,
        .doc = "Causes verbose output"
    },
    {
        .name = "index",
        .key = 'i',
        .arg = "index",
        .flags = 0,
        .doc = "Define a motor index (by default increments with every new module id"
    },
    {
        .name = "module",
        .key = 'm',
        .arg = "module_id",
        .flags = 0,
        .doc = "Define a module ID for a motor index"
    },
    {
        .name = "bus",
        .key = 'b',
        .arg = "CAN_bus",
        .flags = 0,
        .doc = "define a CAN bus for a module"
    },
    {
        .name = "cmd-chan",
        .key = 'c',
        .arg = "pcio_cmd_channel",
        .flags = 0,
        .doc = "ach channel to send amcdrive commands to"
    },
    {
		.name = "state-chan",
		.key = 's',
		.arg = "pcio_state_channel",
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
const char *argp_program_version = "pciod 0.2";
/// argp program arguments documention
static char args_doc[] = "";
/// argp program doc line
static char doc[] = "reads somatic messages and sends pcio motor commands";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL };

static int parse_opt(int key, char *arg, struct argp_state *state) {
	(void) state; // ignore unused parameter

//	static int current_index = 0; // current motor index to be used with 'm' and 'b' args
//	static int current_bus_id = 0;		// The actual bus id to set in pcio_group
//	static int current_bus_index = 0; 	// The ordinality of the current bus

	switch (key) {
//	case 'i':
//		current_index = atoi(arg);
//		break;
//	case 'm':
//		/* Add a new module to the list, and set its CAN bus number and command index */
//		{
//		pcio_mod_list->module_id = atoi( arg );
//		pcio_mod_list->bus_num = current_bus_id;
//		pcio_mod_list->index = current_index;
//
//		/* List overhead: */
//		pcio_mod_list->next = (pcio_module_list_t*) malloc(sizeof(pcio_module_list_t));
//		pcio_mod_list->next->head = pcio_mod_list->head;
//		pcio_mod_list = pcio_mod_list->next;
//		pcio_mod_list->next = NULL;
//
//		/* Update the size of current bus */
//		bus_count[current_bus_index]++;
//		current_index++; // increment idx, in case user isn't specifying manually
//		n_modules++;	// increment n_modules to, since it might differ from current_index
//		}
//		break;
//	case 'b':
//		/* set the bus for subsequent modules */
//		current_bus_id = atoi(arg);
//		// Find index of this bus id
//		int i;
//		current_bus_index = -1;
//		for (i=0;i<MAX_BUSSES;++i)
//			if (bus_ids[i] == current_bus_id)
//				current_bus_index = i;
//		if (current_bus_index == -1) {
//			current_bus_index = (int)n_busses;
//			bus_ids[current_bus_index] = current_bus_id;
//		}
//		n_busses++;
//		//printf("%d %d %d %d\n\n",bus_ids[0],bus_ids[1],bus_ids[2],bus_ids[3]);
//		break;
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
 * Generate the pcio calls requested by the specified motor command message,
 * and update the state with the module acknowledgments
 * \note msg size should match group size
 */
int amcdrive_execute_and_update(servo_vars_t servo, Somatic__MotorCmd *msg, ach_channel_t *state_chan)
{
//	pcio_param_id command;
//	if (opt_verbosity) {
//		switch (msg->param) {
//		case SOMATIC__MOTOR_PARAM__MOTOR_CURRENT:
//			command = PCIO_PARAM_BUSCURRENT; // Correct?  I just picked the one that looked right
//			fprintf(stdout,"Setting motor currents: [");
//			break;
//		case SOMATIC__MOTOR_PARAM__MOTOR_VELOCITY:
//			command = PCIO_TARGET_VEL; // Correct?  I just picked the one that looked right
//			fprintf(stdout,"Setting motor velocities: [");
//			break;
//		case SOMATIC__MOTOR_PARAM__MOTOR_POSITION:
//			command = PCIO_ACT_FPOS; // Correct?  I just picked the one that looked right
//			fprintf(stdout,"Setting motor positions: [");
//			break;
//		default:
//			break;
//		}
//		size_t i;
//		for (i=0; i < msg->values->n_data; ++i)
//			fprintf(stdout,"%lf::",msg->values->data[i]);
//		fprintf(stdout,"]\n");
//	}
//
//	double ack_vals[msg->values->n_data];
//
//	/**
//	 *  Send command values in cmd_vals to pcio group, and receive position
//	 *  Acknowledgments in ack_vals
//	 */
//	int r = pcio_group_cmd_ack(group, ack_vals, msg->values->n_data, command, msg->values->data);
//	somatic_hard_assert( r == NTCAN_SUCCESS, "CAN network failure\n");


	size_t i;
	for (i=0; i < msg->values->n_data; ++i)
		fprintf(stdout,"%lf::",msg->values->data[i]);
	fprintf(stdout,"]\n");

    NTCAN_RESULT status = amcdrive_set_current(&servo, msg->values->data[0]);
    somatic_hard_assert( status == NTCAN_SUCCESS, "spin up\n");



	/**
	 * Package a state message for the ack returned, and send to state channel
	 */
	Somatic__MotorState state;
	somatic__motor_state__init(&state);

	state.has_status = 0; // what do we want to do with this?
	state.status = SOMATIC__MOTOR_STATUS__MOTOR_OK;

	state.position = SOMATIC_NEW(Somatic__Vector);
	somatic__vector__init(state.position);

	//state.position->data = ack_vals;
	//state.position->n_data = n_modules; //TODO: Sneaky use of global variable.  *should* pull from group

	return somatic_motorstate_publish(&state, state_chan);
}


int main(int argc, char *argv[]) {

	/// Parse command line args
	argp_parse(&argp, argc, argv, 0, NULL, NULL);

	/// install signal handler
	somatic_sighandler_simple_install();

	// Open your device
	NTCAN_RESULT status;
	servo_vars_t servo;

	status = canOpen(0,          //net
					 0,          //flags
					 10,         //txqueue
					 128,        //rxqueue
					 1000,       //txtimeout
					 2000,       //rxtimeout
					 &handle);   // handle
	somatic_hard_assert(status == NTCAN_SUCCESS, "canOpen\n");

	status = canSetBaudrate(handle, NTCAN_BAUD_1000);
	somatic_hard_assert(status == NTCAN_SUCCESS, "canSetBaudrate\n");


	/// Create channels if requested
	if (opt_create == 1) {
		somatic_create_channel(opt_cmd_chan, 10, 30);
		somatic_create_channel(opt_state_chan, 10, 30);
	}

	/// Ach channels for amcdrived
	ach_channel_t *motor_cmd_channel = somatic_open_channel(opt_cmd_chan);
	ach_channel_t *motor_state_channel = somatic_open_channel(opt_state_chan);

	/// Declare the state and command messages
	//Somatic__MotorCmd *cmd_msg = somatic_motorcmd_alloc(n_modules);
	//Somatic__MotorState *state_msg = somatic_motorstate_alloc(n_modules);


	/// Initialize the CAN network
	status = amcdrive_init_drive(handle, 0x20,REQUEST_TPDO_VELOCITY|REQUEST_TPDO_CURRENT|ENABLE_RPDO_CURRENT, 1000, &servo);
	somatic_hard_assert(status == NTCAN_SUCCESS, "amcdrive initialization failed\n");


	if (opt_verbosity) {
		fprintf(stderr, "\n* JSD *\n");
		fprintf(stderr, "Verbosity:    %d\n", opt_verbosity);
		fprintf(stderr, "command channel:      %s\n", opt_cmd_chan);
		fprintf(stderr, "state channel:      %s\n", opt_state_chan);
		fprintf(stderr, "-------\n");
	}


	/** \par Main loop
	 *
	 *  Listen on the motor command channel, and issue a pcio command for
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
		somatic_hard_assert(ach_result == ACH_OK,"Ach wait failure\n");

		if (opt_verbosity)
			somatic_motorcmd_print(cmd);

		/// Issue command, and update state with acks
		int r = amcdrive_execute_and_update(servo, cmd, motor_state_channel);

		/// Cleanup
		somatic__motor_cmd__free_unpacked( cmd, &protobuf_c_system_allocator );
	}


	somatic_close_channel(motor_cmd_channel);
	somatic_close_channel(motor_state_channel);

	return 0;
}
