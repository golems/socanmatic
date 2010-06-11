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

/**
 * \file amccan.c
 *
 * \brief routines to communicate with AMC drives
 *
 * \author Neil Dantam
 *
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ntcan.h>
#include "ntcanopen.h"
#include "include/amccan.h"

static uint16_t pdo_com_index[] = { /* From p28 */
  0x1400, //RPDO 1
  0x1401, //RPDO 2
  0x1402, //RPDO 3
  0x1403, //RPDO 4
  0x1404, //RPDO 5
  0x1414, //RPDO 21
  0x1415, //RPDO 22
  0x1416, //RPDO 23
  0x1417, //RPDO 24
  0x1800, //TPDO 1
  0x1802, //TPDO 3
  0x1803, //TPDO 4
  0x1804, //TPDO 5
  0x1814, //TPDO 21
  0x1815, //TPDO 22
  0x1816, //TPDO 23
  0x1817, //TPDO 24
  0x1818, //TPDO 25
  0x1819  //TPDO 26
};


static uint16_t pdo_map_index[] = { /* From p28 */
  0x1600, //RPDO 1
  0x1601, //RPDO 2
  0x1602, //RPDO 3
  0x1603, //RPDO 4
  0x1604, //RPDO 5
  0x1614, //RPDO 21
  0x1615, //RPDO 22
  0x1616, //RPDO 23
  0x1617, //RPDO 24
  0x1A00, //TPDO 1
  0x1A02, //TPDO 3
  0x1A03, //TPDO 4
  0x1A04, //TPDO 5
  0x1A14, //TPDO 21
  0x1A15, //TPDO 22
  0x1A16, //TPDO 23
  0x1A17, //TPDO 24
  0x1A18, //TPDO 25
  0x1A19  //TPDO 26
};

static int pdo_is_valid( int pdo ) {
  return AMCCAN_RPDO_1 <= pdo && AMCCAN_TPDO_USER >= pdo;
}

//static int pdo_is_rpdo( int pdo ) {
//  return AMCCAN_RPDO_1 <= pdo && AMCCAN_RPDO_23 >= pdo;
//}

static int pdo_is_tpdo( uint32_t pdo ) {
  return AMCCAN_TPDO_1 <= pdo && AMCCAN_TPDO_USER >= pdo;
}

// Set Control Word
NTCAN_RESULT amccan_dl_control( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node,
                                   amccan_control_state_t state ) {
  return canOpenSDOWriteWait_dl_u16( h, rcmd, node,
                                     AMCCAN_INDEX_CONTROLWORD,
                                     AMCCAN_SUBINDEX_CONTROLWORD,
                                     state );
}

// Set Mode of Operation
NTCAN_RESULT amccan_dl_op_mode( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node,
                                amccan_op_mode_t value ) {
  return canOpenSDOWriteWait_dl_u16( h, rcmd, node,
                                     AMCCAN_INDEX_OP_MODE,
                                     AMCCAN_SUBINDEX_OP_MODE,
                                     value );
}

NTCAN_RESULT amccan_dl_pdo_id( NTCAN_HANDLE h, uint8_t *rcmd,
                            uint8_t node, amccan_pdo_t pdo,
                            uint16_t id, uint32_t disable, uint32_t rtr ) {
  assert( 0x181 <= id );
  assert( 0x57F >= id );
  assert( pdo_is_valid(pdo) );
  uint32_t u = id;
  u |= (disable & 1) << 31;
  u |= (rtr & 1) << 30;
  return canOpenSDOWriteWait_dl_u32( h, rcmd, node,
                                     pdo_com_index[pdo],
                                     AMCCAN_SUBINDEX_PDO_COM_ID,
                                     u );
}


