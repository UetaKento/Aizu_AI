/**
 * @file s_queue.h
 * @date 2016/09/07
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Sortable queue header file.
 * @details
 * Artificial Intelligence Exercise\n
 * Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 */

#ifndef S_QUEUE_H
#define S_QUEUE_H


#include <stddef.h>
#include <stdbool.h>


enum {
    MAX_QUEUE_SIZE = 128,
};

extern void s_queue_init(float const*);
extern void s_queue_enqueue(size_t);
extern size_t s_queue_dequeue(void);
extern void s_queue_sort(void);
extern bool s_queue_is_empty(void);


#endif
