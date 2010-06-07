/* -*- mode: C; c-basic-offset: 2  -*- */
/** \file amciod.c
 *
 *  \author Jon Scholz
 *  \author Neil Dantam
 *  \author Evan Seguin
 *  \author Kasemsit Teeyapan
 */

/** \mainpage
 *  \brief Daemon to control a CAN network using motor and state messages
 *  from an ACH channel
 *
 *  amciod is an ACH front-end to amcdrive, the library for communicating with amcdrives
 *  on a CAN network.
 *
 *  NOTE:
 *  amciod reads state message in rad, rad/s
 *            write cmd message in Ampere
 *
 */

#include "include/amccan.h"
#include "include/amcdrive.h"
#include "include/amciod.h"

/* ---------- */
/* ARGP Junk  */
/* ---------- */
//TODO: replace these with vars from parsing args
static const char *opt_cmd_chan = AMCIOD_CMD_CHANNEL_NAME;
static const char *opt_state_chan = AMCIOD_STATE_CHANNEL_NAME;
static unsigned int  amciod_cmd_channel_size = AMCIOD_CMD_CHANNEL_SIZE;
static unsigned int  amciod_state_channel_size = AMCIOD_STATE_CHANNEL_SIZE;

static int opt_create = 0;
static int opt_verbosity = 0;
static double opt_frequency = 30.0; // refresh at 30 hz
static size_t n_buses = 0;			// number of bus
static size_t n_modules = 0;		// number of amc modules
static unsigned int opt_bus_id = 0; // amc bus id
static uint32_t opt_mod_id[2];		// amc module id

static size_t max_n_buses = 1;		// TODO: Currently cannot accept more than 1 bus
static size_t max_n_modules = 2;	// maximum number of drives

static struct argp_option options[] = {
		{
				.name = "cmd-chan",
				.key = 'c',
				.arg = "amciod-cmd",
				.flags = 0,
				.doc = "ACH channel to send amcdrive commands to"
		},
		{
				.name = "state-chan",
				.key = 's',
				.arg = "amciod-state",
				.flags = 0,
				.doc = "ACH channel to listen for commands on"
		},
		{
				.name = "verbose",
				.key = 'v',
				.arg = NULL,
				.flags = 0,
				.doc = "Causes verbose output"
		},
		{
				.name = "Create",
				.key = 'C',
				.arg = NULL,
				.flags = 0,
				.doc = "Create channels with specified names (off by default)"
		},
		{
				.name = "frequency",
				.key = 'f',
				.arg = "freq",
				.flags = 0,
				.doc = "Refresh rate on state channel when no commands are received"
		},
		{
				.name = "module",
				.key = 'm',
				.arg = "module_id",
				.flags = 0,
				.doc = "Define a module ID for a motor index (e.g. 0x20)"
		},
		{
				.name = "bus",
				.key = 'b',
				.arg = "CAN_bus",
				.flags = 0,
				.doc = "Define a CAN bus for a module"
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
const char *argp_program_version = "amciod 0.2";
/// argp program arguments documention
static char args_doc[] = "";
/// argp program doc line
static char doc[] = "reads somatic messages and sends amcdrive motor commands";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL };

static int parse_opt(int key, char *arg, struct argp_state *state) {
	(void) state; // ignore unused parameter

	if (n_buses > max_n_buses) {
		fprintf(stderr, "ERROR: Accept only one -b parameters.\n");
		exit(EXIT_FAILURE);
	}

	if (n_modules > max_n_modules) {
		fprintf(stderr, "ERROR: Accept only two -m parameters.\n");
		exit(EXIT_FAILURE);
	}

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
	case 'f':
		opt_frequency = atof(arg);
		break;
	case 'b':
		opt_bus_id = atoi(arg);		// accept only one bus id
		n_buses++;
		break;
	case 'm':
		opt_mod_id[n_modules] = atoi(arg);	// Add an AMC module id to the list
		n_modules++;
		break;
	default:
		return ARGP_ERR_UNKNOWN;

	}
	return 0;
}

/**
 * Function Declaration
 */