NTCAN_RESULT amccan_dl_pdo_trans( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node,
                                  amccan_pdo_t pdo,
                                  amccan_pdo_trans_t trans,
                                  uint32_t sync_interval) {
  assert( pdo_is_valid( pdo ) );
  if( AMCCAN_PDO_TRANS_SYNC_CYC == trans ) {
    trans += sync_interval - 1;
    assert(trans >= 0x1 && trans <= 0xF0);
  }
  return canOpenSDOWriteWait_dl_u8( h, rcmd, node,
                                    pdo_com_index[pdo],
                                    AMCCAN_SUBINDEX_PDO_COM_TRANS,
                                    trans );
}

static int pdo_is_user_mappable( amccan_pdo_t pdo ) {
  switch( pdo ) {
  case AMCCAN_RPDO_1: return 0;
  case AMCCAN_RPDO_2: return 0;
  case AMCCAN_RPDO_3: return 0;
  case AMCCAN_RPDO_4: return 0;
  case AMCCAN_RPDO_5: return 0;
  case AMCCAN_RPDO_21: return 0;
  case AMCCAN_RPDO_22: return 0;
  case AMCCAN_RPDO_23: return 0;
  case AMCCAN_TPDO_1: return 0;
  case AMCCAN_TPDO_3: return 0;
  case AMCCAN_TPDO_4: return 0;
  case AMCCAN_TPDO_5: return 0;
  case AMCCAN_TPDO_21: return 0;
  case AMCCAN_TPDO_23: return 0;
  case AMCCAN_TPDO_24: return 0;
  case AMCCAN_TPDO_25: return 0;
  case AMCCAN_TPDO_USER: return 1;
  default: return 0;
  }
}

NTCAN_RESULT amccan_dl_pdo_map( NTCAN_HANDLE h, uint8_t *rcmd,
                                uint8_t node, amccan_pdo_t pdo,
                                uint8_t mapping_obj,
                                uint16_t index, uint8_t subindex,
                                uint8_t len ) {
  assert( pdo_is_valid( pdo ) );
  assert( pdo_is_user_mappable( pdo ) );
  assert( mapping_obj >= 1 && mapping_obj <= 8 );
  uint32_t u = len;
  u |= (uint32_t)(subindex << 8);
  u |= (uint32_t)((index & 0xFF) << 16);
  u |= (uint32_t)((index >> 8 ) << 24);

  NTCAN_RESULT ntr;
  ntr = canOpenSDOWriteWait_dl_u32( h, rcmd, node,
                                    pdo_map_index[pdo],
                                    mapping_obj,
                                    u );
  // Something is wierd with the function call
  if( NTCAN_SUCCESS != ntr ) return ntr;
  ntr = canOpenSDOWriteWait_dl_u32( h, rcmd, node,
                                    pdo_map_index[pdo],
                                    0,
                                    mapping_obj );
  return ntr;
}

NTCAN_RESULT amccan_dl_timer( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node, int timer,
                               uint32_t cycle_time, uint32_t *pdos,
                               int pdo_len ) {
  uint16_t index_cycle, index_tpdos;
  uint8_t subindex_cycle, subindex_tpdos;
  switch( timer ) {
  case 1:
    index_cycle = AMCCAN_INDEX_TIMER1_CYCLE;
    subindex_cycle = AMCCAN_SUBINDEX_TIMER1_CYCLE;
    index_tpdos = AMCCAN_INDEX_TIMER1_TPDOS;
    subindex_tpdos = AMCCAN_SUBINDEX_TIMER1_TPDOS;
    break;
  case 2:
    index_cycle = AMCCAN_INDEX_TIMER2_CYCLE;
    subindex_cycle = AMCCAN_SUBINDEX_TIMER2_CYCLE;
    index_tpdos = AMCCAN_INDEX_TIMER2_TPDOS;
    subindex_tpdos = AMCCAN_SUBINDEX_TIMER2_TPDOS;
    break;
  default:
    assert(0);
  }
  uint32_t tpdos = 0;
  assert( pdo_len < AMCCAN_TPDO_USER );
  {
    int i;
    for( i = 0; i < pdo_len; i ++ ) {
      assert( pdo_is_tpdo( pdos[i] ) );
      tpdos |= (uint32_t)(1 << (pdos[i] - AMCCAN_TPDO_1));
    }
  }
  NTCAN_RESULT ntr;
  ntr = canOpenSDOWriteWait_dl_u32( h, rcmd, node, index_cycle,
                                    subindex_cycle,
                                    cycle_time );
  if( NTCAN_SUCCESS != ntr ) return ntr;
  ntr = canOpenSDOWriteWait_dl_u32( h, rcmd, node, index_tpdos,
                                    subindex_tpdos, tpdos );
  return ntr;
}

