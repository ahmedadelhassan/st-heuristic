#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define HEAP_PARENT(i) (((i) - 1) >> 1)
#define HEAP_LEFT(i)   (((i) << 1) + 1)
#define HEAP_RIGHT(i)  (((i) << 1) + 2)

/**
 *
 * @param capacity
 * @param data_compar
 * @return
 */
heap_t *heap_alloc(size_t capacity, int (*data_compar)(const void *, const void *)) {
    assert(data_compar);

    /* allocate heap */
    heap_t *p_h = (heap_t *) malloc(sizeof(heap_t));
    if (!p_h) {
        fprintf(stderr, "heap_alloc. memory allocation error for capacity %zu.\n", capacity);
        exit(EXIT_FAILURE);
    }

    /* allocate heap data array */
    p_h->array = (void **) calloc(capacity, sizeof(void *));
    if (p_h->array == NULL) {
        fprintf(stderr, "heap_alloc. memory allocation error for individuals.\n");
        exit(EXIT_FAILURE);
    }

    /* initialize heap */
    p_h->size = 0;
    p_h->capacity = capacity;
    p_h->data_compar = data_compar;

    return (p_h);
}

/**
 *
 * @param h
 */
void heap_release(heap_t *p_h) {
    if (p_h) {
        if (p_h->array) {
            /* release data array */
            memset(p_h->array, 0x0, p_h->capacity * sizeof(void *));
            free(p_h->array);
        }

        /* release the heap itself */
        memset(h, 0x0, sizeof(heap_t));
        free(h);
    }
}

/**
 *
 * @param individual_heap
 */
static void heap_resize(heap_t *p_h) {
    assert(p_h != NULL);

    size_t new_capacity = 1 + (2 * p_h->capacity);
    p_h->array = (void **) calloc(new_capacity, sizeof(void *));
    if (!p_h->array) {
        fprintf(stderr, "heap_alloc. memory allocation error for new capacity %zu.\n", new_capacity);
        exit(EXIT_FAILURE);
    }

    p_h->capacity = new_capacity;
}

/**
 *
 * @param h
 * @param i
 */
static void heap_bubble_up(heap_t *p_h, int i) {
    assert(p_h);
    assert(p_h->compar);

    int parent_index = HEAP_PARENT(i);
    if (p_h->data_compar(p_h->array[i], p_h->array[parent_index] < 0) {
        void *tmp = p_h->array[i];
        p_h->array[i] = p_h->array[parent_index];
        p_h->array[parent_index] = tmp;
        heap_bubble_up(p_h, parent_index);
    }
}

/**
 *
 * @param p_h
 * @param data
 */
void heap_insert(heap_t *p_h, void *data) {
    assert(p_h);
    assert(data);

    if (p_h->size == p_h->capacity) {
        heap_resize(p_h);
    }

    p_h->array[p_h->size++] = data;
    heap_bubble_up(p_h, p_h->size - 1);
}

/**
 *
 * @param h
 * @param i
 */
static void heap_bubble_down(heap_t *p_h, int i) {
    assert(p_h);
    assert(p_h->compar);

    int left_idx = HEAP_LEFT(i);
    int right_idx = HEAP_RIGHT(i);

    if (left_idx >= p_h->size) {
        /* i is a leaf index */
        return;
    }

    int max_idx = i;

    if (p_h->data_compar(p_h->array[i], p_h->array[left_idx]) < 0) {
        max_idx = left_idx;
    }

    if ((right_idx < p_h->size) && p_h->data_compar(p_h->array[i], p_h->array[left_idx]) < 0) {
        max_idx = right_idx;
    }

    if (i != max_idx) {
        void *tmp = p_h->array[i];
        p_h->array[i] = p_h->array[max_idx];
        p_h->array[max_idx] = tmp;
        heap_bubble_down(p_h, max_idx);
    }
}

/**
 *
 * @param h
 * @param i
 * @return
 */
static void *heap_extract(heap_t *p_h, size_t i) {
    assert(p_h);
    assert(i < p_h->size);

    void *data = p_h->array[i];
    p_h->array[i] = p_h->array[p_h->size - 1];
    p_h->array[p_h->size - 1] = NULL;

    p_h->size--;
    heap_bubble_down(p_h, i);

    return (data);
}

/**
 *
 * @param h
 * @return
 */
void *heap_extract_rand(heap_t *p_h) {
    assert(p_h);

    if (p_h->size == 0) {
        return (NULL);
    }
    return (heap_extract(p_h, rand() % p_h->size));
}

/**
 *
 * @param h
 * @return
 */
void *heap_extract_max(heap_t *p_h) {
    assert(p_h);

    return (heap_extract(p_h, 0));
}
