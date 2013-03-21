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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>


/* Assume linux socketcan for now */
#include <linux/can.h>
#include <linux/can/raw.h>

#include <inttypes.h>

#include "socia.h"
#include "socia_private.h"



const char *socia_sdo_abort_code2str( uint32_t code ) {
    switch( code ) {
    case 0x05030000: return "Toggle bit not alternated";
    case 0x05040000: return "SDO protocol timed out";
    case 0x05040001: return "Client/server command specifier not valid or unknown";
    case 0x05040002: return "Invalid block size (block mode only)";
    case 0x05040003: return "Invalid sequence number (block mode only)";
    case 0x05040004: return "CRC error (block mode only)";
    case 0x05040005: return "Out of memory";
    case 0x06010000: return "Unsupported access to an object";
    case 0x06010001: return "Attempt to read a write only object";
    case 0x06010002: return "Attempt to write a read only object";
    case 0x06020000: return "Object does not exist in the object dictionary";
    case 0x06040041: return "Object cannot be mapped to the PDO";
    case 0x06040042: return "The number and length of the objects to be mapped would exceed PDO length";
    case 0x06040043: return "General parameter incompatibility reason";
    case 0x06040047: return "General internal incompatibility in the device";
    case 0x06060000: return "Access failed due to an hardware error";
    case 0x06070010: return "Data type does not match, length of service parameter does not match";
    case 0x06070012: return "Data type does not match, length of service parameter too high";
    case 0x06070013: return "Data type does not match, length of service parameter too low";
    case 0x06090011: return "Sub-index does not exist";
    case 0x06090030: return "Invalid value for parameter (download only)";
    case 0x06090031: return "Value of parameter written too high (download only)";
    case 0x06090032: return "Value of parameter written too low (download only)";
    case 0x06090036: return "Maximum value is less than minimum value";
    case 0x060A0023: return "Resource not available: SDO connection";
    case 0x08000000: return "General error";
    case 0x08000020: return "Data cannot be transferred or stored to the application";
    case 0x08000021: return "Data cannot be transferred or stored to the application because of local control";
    case 0x08000022: return "Data cannot be transferred or stored to the application "
            "because of the present device state";
    case 0x08000023: return "Object dictionary dynamic generation fails or no object dictionary "
            "is present (e.g. object dictionary is generated from file and "
            "generation fails because of an file error)";
    case 0x08000024: return "No data available";
    default: return "Reserved";
    }
}
