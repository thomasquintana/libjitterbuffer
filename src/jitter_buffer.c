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

/** @file jitter_buffer.c
 *  @brief Implements a jitter buffer.
 *
 *  @author Thomas Quintana (quintana.thomas@gmail.com)
 *  @bug No known bugs.
 */

/* C Run-Time */
#include <stdlib.h>
#include <string.h>

/* Jitter Buffer Run-Time */
#include "jitter_buffer.h"
#include "spinlock.h"

typedef struct jitter_buffer_frame {
  unsigned char *data;
  unsigned int   seq;
  unsigned int   size;
} jitter_buffer_frame_t;

typedef struct jitter_buffer {
  unsigned int             count;
  unsigned int             head;
  unsigned int             tail;
  unsigned int             size;
  jitter_buffer_frame_t   *frames;
  spinlock_t              *lock;
} jitter_buffer_t;

jitter_buffer_t* jb_create(unsigned int buffer_size, unsigned int frame_size) {
  /* Allocate all the memory necessary for the jitter buffer. */
  unsigned int size = sizeof(jitter_buffer_frame_t) + frame_size;
  size *= buffer_size;
  size += sizeof(jitter_buffer_t);
  unsigned char *temp = (unsigned char*)malloc(size);
  if (temp == NULL) {
    return NULL;
  }
  memset(temp, 0, size);
  /* Initialize the jitter buffer. */
  jitter_buffer_t *buffer = (jitter_buffer_t*)temp;
  buffer->frames = (jitter_buffer_frame_t*)&temp[sizeof(jitter_buffer_t)];
  buffer->size = buffer_size;
  /* Initialize the jitter buffer frames. */
  unsigned int offset = sizeof(jitter_buffer_frame_t) * buffer_size;
  offset += sizeof(jitter_buffer_t);
  for (unsigned int idx = 0; idx < buffer_size; idx++) {
    buffer->frames[idx].data = &temp[offset + (idx * frame_size)];
    buffer->frames[idx].size = frame_size;
  }
  /* Initialize the jitter buffer lock. */
  buffer->lock = spinlock_create();
  return buffer;
}

void jb_destroy(jitter_buffer_t *buffer) {
  spinlock_destroy(buffer->lock);
  free(buffer);
}

unsigned int jb_count(jitter_buffer_t *buffer) {
  spinlock_acquire(buffer->lock);
  unsigned int result = buffer->count;
  spinlock_release(buffer->lock);
  return result;
}

void jb_flush(jitter_buffer_t *buffer) {
  spinlock_acquire(buffer->lock);
  buffer->head = 0;
  buffer->tail = 0;
  buffer->count = 0;
  spinlock_release(buffer->lock);
}

bool jb_get(jitter_buffer_t *buffer, void *data) {
  spinlock_acquire(buffer->lock);
  if (buffer->count == 0) {
    spinlock_release(buffer->lock);
    return false;
  }
  jitter_buffer_frame_t *frame = &buffer->frames[buffer->head];
  memcpy(data, frame->data, frame->size);
  buffer->head = (buffer->head + 1) % buffer->size;
  buffer->count--;
  spinlock_release(buffer->lock);
  return true;
}

inline bool jb_is_empty(jitter_buffer_t *buffer) {
  spinlock_acquire(buffer->lock);
  bool result = buffer->count == 0 ? true : false;
  spinlock_release(buffer->lock);
  return result;
}

inline bool jb_is_full(jitter_buffer_t *buffer) {
  spinlock_acquire(buffer->lock);
  bool result = buffer->count == buffer->size ? true : false;
  spinlock_release(buffer->lock);
  return result;
}

bool jb_put(jitter_buffer_t *buffer, void *data, unsigned int seq) {
  spinlock_acquire(buffer->lock);
  /* Handle late packet arrivals. */
  jitter_buffer_frame_t *head = &buffer->frames[buffer->head];
  if (seq < head->seq) {
    spinlock_release(buffer->lock);
    return false;
  }
  /* Handle buffer overflows. */
  if (buffer->count == buffer->size) {
    buffer->head = (buffer->head + 1) % buffer->size;
    buffer->count--;
  }
  /* Insert the frame into the buffer at the tail end. */
  if (buffer->count > 0) {
    buffer->tail = (buffer->tail + 1) % buffer->size;
  }
  jitter_buffer_frame_t *frame = &buffer->frames[buffer->tail];
  memcpy(frame->data, data, frame->size);
  frame->seq = seq;
  buffer->count++;
  /* 
    Since the list is kept sorted we just have to move
    the last frame into the proper position based on its
    sequence number.
  */
  if (buffer->count > 1) {
    jitter_buffer_frame_t temp = {0};
    for (unsigned int idx = buffer->count - 1; idx > 0; idx--) {
      unsigned int curr_offset = (buffer->head + idx) % buffer->size;
      unsigned int prev_offset = (buffer->head + idx - 1) % buffer->size;
      jitter_buffer_frame_t *curr = &buffer->frames[curr_offset];
      jitter_buffer_frame_t *prev = &buffer->frames[prev_offset];
      if (curr->seq < prev->seq) {
        /* Swap */
        temp.data = curr->data;
        temp.seq = curr->seq;
        curr->data = prev->data;
        curr->seq = prev->seq;
        prev->data = temp.data;
        prev->seq = temp.seq;
      } else {
        break;
      }
    }
  }
  spinlock_release(buffer->lock);
  return true;
}
