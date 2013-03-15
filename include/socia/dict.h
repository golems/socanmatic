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
typedef enum socia_access {
    SOCIA_ACCESS_UNKNOWN,    ///< Unknown access type
    SOCIA_ACCESS_RO,         ///< Read only
    SOCIA_ACCESS_WO,         ///< Write only
    SOCIA_ACCESS_RW,         ///< Read-Write
    SOCIA_ACCESS_RWR,        ///< Read-Write on process input
    SOCIA_ACCESS_RWW,        ///< Read-Write on process output
} socia_access_t;

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
    enum socia_access access_type;

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
