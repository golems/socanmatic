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

#ifndef SOCANMATIC_DICT_FUN_H
#define SOCANMATIC_DICT_FUN_H

#ifdef __cplusplus
extern "C" {
#endif


canmat_status_t canmat_obj_ul (
    canmat_iface_t *cif, uint8_t node, const canmat_obj_t *obj, canmat_scalar_t *val );

canmat_status_t canmat_obj_dl (
    canmat_iface_t *cif, uint8_t node, const canmat_obj_t *obj, const canmat_scalar_t *val,
    uint32_t *err_val );

canmat_status_t canmat_obj_dl_str (
    canmat_iface_t *cif, uint8_t node, const canmat_obj_t *obj, const char *val, uint32_t *err_val );

/* Return the item in dict with given name */
canmat_obj_t *canmat_dict_search_name( const struct canmat_dict *dict, const char *name );

/* Return the item in dict with given index and subindex */
canmat_obj_t *canmat_dict_search_index (
    const struct canmat_dict *dict, uint16_t idx, uint8_t subindex );

/* canmat_status_t canmat_dict_ul ( */
/*     canmat_iface_t *cif, const struct canmat_dict *dict, */
/*     uint8_t node, const char *name, */
/*     canmat_scalar_t *val ); */

/* Parse str based on provided type and store in val */
canmat_status_t canmat_typed_parse( enum canmat_data_type type, const char *str, canmat_scalar_t *val );

int canmat_obj_print( FILE *f, const canmat_obj_t *obj, canmat_scalar_t *val );

typedef int canmat_obj_print_fun(canmat_scalar_t);


#ifdef __cplusplus
}
#endif
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
#endif //SOCANMATIC_DICT_H