double amccan_decode_ds1( int32_t i, uint16_t k_i, uint32_t k_s ) {
  double i_d = i, k_i_d = k_i, k_s_d = k_s;
  return i_d / ( (1 << 17) / (k_i_d * k_s_d ) );
}

double amccan_decode_dc1( int32_t i_dc1, uint32_t k_p ) {
  double ii = i_dc1;
  double divisor = (1 << 13) / k_p;
  return (ii / divisor) / 10.0;
}


int32_t amccan_encode_ds1( double amps, uint16_t k_i, uint32_t k_s ) {
  //printf("Encoding: %f amps, %d k_i, %d k_s\n", amps, k_i, k_s );
  double k_i_d = k_i, k_s_d = k_s;
  double i =  (amps * ( (1 << 17)) / (k_i_d * k_s_d ) );
  int32_t r = (int32_t)i;
  if( r >= INT32_MAX ) r = INT32_MAX;
  else if ( r <= INT32_MIN) r = INT32_MIN;

  return r;
}

//NTCAN_RESULT amccan_pdo_current( NTCAN_HANDLE h, uint16_t cob_id, double amps,
//                                 uint16_t k_i, uint32_t k_s ) {
//  CMSG msg;
//  int32_t enc_amps = amccan_encode_ds1( amps, k_i, k_s );
//  //printf("Setting current: %f, code: %d\n", amps, enc_amps );
//  msg.id = cob_id;
//  // control word
//  msg.data[0] = 0x0f;
//  msg.data[1] = 0x00;
//  // current
//  msg.data[2] = enc_amps & 0xff;
//  msg.data[3] = (enc_amps >> 8) & 0xff;
//  msg.len = 4;
//  int num = 1;
//  //printf("p: Sending Message [%x] %.2x:%.2x:%.2x:%.2x:%.2x:%.2x:%.2x:%.2x \n\r", msg.id, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
//  return canWrite( h, &msg, &num, NULL );
//}

uint32_t amcccan_decode_pbf( uint32_t pbf  ) {
  return pbf / (1 << 16 );
}

NTCAN_RESULT amccan_dl_timer1( NTCAN_HANDLE h, uint8_t *rcmd, uint8_t node,
                               uint32_t cycle_time, uint32_t *pdos,
                               int pdo_len ) {
  uint32_t tpdos = 0;
  assert( pdo_len < AMCCAN_TPDO_USER );
  {
    int i;
    for( i = 0; i < pdo_len; i ++ ) {
      assert( pdo_is_tpdo( pdos[i] ) );
      tpdos |= (uint32_t)(1 << (pdos[i] - AMCCAN_TPDO_1));
    }
  }
  NTCAN_RESULT ntr;
  ntr = canOpenSDOWriteWait_dl_u32( h, rcmd, node, AMCCAN_INDEX_TIMER1_CYCLE,
                                    AMCCAN_SUBINDEX_TIMER1_CYCLE,
                                    cycle_time );
  if( NTCAN_SUCCESS != ntr ) return ntr;
  ntr = canOpenSDOWriteWait_dl_u32( h, rcmd, node, AMCCAN_INDEX_TIMER1_TPDOS,
                                    AMCCAN_SUBINDEX_TIMER1_TPDOS, tpdos );
  return ntr;
}
