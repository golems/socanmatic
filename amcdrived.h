#define AMCDRIVE_CONTROL_CURRENT  1
#define AMCDRIVE_EMERGENCY_STOP   2
#define AMCDRIVE_RESUME_OPERATION 3

typedef struct drive_data {
    double ts;
    int32_t position;
    int32_t velocity;
    double current;
} drive_data_t;

typedef struct drive_control {
    char control_type;
    double current;
} drive_control_t;


