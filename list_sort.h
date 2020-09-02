/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_LIST_SORT_H
#define _LINUX_LIST_SORT_H

#include "list.h"

struct list_head;

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect((x), 0)
#endif

__attribute__((nonnull(2, 3))) void list_sort(void *priv,
                                              struct list_head *head,
                                              int (*cmp)(void *priv,
                                                         struct list_head *a,
                                                         struct list_head *b));
#endif
