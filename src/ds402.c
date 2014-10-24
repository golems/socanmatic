/* -*- mode: C; c-basic-offset: 4 -*- */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*
 * Copyright (c) 2008-2013, Georgia Tech Research Corporation
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


#include <assert.h>
#include <math.h>
#include "socanmatic.h"
#include "socanmatic/dict402.h"

enum canmat_402_state_val canmat_402_state( const struct canmat_402_drive *drive ) {
    uint16_t s = drive->stat_word;
    if( CANMAT_402_STATE_VAL_OFF_NRDY == (s & CANMAT_402_STATE_VAL_MASK_OFF_NRDY) ) {
        return CANMAT_402_STATE_VAL_OFF_NRDY;
    } else if  ( CANMAT_402_STATE_VAL_OFF_SW_ON_DISABLE == (s & CANMAT_402_STATE_VAL_MASK_OFF_SW_ON_DISABLE) ) {
        return CANMAT_402_STATE_VAL_OFF_SW_ON_DISABLE;
    } else if  ( CANMAT_402_STATE_VAL_OFF_RDY == (s & CANMAT_402_STATE_VAL_MASK_OFF_RDY) ) {
        return CANMAT_402_STATE_VAL_OFF_RDY;
    } else if  ( CANMAT_402_STATE_VAL_ON_OP_DIS == (s & CANMAT_402_STATE_VAL_MASK_ON_OP_DIS) ) {
        return CANMAT_402_STATE_VAL_ON_OP_DIS;
    } else if  ( CANMAT_402_STATE_VAL_ON_OP_EN == (s & CANMAT_402_STATE_VAL_MASK_ON_OP_EN) ) {
        return CANMAT_402_STATE_VAL_ON_OP_EN;
    } else if  ( CANMAT_402_STATE_VAL_ON_QUICK_STOP == (s & CANMAT_402_STATE_VAL_MASK_ON_QUICK_STOP )) {
        return CANMAT_402_STATE_VAL_ON_QUICK_STOP;
    } else if  ( CANMAT_402_STATE_VAL_FAULT_REACTION_ACTIVE ==
                 (s & CANMAT_402_STATE_VAL_MASK_FAULT_REACTION_ACTIVE) ) {
        return CANMAT_402_STATE_VAL_FAULT_REACTION_ACTIVE;
    } else if  ( CANMAT_402_STATE_VAL_FAULT == (s & CANMAT_402_STATE_VAL_MASK_FAULT) ) {
        return CANMAT_402_STATE_VAL_FAULT;
    } else {
        return CANMAT_402_STATE_VAL_UNKNOWN;
    }
}

const char *canmat_402_state_string( enum canmat_402_state_val s ) {
    switch(s) {
    case CANMAT_402_STATE_VAL_OFF_NRDY: return "Not ready to switch on";
    case CANMAT_402_STATE_VAL_OFF_SW_ON_DISABLE: return "Off, switch on disabled";
    case CANMAT_402_STATE_VAL_OFF_RDY: return "Off, ready to switch on";
    case CANMAT_402_STATE_VAL_ON_OP_DIS: return "On, operation disabled";
    case CANMAT_402_STATE_VAL_ON_OP_EN: return "On, operation enabled";
    case CANMAT_402_STATE_VAL_ON_QUICK_STOP: return "Quick stop";
    case CANMAT_402_STATE_VAL_FAULT_REACTION_ACTIVE: return "Fault reaction active";
    case CANMAT_402_STATE_VAL_FAULT: return "Fault";
    case CANMAT_402_STATE_VAL_UNKNOWN: return "?";
    }
    return "?";
}

#define CHECK_STATUS(V)                         \
    {                                           \
    enum canmat_status check_status_r = (V);    \
    if( CANMAT_OK != check_status_r ) {         \
        return check_status_r;                  \
    }                                           \
    }                                           \



enum canmat_status canmat_402_dl_ctrlmask( struct canmat_iface *cif, struct canmat_402_drive *drive ,
                                           uint16_t mask_and, uint16_t mask_or ) {

    uint16_t ctrl = (drive->ctrl_word & mask_and) | mask_or;
    canmat_status_t r = canmat_402_dl_controlword( cif, drive->node_id, ctrl, &drive->abort_code );
    if( CANMAT_OK == r ) drive->ctrl_word = ctrl;
    return r;
}

enum canmat_status canmat_402_set_op_mode( struct canmat_iface *cif, struct canmat_402_drive *drive ,
                                           enum canmat_402_op_mode op_mode );

/**
 * @function canmat_402_init
 * @brief Get control word, status word, position raw value, velocity actual value, velocity target
 */
enum canmat_status canmat_402_init( struct canmat_iface *cif, uint8_t id, struct canmat_402_drive *drive ) {

    drive->node_id = id;

    // control word
    CHECK_STATUS( canmat_402_ul_controlword( cif, drive->node_id, &drive->ctrl_word, &drive->abort_code ) );

    // status word
    CHECK_STATUS( canmat_402_ul_statusword( cif, drive->node_id, &drive->stat_word, &drive->abort_code ) );

