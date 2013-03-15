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

#ifndef SOCIA_DICT_H
#define SOCIA_DICT_H

#ifdef __cplusplus
extern "C" {
#endif

/// Access type for an object
typedef enum socia_access_type {
    SOCIA_ACCESS_UNKNOWN,    ///< Unknown access type
    SOCIA_ACCESS_RO,         ///< Read only
    SOCIA_ACCESS_WO,         ///< Write only
    SOCIA_ACCESS_RW,         ///< Read-Write
    SOCIA_ACCESS_RWR,        ///< Read-Write on process input
    SOCIA_ACCESS_RWW,        ///< Read-Write on process output
} socia_access_type_t;


/** Object Dictionary Object Definition
 *  see CiA 301
 */
typedef enum socia_object_type {
    SOCIA_OBJ_CODE_NULL       = 0, ///< no data fields
    SOCIA_OBJ_CODE_DOMAIN     = 2, ///< large variable amount of data
    SOCIA_OBJ_CODE_DEFTYPE    = 5, ///< A type definition
    SOCIA_OBJ_CODE_DEFSTRUCT  = 6, ///< A struct definition
    SOCIA_OBJ_CODE_VAR        = 7, ///< A single value
    SOCIA_OBJ_CODE_ARRAY      = 8, ///< An array
    SOCIA_OBJ_CODE_RECORD     = 9  ///< A record type
} socia_object_type_t;

/** Data type for an object
 *  see CiA 301
 */
typedef enum socia_data_type {
    SOCIA_DATA_TYPE_BOOL            = 0x0001,
    SOCIA_DATA_TYPE_INT8            = 0x0002,
    SOCIA_DATA_TYPE_INT16           = 0x0003,
    SOCIA_DATA_TYPE_INT32           = 0x0004,
    SOCIA_DATA_TYPE_UINT8           = 0x0005,
    SOCIA_DATA_TYPE_UINT16          = 0x0006,
    SOCIA_DATA_TYPE_UINT32          = 0x0007,
    SOCIA_DATA_TYPE_REAL32          = 0x0008,
    SOCIA_DATA_TYPE_VISIBLE_STRING  = 0x0009,
    SOCIA_DATA_TYPE_OCTET_STRING    = 0x000A,
    SOCIA_DATA_TYPE_UNICODE_STRING  = 0x000B,
    SOCIA_DATA_TYPE_TIME_OF_DAY     = 0x000C,
    SOCIA_DATA_TYPE_TIME_DIFFERENCE = 0x000D,
    /* 0x000E is reserved */
    SOCIA_DATA_TYPE_DOMAIN          = 0x000F,
    SOCIA_DATA_TYPE_INT24           = 0x0010,
    SOCIA_DATA_TYPE_REAL64          = 0x0011,
    SOCIA_DATA_TYPE_INT40           = 0x0012,
    SOCIA_DATA_TYPE_INT48           = 0x0013,
    SOCIA_DATA_TYPE_INT56           = 0x0014,
    SOCIA_DATA_TYPE_INT64           = 0x0015,
    SOCIA_DATA_TYPE_UINT24          = 0x0016,
    /* 0x0017 is reserved */
    SOCIA_DATA_TYPE_UINT40          = 0x0018,
    SOCIA_DATA_TYPE_UINT48          = 0x0019,
    SOCIA_DATA_TYPE_UINT56          = 0x001A,
    SOCIA_DATA_TYPE_UINT64          = 0x001B,
    /* 0x001C - 0x001F are reserved */
    SOCIA_DATA_TYPE_PDO_COM         = 0x0020,
    SOCIA_DATA_TYPE_PDO_MAP         = 0x0021,
    SOCIA_DATA_TYPE_SDO_PARM        = 0x0022,
    SOCIA_DATA_TYPE_IDENTITY        = 0x0023
    /* 0x0024 - 0x003F are reserved */
} socia_data_type_t;

/** Description of a CiA Object */
typedef struct socia_obj {
    /** Index of the object */
    const uint16_t index;

    /** Subindex of the object */
    const uint8_t subindex;

    /* Name of the object */
    const char *parameter_name

    /** Number of sub-indices for an intex.
     *  May be empty or absent of object has no sub-objects. */
    const uint8_t sub_number;


    /** Access type for object.
     */
    enum socia_access_type access_type;


    /** The object code
     */
    enum socia_object_type object_type;

    /** inded of the data type of the object in the object dictionary.
     */
    enum socia_data_type data_type;

    /** Can object be pammped into a PDO */
    const unsigned pdo_mapping : 1;
} socia_obj_t;

/** Description of a CiA Dictionary */
typedef socia_dict {
    size_t length;
    struct socia_obj obj[1];
} socia_dict;



/* Return the index of the item in dict with given name */
ssize_t socia_dict_search_name( const struct socia_dict *dict, const char *name );

/* Merge two dictionaries */
struct socia_dict *socia_dict_merge( const struct socia_dict *dict0,
                                     const struct socia_dict dict1 );


#ifdef __cplusplus
}
#endif
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
#endif //SODICT_H
