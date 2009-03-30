
#include <ntcan.h>
#include "byteorder.h"

typedef struct {
  int16_t k_i;    // feedback position interp
  uint32_t k_s;    // switch freq
  uint16_t k_p;    // max rated current
  int32_t pos_cnt; // position in counts
  double vel_cps;  //velocity in counts per second
  double i_ref; // target current
  double i_act; // actual current
  
  // PDO->COB mappings
  uint16_t rpdo_position;
  uint16_t rpdo_velocity;
  uint16_t rpdo_current;
  
  uint16_t tpdo_position;
  uint16_t tpdo_velocity;
  uint16_t tpdo_current;
} servo_vars_t;

#define ENABLE_RPDO_POSITION 0x01
#define ENABLE_RPDO_VELOCITY 0x02
#define ENABLE_RPDO_CURRENT  0x04

#define REQUEST_TPDO_POSITION 0x10
#define REQUEST_TPDO_VELOCITY 0x20
#define REQUEST_TPDO_CURRENT  0x40

NTCAN_RESULT amcdrive_init(uint drive, uint identifier, uint pdos, uint update_freq, NTCAN_HANDLE *handle, servo_vars_t *drive_info);

