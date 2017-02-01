/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/** @file jitterbuffer.h
 *  @brief Public functions, macros and type definitions used for
 *         jitter buffers.
 *
 *  @author Thomas Quintana (quintana.thomas@gmail.com)
 *  @bug No known bugs.
 */

#ifndef JITTER_BUFFER_H
#define JITTER_BUFFER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A jitter buffer is a shared data area where voice packets
 * can be collected, stored, and sent to the voice processor
 * in evenly spaced intervals.
 */
typedef struct jitter_buffer jitter_buffer_t;

/**
 * Create a new jitter buffer.
 *
 * @param buffer_size The maximum number of frames to store in
 *                    the jitter buffer.
 *
 * @param frame_size The size of each frame in bytes.
 *
 * @return A new jitter buffer.
 */
jitter_buffer_t* jb_create(unsigned int buffer_size, unsigned int frame_size);

/**
 * Destroy a jitter buffer.
 *
 * @param buffer A jitter buffer.
 *
 * @return Void.
 */
void             jb_destroy(jitter_buffer_t *buffer);

/**
 * Returns the number of frames in a jitter buffer.
 *
 * @param buffer A jitter buffer.
 *
 * @return The number of frames in a jitter buffer.
 */
unsigned int     jb_count(jitter_buffer_t *buffer);

/**
 * Flushes all the frames in a jitter buffer.
 *
 * @param buffer A jitter buffer.
 *
 * @return Void.
 */
void             jb_flush(jitter_buffer_t *buffer);

/**
 * Returns the next frame in a jitter buffer.
 *
 * @param buffer A jitter buffer.
 *
 * @param data A pointer to a memory location which will serve as the 
               destination for the frame.
 */
bool             jb_get(jitter_buffer_t *buffer, void *data);

/**
 * Returns true if the jitter buffer is empty and false otherwise.
 *
 * @return True if the jitter buffer is empty and false otherwise.
 */
bool             jb_is_empty(jitter_buffer_t *buffer);

/**
 * Returns true if the jitter buffer is full and false otherwise.
 *
 * @return True if the jitter buffer is full and false otherwise.
 */
bool             jb_is_full(jitter_buffer_t *buffer);

/**
 * Insert a new frame into the jitter buffer.
 *
 * @param buffer A jitter buffer.
 *
 * @param data A pointer to a memory location containing the source frame.
 *
 * @param seq A sequence number used to order the frames in the jitter buffer.
 */
bool             jb_put(jitter_buffer_t *buffer, void *data, unsigned int seq);

#ifdef __cplusplus
}
#endif

#endif /* JITTER_BUFFER_H */
