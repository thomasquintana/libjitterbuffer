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

/** @file rbt_tests.cpp
 *  @brief Tests the red-black trees interface.
 *
 *  @author Thomas Quintana (quintana.thomas@gmail.com)
 *  @bug No known bugs.
 */

#include <stdbool.h>
#include <string.h>

#include <gtest/gtest.h>

#include "../src/jitter_buffer.h"

TEST(jitter_buffer_tests, jitter_buffer_flow_in_order) {
  jitter_buffer_t *buffer = jb_create(5, 160);
  unsigned char frame[160];
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  for (unsigned int idx = 0; idx < 5; idx++) {
    memset(&frame, idx, 160);
    jb_put(buffer, &frame, idx);
  }
  ASSERT_TRUE(jb_is_full(buffer) == true);
  ASSERT_TRUE(jb_is_empty(buffer) == false);
  for (unsigned int f_idx = 0; f_idx < 5; f_idx++) {
    jb_get(buffer, &frame);
    for (unsigned int s_idx = 0; s_idx < 160; s_idx++) {
      ASSERT_TRUE(frame[s_idx] == f_idx);
    }
  }
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  jb_destroy(buffer);
}

TEST(jitter_buffer_tests, jitter_buffer_flow_out_of_order) {
  jitter_buffer_t *buffer = jb_create(5, 160);
  unsigned char frame[160];
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  memset(&frame, 0, 160);
  jb_put(buffer, &frame, 0);
  for (unsigned int idx = 4; idx > 0; idx--) {
    memset(&frame, idx, 160);
    jb_put(buffer, &frame, idx);
  }
  ASSERT_TRUE(jb_is_full(buffer) == true);
  ASSERT_TRUE(jb_is_empty(buffer) == false);
  for (unsigned int f_idx = 0; f_idx < 5; f_idx++) {
    jb_get(buffer, &frame);
    for (unsigned int s_idx = 0; s_idx < 160; s_idx++) {
      ASSERT_TRUE(frame[s_idx] == f_idx);
    }
  }
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  jb_destroy(buffer);
}

TEST(jitter_buffer_tests, jitter_buffer_late_arrivals) {
  jitter_buffer_t *buffer = jb_create(5, 160);
  unsigned char frame[160];
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  for (unsigned int idx = 5; idx > 0; idx--) {
    memset(&frame, idx, 160);
    jb_put(buffer, &frame, idx);
  }
  ASSERT_TRUE(jb_is_full(buffer) == false);
  ASSERT_TRUE(jb_is_empty(buffer) == false);
  ASSERT_TRUE(jb_count(buffer) == 1);
  jb_get(buffer, &frame);
  for (unsigned int s_idx = 0; s_idx < 160; s_idx++) {
    ASSERT_TRUE(frame[s_idx] == 5);
  }
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  jb_destroy(buffer);
}

TEST(jitter_buffer_tests, jitter_buffer_over_flow) {
  jitter_buffer_t *buffer = jb_create(5, 160);
  unsigned char frame[160];
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  for (unsigned int idx = 0; idx < 10; idx++) {
    memset(&frame, idx, 160);
    jb_put(buffer, &frame, idx);
  }
  ASSERT_TRUE(jb_is_full(buffer) == true);
  ASSERT_TRUE(jb_is_empty(buffer) == false);
  for (unsigned int f_idx = 5; f_idx < 10; f_idx++) {
    jb_get(buffer, &frame);
    for (unsigned int s_idx = 0; s_idx < 160; s_idx++) {
      ASSERT_TRUE(frame[s_idx] == f_idx);
    }
  }
  ASSERT_TRUE(jb_is_empty(buffer) == true);
  ASSERT_TRUE(jb_is_full(buffer) == false);
  jb_destroy(buffer);
}
