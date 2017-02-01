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

#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stddef.h>
#include <stdlib.h>

typedef unsigned int spinlock_t;

#define spinlock_acquire(lock) { \
  while(__sync_val_compare_and_swap((lock), 0, 1)) { \
    asm volatile("pause\n": : :"memory"); \
  } \
}

#define spinlock_create() ((spinlock_t*)calloc(1, sizeof(spinlock_t)))

#define spinlock_destroy(lock) free(lock)

#define spinlock_release(lock) { \
  asm volatile("": : :"memory"); \
  *(lock) = 0; \
}

#endif /* SPINLOCK_H */