    // position
    CHECK_STATUS( canmat_402_ul_position_actual_value( cif, drive->node_id,
                                                       &drive->actual_pos_raw, &drive->abort_code ) );
    // velocity
    CHECK_STATUS( canmat_402_ul_velocity_actual_value( cif, drive->node_id,
                                                       &drive->actual_vel_raw, &drive->abort_code ) );

    // vl target
    CHECK_STATUS( canmat_402_ul_vl_target_velocity( cif, drive->node_id,
                                                    &drive->target_vel_raw, &drive->abort_code ) );

    // operational mode
    {
        int8_t i;
        CHECK_STATUS( canmat_402_ul_modes_of_operation( cif, drive->node_id,
                                                        &i, &drive->abort_code ) );
        drive->op_mode = (enum canmat_402_op_mode)i;
    }

    // limits
    {
        int32_t min, max; // raw values
        CHECK_STATUS( canmat_402_ul_software_position_limit_sub_min_position_range_limit(cif, drive->node_id,
                                                                                         &min, &drive->abort_code) );
        CHECK_STATUS( canmat_402_ul_software_position_limit_sub_max_position_range_limit(cif, drive->node_id,
                                                                                         &max, &drive->abort_code) );

        // FIXME: parameterize limits better
        drive->pos_max_hard = max / drive->pos_factor - 5*M_PI/180;
        drive->pos_min_hard = min / drive->pos_factor + 5*M_PI/180;
        drive->pos_max_soft = drive->pos_max_hard - 5*M_PI/180;
        drive->pos_min_soft = drive->pos_min_hard + 5*M_PI/180;
    }

    // current
    /* { */
    /*     canmat_obj_t *obj = canmat_dict_search_name( &canmat_dict402, "current actual value" ); */
    /*     assert( obj && obj->data_type == CANMAT_DATA_TYPE_INTEGER32 ); */
    /*     r = canmat_obj_ul( cif, drive->node_id, obj, &val ); */
    /*     if( CANMAT_OK != r ) return r; */
    /*     drive->actual_cur_raw = val.i32; */
    /* } */
    return CANMAT_OK;
}

/* static enum canmat_status check_state( struct canmat_iface *cif, struct canmat_402_drive *drive, */
/*                                        enum canmat_402_state_val state ) */
/* { */
/*     // get word */
/*     CHECK_STATUS( canmat_402_ul_statusword( cif, drive->node_id, &drive->stat_word, &drive->abort_code ) ); */
/*     // ensure we're in the expected state */
/*     if( state != canmat_402_state(drive) ) return CANMAT_ERR_DEV; */
/*     else return CANMAT_OK; */
/* } */

enum canmat_status canmat_402_start( struct canmat_iface *cif, struct canmat_402_drive *drive ) {
    // TODO: handle quick-stopped drives
    // TODO: check status word after each dl_control to make sure we really transitioned

    // update status
    int c = 0;
    while( c++ < 100 ) {
        CHECK_STATUS( canmat_402_ul_statusword( cif, drive->node_id, &drive->stat_word, &drive->abort_code ) );
        switch( canmat_402_state(drive) ) {
        case CANMAT_402_STATE_VAL_OFF_NRDY:
            CHECK_STATUS( canmat_send_nmt( cif, drive->node_id, CANMAT_NMT_START_REMOTE ) );
            break;
        case CANMAT_402_STATE_VAL_OFF_SW_ON_DISABLE:
            CHECK_STATUS( canmat_402_dl_ctrlmask( cif, drive,
                                                  CANMAT_402_CTRLCMD_MASK_AND_SHUTDOWN,
                                                  CANMAT_402_CTRLCMD_MASK_OR_SHUTDOWN ) );
            break;
        case CANMAT_402_STATE_VAL_OFF_RDY:
            CHECK_STATUS( canmat_402_dl_ctrlmask( cif, drive,
                                                  CANMAT_402_CTRLCMD_MASK_AND_SWITCH_ON,
                                                  CANMAT_402_CTRLCMD_MASK_OR_SWITCH_ON ) );
            break;
        case CANMAT_402_STATE_VAL_ON_OP_DIS:
            CHECK_STATUS( canmat_402_dl_ctrlmask( cif, drive,
                                                  CANMAT_402_CTRLCMD_MASK_AND_ENABLE_OP,
                                                  CANMAT_402_CTRLCMD_MASK_OR_ENABLE_OP ) );
            break;
        case CANMAT_402_STATE_VAL_ON_OP_EN:
            return CANMAT_OK;
        case CANMAT_402_STATE_VAL_ON_QUICK_STOP:
            CHECK_STATUS( canmat_402_dl_ctrlmask( cif, drive,
                                                  CANMAT_402_CTRLCMD_MASK_AND_DISABLE_VOLTAGE,
                                                  CANMAT_402_CTRLCMD_MASK_OR_DISABLE_VOLTAGE ) );
            break;
        case CANMAT_402_STATE_VAL_FAULT_REACTION_ACTIVE:
        case CANMAT_402_STATE_VAL_FAULT:
            CHECK_STATUS( canmat_402_dl_ctrlmask( cif, drive,
                                                  CANMAT_402_CTRLCMD_MASK_AND_SHUTDOWN,
                                                  CANMAT_402_CTRLCMD_MASK_OR_SHUTDOWN ) );
            // Schunk drives seem to need this start NMT to get over the error
            CHECK_STATUS( canmat_send_nmt( cif, drive->node_id, CANMAT_NMT_START_REMOTE ) );
            // reset error
            CHECK_STATUS( canmat_402_dl_ctrlmask( cif, drive,
                                                  CANMAT_402_CTRLCMD_MASK_AND_RESET_FAULT,
                                                  CANMAT_402_CTRLCMD_MASK_OR_RESET_FAULT ) );
            break;
        case CANMAT_402_STATE_VAL_UNKNOWN:
        default:
            return CANMAT_ERR_PROTO;
        }
        // update status
        //CHECK_STATUS( canmat_402_ul_statusword( cif, drive->node_id, &drive->stat_word, &drive->abort_code ) );
    }
    return CANMAT_ERR_DEV;

}

