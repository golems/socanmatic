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

#ifndef SOCANMATIC_DICT_H
#define SOCANMATIC_DICT_H

#ifdef __cplusplus
extern "C" {
#endif

/// Access type for an object
typedef enum canmat_access_type {
    CANMAT_ACCESS_UNKNOWN,    ///< Unknown access type
    CANMAT_ACCESS_RO,         ///< Read only
    CANMAT_ACCESS_WO,         ///< Write only
    CANMAT_ACCESS_RW,         ///< Read-Write
    CANMAT_ACCESS_RWR,        ///< Read-Write on process input
    CANMAT_ACCESS_RWW,        ///< Read-Write on process output
    CANMAT_ACCESS_CONST       ///< Constant data
} canmat_access_type_t;


typedef uint8_t  CANMAT_UNSIGNED8;
typedef uint16_t CANMAT_UNSIGNED16;
typedef uint32_t CANMAT_UNSIGNED32;
typedef int8_t   CANMAT_INTEGER8;
typedef int16_t  CANMAT_INTEGER16;
typedef int32_t  CANMAT_INTEGER32;
typedef float    CANMAT_REAL32;
typedef double   CANMAT_REAL64;


/** Description of a CiA Object */
typedef struct canmat_obj {
    /** Index of the object */
    const uint16_t index;

    /** Subindex of the object */
    const uint8_t subindex;

    /* Name of the object */
    const char *parameter_name;

    /** Number of sub-indices for an index.
     *  May be empty or absent of object has no sub-objects. */
    const uint8_t sub_number;


    /** Access type for object.
     */
    enum canmat_access_type access_type;


    /** The object code
     */
    enum canmat_object_type object_type;

    /** index of the data type of the object in the object dictionary.
     */
    enum canmat_data_type data_type;

    /** Can object be mapped into a PDO */
    const unsigned pdo_mapping : 1;

    struct canmat_code_descriptor *value_descriptor;
    struct canmat_code_descriptor *mask_descriptor;
} canmat_obj_t;

typedef struct canmat_dict_name_tree {
    const char *parameter_name;
    size_t i;
} canmat_dict_name_tree_t ;

/** Description of a CiA Dictionary */
typedef struct canmat_dict {
    size_t length;

    canmat_dict_name_tree_t *btree_name;

    /** Array of object descriptors
     * sorted in ascending order by index
     */
    struct canmat_obj *obj;
} canmat_dict_t;

typedef union canmat_scalar {
    uint8_t u8;
    int8_t  i8;

    uint16_t u16;
    int16_t  i16;

    uint32_t u32;
    int32_t  i32;

    float r32;

    uint8_t  byte[4];
} canmat_scalar_t;

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
