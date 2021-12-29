/**
 * @file s_queue.c
 * @date 2016/09/07
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Sortable queue implementation file.
 * @details
 * Artificial Intelligence Exercise\n
 * Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 */

#include "s_queue.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>


static size_t queue[MAX_QUEUE_SIZE];
static size_t queue_tail_index;
static float const* sort_criteria;

typedef int (*FuncComparator)(void const*, void const*);

static int compare_by_criteria(void const*, void const*);
static void bubble_sort(void*, size_t, size_t, FuncComparator);


/**
 * @brief Initialize sortable queue.
 * @param criteria Pointer to array which contains criterion for sorting. If sorting will NOT be used, pass NULL as the argument.
 */
void s_queue_init(float const* criteria)
{
    sort_criteria = criteria;
    queue_tail_index = 0;
}


/**
 * @brief Enqueue from the queue.
 * @param element element is stored into the queue.
 */
void s_queue_enqueue(size_t element)
{
    if (queue_tail_index == MAX_QUEUE_SIZE) {
        return;
    }

    queue[queue_tail_index++] = element;
}


/**
 * @brief Dequeue from the queue.
 *      you should call `s_queue_is_empty` before dequeue.
 * @return Dequeued element. If the queue is empty, `S_QUEUE_EMPTY_ELEMENT` will be returned.
 */
size_t s_queue_dequeue(void)
{
    size_t element = queue[0];
    queue_tail_index--;

    // Shift all elements.
    for (size_t i = 0; i < queue_tail_index; i++) {
        queue[i] = queue[i + 1];
    }

    return element;
}


/**
 * @brief Sorting all elements in the queue by the criteria.
 */
void s_queue_sort(void)
{
    if (sort_criteria == NULL) {
        return;
    }

    bubble_sort(queue, queue_tail_index, sizeof(size_t), compare_by_criteria);
}


/**
 * @brief Check whether or not the queue is empty.
 * @return true if the queue is empty. Otherwise false is returned.
 */
bool s_queue_is_empty(void)
{
    return (queue_tail_index == 0) ? (true) : false;
}


/**
 * @brief Comparator function for integer.
 * @param px Pointer to int.
 * @param py Pointer to int.
 * @return It returns
 *           0 (x == y)
 *          -1 (x <= y)
 *           1 (y <= x)
 */
static int compare_by_criteria(void const* px, void const* py)
{
    size_t x = *((size_t const*)px);
    size_t y = *((size_t const*)py);

    float d = sort_criteria[x] - sort_criteria[y];

    if (fabsf(d) < FLT_EPSILON) {
        return 0;
    } else if (d < 0.0f) {
        return -1;
    }

    return 1;
}


/**
 * @brief Sorting array by bubble sort (stable sort).
 * @param base Pointer to array which is sorted.
 * @param n The number of array elements.
 * @param size The size of array element.
 * @param f Comparator function for sorting.
 */
static void bubble_sort(void* base, size_t n, size_t size, FuncComparator f)
{
    if ((base == NULL) || (n == 0) || (size == 0) || (f == NULL)) {
        return;
    }
    uintptr_t base_addr = (uintptr_t)base;

    for (size_t i = 0; i < (n - 1); i++) {
        for (size_t j = n - 1; i < j; j--) {
            uintptr_t x = base_addr + size * (j);
            uintptr_t y = base_addr + size * (j - 1);
            if (f((void const*)x, (void const*)y) < 0) {
                // Swap.
                uint8_t* a = (uint8_t*)x;
                uint8_t* b = (uint8_t*)y;

                for (size_t k = 0; k < size; k++) {
                    uint8_t t = a[k];
                    a[k] = b[k];
                    b[k] = t;
                }
            }
        }
    }
}