void amcdrive_execute_and_update(servo_vars_t *, Somatic__MotorCmd *, ach_channel_t *);
void amcdrive_update_state(servo_vars_t *, ach_channel_t *);
int amcdrive_open(servo_vars_t *);

/**
 * Generate the amcdrive calls requested by the specified motor command message, and update the state
 */
int main(int argc, char *argv[]) {

	/// Parse command line args
	argp_parse(&argp, argc, argv, 0, NULL, NULL);

	/// Argument check
	if (n_buses < max_n_buses) {
		fprintf(stderr, "ERROR: Please specify bus ID (e.g. -b 0).\n");
		exit(EXIT_FAILURE);
	}

	if (n_modules < max_n_modules) {
		fprintf(stderr, "ERROR: Please specify two motor IDs (e.g. -m 32 -m 33).\n");
		exit(EXIT_FAILURE);
	}

	/// install signal handler
	somatic_sighandler_simple_install();

	/// Set the wait time based on specificed frequency
	struct timespec wait_time = {
	    .tv_sec = 0,
	    .tv_nsec = (long int)((1.0/opt_frequency) * 1e9)
	};

	/// amcdrive setup
	NTCAN_RESULT status;
	servo_vars_t servos[2];

	printf("n_module = %d\n", n_modules);

	status = amcdrive_open(servos);
	somatic_hard_assert(status == NTCAN_SUCCESS, "amcdrive initialization failed\n");

	/// Create channels if requested
	if (opt_create == 1) {
		somatic_create_channel(opt_cmd_chan, 10, amciod_cmd_channel_size);
		somatic_create_channel(opt_state_chan, 10, amciod_state_channel_size);
	}

	/// Ach channels for amcdrived
	ach_channel_t *motor_cmd_channel = somatic_open_channel(opt_cmd_chan);
	ach_channel_t *motor_state_channel = somatic_open_channel(opt_state_chan);


	if (opt_verbosity) {
		fprintf(stderr, "\n* AMCIOD *\n");
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

	//  used "size_t size = somatic__motorstate__get_packed_size(msg);"  to find the size after packing a message

	/// wait for SIGINT
	while (!somatic_sig_received) {
		struct timespec abstime = somatic_timespec_future(wait_time);

		/// read current state from state channel
		Somatic__MotorCmd *cmd = somatic_motorcmd_receive(motor_cmd_channel, &ach_result, AMCIOD_CMD_CHANNEL_SIZE, &abstime, &protobuf_c_system_allocator);
		somatic_hard_assert(ach_result == ACH_OK || ach_result == ACH_TIMEOUT, "Ach wait failure on cmd receive!\n");

		if (ach_result == ACH_TIMEOUT) {
		    //if (verbosity) { printf("timeout!!\n");}
		    amcdrive_update_state(servos, motor_state_channel);
		}
		else {
		    if (opt_verbosity) {
			    somatic_motorcmd_print(cmd);
		    }

		    /// Issue command, and update state
		    amcdrive_execute_and_update(servos, cmd, motor_state_channel);
		    /// Cleanup
		    somatic__motor_cmd__free_unpacked( cmd, &protobuf_c_system_allocator );
		}

		if (opt_verbosity) {
			//printf("pos: %f  %f [rad]\t vel: %f  %f [rad/s]\n", COUNT_TO_RAD(servos[0].pos_c), COUNT_TO_RAD(servos[1].pos_c), COUNT_TO_RAD(servos[0].vel_cps), COUNT_TO_RAD(servos[1].vel_cps));
			printf("pos: %f  %f [cnt]\t", servos[0].pos_c, servos[1].pos_c);
			printf("vel: %f  %f [rad/s]\n", COUNT_TO_RAD(servos[0].vel_cps), COUNT_TO_RAD(servos[1].vel_cps));
		}
	}

	/// Close channels
	somatic_close_channel(motor_cmd_channel);
	somatic_close_channel(motor_state_channel);

	/// Stop motors
    //usleep(500000);
    status = amcdrive_set_current(&servos[0], 0.0);
    somatic_hard_assert(status == NTCAN_SUCCESS, "zero out 0\n");
    status = amcdrive_set_current(&servos[1], 0.0);
    somatic_hard_assert(status == NTCAN_SUCCESS, "zero out 1\n");

	return 0;
}

void amcdrive_execute_and_update(servo_vars_t *servos, Somatic__MotorCmd *msg, ach_channel_t *state_chan)
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

		//size_t size = somatic__motor_cmd__get_packed_size(msg);
		//printf("\tmotor_cmd packed size = %d \n",size);
	}

	double motorLeftCurrent = msg->values->data[0];
	double motorRightCurrent = msg->values->data[1];
	
	// Current limit
    motorLeftCurrent = min(MAX_CURRENT, max(-MAX_CURRENT, motorLeftCurrent));
    motorRightCurrent = min(MAX_CURRENT, max(-MAX_CURRENT, motorRightCurrent));

	// Send current to amcdrive
	status = amcdrive_set_current(&servos[0], motorLeftCurrent);
	somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot set current (Left)!\n");
	status = amcdrive_set_current(&servos[1], motorRightCurrent);
    somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot set current (Right)!\n");

    // Update states
    amcdrive_update_state(servos, state_chan);

}

