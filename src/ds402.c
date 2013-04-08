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
#include "socanmatic.h"
#include "socanmatic_private.h"

static int p_controlword(canmat_scalar_t val);
static int p_statusword(canmat_scalar_t val);

static const struct  {
    const char *name;
    canmat_obj_print_fun * fun;
} tab[] = { {"controlword", &p_controlword},
            {"statusword", &p_statusword},
            {NULL, NULL} };

struct mask_format {
    const char *name;
    unsigned mask;
};

int print_masks( struct mask_format fmt[], unsigned u ) {
    int r = 0;
    size_t maxlen = 0;
    for( size_t i = 0; fmt[i].mask && fmt[i].name; i ++ ) {
        size_t n = strlen(fmt[i].name);
        if( n > maxlen ) maxlen = n;
    }

    for( size_t i = 0; fmt[i].mask && fmt[i].name; i ++ ) {
        size_t space_size = maxlen + 1 - strlen(fmt[i].name);
        char space[ space_size + 1 ];
        memset( space, ' ', space_size );
        space[space_size] = '\0';
        r += printf( "> %s:%s%d\n", fmt[i].name, space,
                     (u & fmt[i].mask) ? 1 : 0 );
    }
    return r;
}

canmat_obj_print_fun* canmat_402_print_lookup( const char *name ) {
    for( size_t i = 0; tab[i].name && tab[i].fun; i ++ ) {
        if( 0 == strcasecmp( name, tab[i].name ) ) {
            return tab[i].fun;
        }
    }
    return NULL;
}

static int p_controlword(canmat_scalar_t val) {
    struct mask_format fmt[] = {
        { "switch_on", CANMAT_402_CTRLMASK_SWITCH_ON},
        { "enable_voltage", CANMAT_402_CTRLMASK_ENABLE_VOLTAGE},
        { "quick_stop", CANMAT_402_CTRLMASK_QUICK_STOP},
        { "enable_op", CANMAT_402_CTRLMASK_ENABLE_OPERATION},
        { "reset_fault", CANMAT_402_CTRLMASK_RESET_FAULT},
        { "halt", CANMAT_402_CTRLMASK_HALT},
        {NULL, 0} };
    int r = printf("0x%"PRIx16"\n", val.u16);
    r += print_masks( fmt, val.u16 );
    return r;
}
static int p_statusword(canmat_scalar_t val) {
    struct mask_format fmt[] = {
        {"ready_to_switch_on", CANMAT_402_STATMASK_READY_TO_SWITCH_ON},
        {"switched_on", CANMAT_402_STATMASK_SWITCHED_ON},
        {"op_enabled", CANMAT_402_STATMASK_OP_ENABLED},
        {"fault", CANMAT_402_STATMASK_FAULT},
        {"voltage_enabled", CANMAT_402_STATMASK_VOLTAGE_ENABLED},
        {"quick_stop", CANMAT_402_STATMASK_QUICK_STOP},
        {"switch_on_disabled", CANMAT_402_STATMASK_SWITCH_ON_DISABLED},
        {"warning", CANMAT_402_STATMASK_WARNING},
        {"remote", CANMAT_402_STATMASK_REMOTE},
        {"target_reached", CANMAT_402_STATMASK_TARGET_REACHED},
        {"internal_limit_active", CANMAT_402_STATMASK_INTERNAL_LIMIT_ACTIVE},
        {NULL, 0} };
    int r = printf("0x%"PRIx16"\n", val.u16);
    r += print_masks( fmt, val.u16 );
    return 0;
}
