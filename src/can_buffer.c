/* Copyright (c) 2013, Matthew A. Zucker
 * All rights reserved.
 *
 * Author(s): Matthew A. Zucker <mzucker1@swarthmore.edu>
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


#include "can_buffer.h"

/* clear buffer */
void can_buf_clear(can_buf_t* buf) {
    buf->head = 0;
    buf->size = 0;
}

/* return true if empty */
int can_buf_isempty(const can_buf_t* buf) {
    return (buf->size == 0);
}

/* return true if full */
int can_buf_isfull(const can_buf_t* buf) {
    return (buf->size == CAN_BUFFER_MAX_SIZE);
}

/* push a message into the buffer and return 1 on success */
int can_buf_push(can_buf_t* buf,
                 const struct can_frame* frame,
                 int sequence_no) {

    if (buf->size >= CAN_BUFFER_MAX_SIZE) {

        return 0;

    } else {

        ++buf->size;

        can_tagged_frame_t* tf = can_buf_tail(buf);

        tf->frame = *frame;
        tf->sequence_no = sequence_no;

        return 1;

    }

}

/* get the message at the head (least recently inserted) return NULL if empty */
can_tagged_frame_t* can_buf_head(can_buf_t* buf) {

    if (!buf->size) {

        return NULL;

    } else {

        return buf->data + buf->head;

    }

}

/* get the message at the tail (most recently inserted) return NULL if empty */
can_tagged_frame_t* can_buf_tail(can_buf_t* buf) {

    if (!buf->size) {

        return NULL;

    } else {

        // presumably some of these casts are unnecessary
        return buf->data + ( (buf->head + buf->size - (size_t)1) & (size_t)CAN_BUFFER_MOD_MASK );

    }

}

/* pop a can buf and return 1 on success */
int can_buf_pop(can_buf_t* buf) {

    if (!buf->size) {

        return 0;

    } else {

        // presumably some of these casts are unnecessary
        buf->head = ( (buf->head + (size_t)1) & (size_t)CAN_BUFFER_MOD_MASK );
        --buf->size;

        return 1;

    }


}


/* Local Variables:                          */
/* mode: c                                   */
/* c-basic-offset: 4                         */
/* indent-tabs-mode:  nil                    */
/* End:                                      */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