void amcdrive_update_state(servo_vars_t *servos, ach_channel_t *state_chan)
{
	NTCAN_RESULT status;

	// Update amcdrive state
	status =  amcdrive_update_drives(servos, (int)n_modules);
	somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot update drive states!\n");

	/* Kasemsit:
	 * I don't know why amcdrive_update_drives() gives me correct position (counts)
	 * but wrong velocity (counts/s). I did measure the velocity and position manually and compare to
	 * what amcdrive_update_drives() gave me, I found that even though the position is accurate,
	 * the velocity is off by a factor of 2. Can someone point me out why?
	 */
	servos[0].vel_cps *= 2.0;
	servos[1].vel_cps *= 2.0;

	double position[2], velocity[2];
	position[0] = COUNT_TO_RAD(servos[0].pos_c);
	position[1] = COUNT_TO_RAD(servos[1].pos_c);
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

	// Set Position [rad]
	state.position = SOMATIC_NEW(Somatic__Vector);
	somatic__vector__init(state.position);

	state.position->data = position;
	state.position->n_data = n_modules;

	// Set Velocity [rad/s]
	state.velocity = SOMATIC_NEW(Somatic__Vector);
	somatic__vector__init(state.velocity);

	state.velocity->data = velocity;
	state.velocity->n_data = n_modules;

	// Publish state
	status = somatic_motorstate_publish(&state, state_chan);
	somatic_hard_assert( status == NTCAN_SUCCESS, "Cannot update publish states!\n");

	// // Print state channel size
	// size_t size = somatic__motor_state__get_packed_size(&state);
	// printf("\tmotor_state packed size = %d \n",size);

	// Cleanup heap-allocated part(s) of message
	free(state.position);
	free(state.velocity);
}

int amcdrive_open(servo_vars_t *servos){
    NTCAN_RESULT status;

    status = amcdrive_open_drives(opt_bus_id, opt_mod_id, n_modules,
        REQUEST_TPDO_VELOCITY|REQUEST_TPDO_POSITION|ENABLE_RPDO_CURRENT,
        1000, servos);
    if (status != NTCAN_SUCCESS) {
        perror("amcdrive_open_drives");
        return status;
    }
    servos[1].current_sign = -1;

    status = amcdrive_set_current(&servos[0], 0.0);
    somatic_hard_assert(status == NTCAN_SUCCESS, "zero out 0\n");

    status = amcdrive_set_current(&servos[1], 0.0);
    somatic_hard_assert(status == NTCAN_SUCCESS, "zero out 1\n");

    /// set amc position to zero
    uint8_t rcmd;
    amcdrive_reset_position( servos[0].handle, &rcmd, opt_mod_id[0]);
    amcdrive_reset_position( servos[0].handle, &rcmd, opt_mod_id[1]);

    return 0;
}

