/*
 * Copyright (c) 2008, Georgia Tech Research Corporation
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

#include <ntcan.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file amccan.h
 *
 * \brief Header file for AMC CAN servo drives
 *
 * \author Neil Dantam
 *
 */


/// Values to assign to the control word CANopen object
typedef enum {
  AMCCAN_CONTROL_STATE_RESET = 0x80,
  AMCCAN_CONTROL_STATE_SHUTDOWN = 0x06,
  AMCCAN_CONTROL_STATE_ON = 0x07,
  AMCCAN_CONTROL_STATE_ENABLE = 0x0f
} amccan_control_state_t;

/// Values to assign to the operating mode CANopen object
typedef enum {
  AMCCAN_OP_MODE_POSITION = 0x1,
  AMCCAN_OP_MODE_VELOCITY = 0x3,
  AMCCAN_OP_MODE_CURRENT  = 0x4,
  AMCCAN_OP_MODE_HOMING   = 0x6,
  AMCCAN_OP_MODE_PVT      = 0x7,
  AMCCAN_OP_MODE_CUSTOM   = 0xff
} amccan_op_mode_t;


#define AMCCAN_INDEX_NODE_ID         0x100b
#define AMCCAN_SUBINDEX_NODE_ID      0x00;
#define AMCCAN_INDEX_IDENTITY        0x1018
#define AMCCAN_SUBINDEX_IDENTITY     0x01
#define AMCCAN_INDEX_CONTROLWORD     0x6040
#define AMCCAN_SUBINDEX_CONTROLWORD  0x00
#define AMCCAN_INDEX_OP_MODE         0x6060
#define AMCCAN_SUBINDEX_OP_MODE      0x00

#define AMCCAN_INDEX_TARGET_CURRENT 0x6071
#define AMCCAN_SUBINDEX_TARGET_CURRENT 0x0

#define AMCCAN_INDEX_ACTUAL_VELOCITY 0x606C
#define AMCCAN_SUBINDEX_ACTUAL_VELOCITY 0x0
#define AMCCAN_INDEX_ACTUAL_POSITION 0x6064
#define AMCCAN_SUBINDEX_ACTUAL_POSITION 0x0

#define AMCCAN_INDEX_TIMER1_CYCLE 0x2120
#define AMCCAN_SUBINDEX_TIMER1_CYCLE 0x0
#define AMCCAN_INDEX_TIMER2_CYCLE 0x2123
#define AMCCAN_SUBINDEX_TIMER2_CYCLE 0x0

#define AMCCAN_INDEX_TIMER1_TPDOS 0x2121
#define AMCCAN_SUBINDEX_TIMER1_TPDOS 0x0
#define AMCCAN_INDEX_TIMER2_TPDOS 0x2124
#define AMCCAN_SUBINDEX_TIMER2_TPDOS 0x0

#define AMCCAN_INDEX_BOARD_INFO 0x20D8
#define AMCCAN_SUBINDEX_BOARD_SWITCH_FREQ 0x24
#define AMCCAN_SUBINDEX_MAX_PEAK_CURRENT 0x0C

#define AMCCAN_INDEX_FEEDBACK_PARM 0x2032
#define AMCCAN_SUBINDEX_FEEDBACK_POS_INTERP 0x8

#define AMCCAN_INDEX_POS_LIMIT 0x2039
#define AMCCAN_SUBINDEX_POS_LIMIT_MEASURED 0x01



typedef enum {
  AMCCAN_RPDO_1 = 0,
  AMCCAN_RPDO_2,
  AMCCAN_RPDO_3,
  AMCCAN_RPDO_4,
  AMCCAN_RPDO_5,
  AMCCAN_RPDO_21,
  AMCCAN_RPDO_22,
  AMCCAN_RPDO_24,
  AMCCAN_RPDO_23,
  AMCCAN_TPDO_1,
  AMCCAN_TPDO_3,
  AMCCAN_TPDO_4,
  AMCCAN_TPDO_5,
  AMCCAN_TPDO_21,
  AMCCAN_TPDO_22,
  AMCCAN_TPDO_23,
  AMCCAN_TPDO_24,
  AMCCAN_TPDO_25,
  AMCCAN_TPDO_USER, // 26
} amccan_pdo_t;

/// subindices for SDOs used ton control PDO communications
typedef enum {
  AMCCAN_SUBINDEX_PDO_COM_ID = 0x1,
  AMCCAN_SUBINDEX_PDO_COM_TRANS = 0x2
} amccan_pdo_com_subindex_t;

typedef enum {
  AMCCAN_PDO_TRANS_SYNC_ACYC = 0x00,
  AMCCAN_PDO_TRANS_SYNC_CYC = 0x01,
  AMCCAN_PDO_TRANS_SYNC_RTR = 0xFC,
  AMCCAN_PDO_TRANS_ASYNC_RTR = 0xFC,
  AMCCAN_PDO_TRANS_ASYNC = 0xFE
} amccan_pdo_trans_t;

/** sets the control word.

    This function blocks till a response is received or we have a
    timeout.

    \pre h is valid can handle listing for sdo responses from node
 */
NTCAN_RESULT amccan_dl_control( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node,
                                   amccan_control_state_t state );

/** sets the operating mode.

    This function blocks till a response is received or we have a
    timeout.

    \pre h is valid can handle listing for sdo responses from node
 */
NTCAN_RESULT amccan_dl_op_mode( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node,
                               amccan_op_mode_t state );

/** sets pdo cob-id
 */
NTCAN_RESULT amccan_dl_pdo_id( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node, amccan_pdo_t pdo,
                               uint16_t id, int disable, int rtr );

/// sets the transmission type of a node
NTCAN_RESULT amccan_dl_pdo_trans( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node, amccan_pdo_t pdo,
                                  amccan_pdo_trans_t trans, int sync_interval );

/** sets up a pdo mapping.

    Must map pdos in numerical order.
 */
NTCAN_RESULT amccan_dl_pdo_map( NTCAN_HANDLE, uint8_t *rcmd, uint8_t node, amccan_pdo_t pdo,
                                int mapping_obj, uint16_t index, uint8_t subindex,
                                uint8_t len );

/** Sets timer 1
    \param Cycle time of timer 1 in milliseconds
    \param pdos TPDOs which timer1 will trigger
    \param pdo_len length of parameter pdos array
    \param timer to use (1 or 2)
 */
NTCAN_RESULT amccan_dl_timer( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node, int timer,
                               uint32_t cycle_time, int *pdos, int pdo_len );

/** Decodes a velocity value received from the drive.

 \return value in counts per second*/
double amccan_decode_ds1( int32_t i, uint16_t k_i, uint32_t k_s );

/** Decodes a current value received from the drive. 

 \return value in amps*/
double amccan_decode_dc1( int32_t i_dc1, uint32_t k_p );


/** Sets position of the drive.
 */
NTCAN_RESULT amccan_position_set( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node,
                                  int32_t pos );

/** Sets drive to current control and sets the current. */
NTCAN_RESULT amccan_pdo_current( NTCAN_HANDLE h, uint16_t cob_id, double amps,
                                 uint16_t k_i, uint32_t k_s );

/** Returns switching frequency of the drive in kHz */
uint32_t amcccan_decode_pbf( uint32_t pbf );

#ifdef __cplusplus
}
#endif

