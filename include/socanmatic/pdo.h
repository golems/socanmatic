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

#ifndef SOCANMATIC_PDO_H
#define SOCANMATIC_PDO_H

#ifdef __cplusplus
extern "C" {
#endif

#define CANMAT_RPDO_COM_BASE  ((uint16_t)0x1400)
#define CANMAT_TPDO_COM_BASE  ((uint16_t)0x1800)
#define CANMAT_RPDO_MAP_BASE  ((uint16_t)0x1600)
#define CANMAT_TPDO_MAP_BASE  ((uint16_t)0x1A00)

#define CANMAT_RPDO_COM_COB_ID       0x1
#define CANMAT_RPDO_COM_TRANS_TYPE   0x2


struct pdo_descriptor {
    uint32_t cob_id;          ///< CAN frame COB-ID
    unsigned n_obj : 4;       ///< number of objects in the PDO
    struct {
        unsigned offset : 3;  ///< offset in frame data for this PDO
        unsigned size   : 3;  ///< size of the object, range 1-4
        void *ptr;            ///< pointer to where we should store the object
    } obj[8];
};

struct pdo_descriptor_table {
    size_t n;             ///< number of valid entries
    size_t max;           ///< allocated size of descriptor
    struct pdo_descriptor_table *descriptor; ///< pointer to the entries
};

/** Lookup the descriptor for can_frame in and write message data to the pointed location.
 *
 */
enum canmat_status canmat_pdo_process(
    const struct pdo_descriptor_table table, const struct can_frame );

#ifdef __cplusplus
}
#endif
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
#endif //SOCANMATIC_PDO_H
