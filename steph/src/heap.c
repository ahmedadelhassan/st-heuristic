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
 * @param compar
 * @return
 */
heap_t *heap_alloc(size_t capacity, int (*compar)(const void *, const void *)) {
    heap_t *h = (heap_t *) malloc(sizeof(heap_t));
    if (h == NULL) {
        fprintf(stderr, "heap_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    h->array = (void **) calloc(capacity, sizeof(void *));
    if (h->array == NULL) {
        fprintf(stderr, "heap_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    h->size = 0;
    h->capacity = capacity;
    h->compar = compar;

    return (h);
}

/**
 *
 * @param h
 */
void heap_release(heap_t *h) {
    heap_release_with_data_release(h, NULL);
}

/**
 *
 * @param h
 * @param data_release
 */
void heap_release_with_data_release(heap_t *h, void (*data_release)(void *)) {
    if (h != NULL) {
        if (h->array != NULL) {

            /* release data if a release function is provided */
            if (data_release != NULL) {
                for (int i = 0; i < h->size; i++) {
                    data_release(h->array[i]);
                }
            }

            memset(h->array, 0x0, h->capacity * sizeof(void *));
            free(h->array);
        }

        memset(h, 0x0, sizeof(heap_t));
        free(h);
    }
}

/**
 *
 * @param heap
 */
static void heap_resize(heap_t *h) {
    assert(h != NULL);

    size_t new_capacity = 1 + (2 * h->capacity);
    h->array = (void **) calloc(new_capacity, sizeof(void *));
    if (h->array == NULL) {
        fprintf(stderr, "heap_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    h->capacity = new_capacity;
}

/**
 *
 * @param h
 * @param i
 */
static void heap_bubble_up(heap_t *h, int i) {
    if (h->compar(h->array[i], h->array[HEAP_PARENT(i)]) < 0) {
        void *tmp = h->array[i];
        h->array[i] = h->array[HEAP_PARENT(i)];
        h->array[HEAP_PARENT(i)] = tmp;
        heap_bubble_up(h, HEAP_PARENT(i));
    }
}

/**
 *
 * @param h
 * @param data
 */
void heap_insert(heap_t *h, void *data) {
    assert(h != NULL);
    assert(data != NULL);

    if (h->size == h->capacity) {
        heap_resize(h);
    }

    h->array[h->size++] = data;
    heap_bubble_up(h, h->size - 1);
}

/**
 *
 * @param h
 * @param i
 */
static void heap_bubble_down(heap_t *h, int i) {
    int left_idx = HEAP_LEFT(i);
    int right_idx = HEAP_RIGHT(i);

    if (left_idx >= h->size) {
        /* i is a leaf index */
        return;
    }

    int max_idx = i;

    if (h->compar(h->array[i], h->array[left_idx]) < 0) {
        max_idx = left_idx;
    }

    if ((right_idx < h->size) && (h->compar(h->array[max_idx], h->array[left_idx]) < 0)) {
        max_idx = right_idx;
    }

    if (i != max_idx) {
        void *tmp = h->array[i];
        h->array[i] = h->array[max_idx];
        h->array[max_idx] = tmp;
        heap_bubble_down(h, max_idx);
    }
}

/**
 *
 * @param h
 * @param i
 * @return
 */
static void *heap_extract(heap_t *h, size_t i) {
    assert(h != NULL);
    assert(i < h->size);

    void *data = h->array[i];
    h->array[i] = h->array[h->size - 1];
    h->array[h->size - 1] = NULL;

    h->size--;
    heap_bubble_down(h, i);

    return (data);
}

/**
 *
 * @param h
 * @return
 */
void *heap_extract_rand(heap_t *h) {
    int i = rand() % h->size;
    return (heap_extract(h, i));
}

/**
 *
 * @param h
 * @return
 */
void *heap_extract_max(heap_t *h) {
    return (heap_extract(h, 0));
}