enum canmat_status canmat_402_stop( struct canmat_iface *cif, struct canmat_402_drive *drive ) {
    return canmat_402_dl_ctrlmask( cif, drive, CANMAT_402_CTRLCMD_MASK_AND_SHUTDOWN, CANMAT_402_CTRLCMD_MASK_OR_SHUTDOWN );
}

enum canmat_status canmat_402_probe_pdo(
    struct canmat_iface *cif, struct canmat_pdo_descriptor_table tab,
    const struct canmat_402_drive *drive )
{
    return CANMAT_OK;

}

/**
 * @function canmat_402_set_op_mode
 * @brief Set operation mode (velocity/profile position)
 */
enum canmat_status canmat_402_set_op_mode( struct canmat_iface *cif, struct canmat_402_drive *drive,
                                           enum canmat_402_op_mode op_mode ) {
    // Depend on a user-configurable RPDO for now
    if( drive->rpdo_user < 0 || drive->rpdo_user > 0xFF ) return CANMAT_ERR_PARAM;

    // Check that operation mode is handled
    struct canmat_obj *ref_obj = NULL;
    uint16_t ctrl_and = 0xFFFF, ctrl_or = 0;
    canmat_scalar_t ref_val = {0};
    switch( op_mode ) {
    case CANMAT_402_OP_MODE_VELOCITY:
        ref_obj = CANMAT_402_OBJ_VL_TARGET_VELOCITY;
        ref_val.u16 = 0;
        ctrl_and = 0xFFFF;
        ctrl_or = ( CANMAT_402_CTRLMASK_VL_RFG_ENABLE |
                    CANMAT_402_CTRLMASK_VL_RFG_UNLOCK |
                    CANMAT_402_CTRLMASK_VL_RFG_USE_REF );
        break;
    case CANMAT_402_OP_MODE_PROFILE_POSITION:
    	ref_obj = CANMAT_402_OBJ_TARGET_POSITION;

    	// Reference value should be the same as the current position
    	int32_t current_position_raw; uint32_t error;
    	canmat_402_ul_position_actual_value( cif, drive->node_id,
    										 &current_position_raw,
    										 &error );
    	ref_val.i32 = current_position_raw;
    	ctrl_and = 0xFF8F; // Bits 4,5 and 6 zeros
    	ctrl_or = ( CANMAT_402_CTRLMASK_PP_NEW_SET_POINT ); //|
    				//CANMAT_402_CTRLMASK_PP_CHANGE_SET_IMMEDIATELY | // Finish current positioning
    				//CANMAT_402_CTRLMASK_PP_ABS_REL ); // Absolute position
    	break;
    default: return CANMAT_ERR_PARAM;
    }
    if( NULL == ref_obj ) return CANMAT_ERR_PARAM;

    // Set mode unless already set
    if( op_mode != drive->op_mode ) {
        // Make sure the drive is halted
        if( ! (drive->ctrl_word & CANMAT_402_CTRLMASK_HALT) ) {
            return CANMAT_ERR_MOTION;
        }

        // set the mode
        CHECK_STATUS( canmat_402_dl_modes_of_operation( cif, drive->node_id, op_mode,
                                                        &(drive->abort_code) ) );
        drive->op_mode = op_mode;

        // Set reference to no-motion value
        CHECK_STATUS( canmat_obj_dl( cif, drive->node_id, ref_obj, &ref_val, &(drive->abort_code) ) );
    }


    // Map the RPDO
    const struct canmat_obj *obj_ar[1] = {ref_obj};
    canmat_status_t r = canmat_pdo_remap( cif, drive->node_id, (uint8_t)(drive->rpdo_user), CANMAT_DL,
                                          CANMAT_PDO_TRANSMISSION_TYPE_EVENT_DRIVEN, -1, -1,
                                          1, obj_ar, &(drive->abort_code) );

    // set control word
    CHECK_STATUS( canmat_402_dl_ctrlmask( cif, drive,
                                          ctrl_and,
                                          ctrl_or ) );
    return r;
}
