/******************************************************************************
 * Copyright (C) 2016                                                         *
 *                                                                            *
 * NOTICE:  All information contained herein is, and remains the property of  *
 * COMPANY. The intellectual and technical concepts contained herein are      *
 * proprietary to COMPANY and may be covered by U.S. and Foreign Patents,     *
 * patents in process, and are protected by trade secret or copyright law.    *
 * Dissemination of this information or reproduction of this material is      *
 * strictly forbidden unless prior written permission is obtained from        *
 * COMPANY. Access to the source code contained herein is hereby forbidden to *
 * anyone except current COMPANY employees, managers or contractors who have  *
 * executed Confidentiality and Non-disclosure agreements explicitly covering *
 * such access.                                                               *
 *                                                                            *
 * The copyright notice above does not evidence any actual or intended        *
 * publication or disclosure  of  this source code, which includes            *
 * information that is confidential and/or proprietary, and is a trade        *
 * secret, of  COMPANY. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC  *
 * PERFORMANCE, OR PUBLIC DISPLAY OF OR THROUGH USE  OF THIS SOURCE CODE      *
 * WITHOUT THE EXPRESS WRITTEN CONSENT OF COMPANY IS STRICTLY PROHIBITED, AND *
 * IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES. THE RECEIPT OR *
 * POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY  *
 * OR IMPLY ANY RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR  *
 * TO MANUFACTURE, USE, OR SELL ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN *
 * PART.                                                                      *
 ******************************************************************************/

#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H

#include <stddef.h>
#include <stdlib.h>

/**
 * A spin lock is a non-blocking lock used for protecting shared resources or
 * critical sections of code.
 */
typedef unsigned int spin_lock_t;

/**
 * Acquire a lock.
 *
 * @param lock A spin lock.
 *
 * @return Void.
 */
#define spin_lock_acquire(lock) { \
  while(__sync_val_compare_and_swap((lock), 0, 1)) { \
    asm volatile("pause\n": : :"memory"); \
  } \
}

/**
 * Create a new spin lock.
 *
 * @return A new spin lock.
 */
#define spin_lock_create() ((spin_lock_t*)calloc(1, sizeof(spin_lock_t)))

/**
 * Destroy a spin lock.
 *
 * @param lock A spin lock.
 *
 * @return Void.
 */
#define spin_lock_destroy(lock) free(lock)

/**
 * Release a spin lock.
 *
 * @param lock A spin lock.
 *
 * @return Void.
 */
#define spin_lock_release(lock) { \
  asm volatile("": : :"memory"); \
  *(lock) = 0; \
}

#endif /* SPIN_LOCK_H */