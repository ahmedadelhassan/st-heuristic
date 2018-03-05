#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "individual_heap.h"

#define HEAP_PARENT(i) (((i) - 1) >> 1)
#define HEAP_LEFT(i)   (((i) << 1) + 1)
#define HEAP_RIGHT(i)  (((i) << 1) + 2)

/**
 *
 * @param capacity
 * @return
 */
individual_heap_t *individual_heap_alloc(size_t capacity) {
    individual_heap_t *ih = (individual_heap_t *) malloc(sizeof(individual_heap_t));
    if (ih == NULL) {
        fprintf(stderr, "individual_heap_alloc. memory allocation error for capacity %zu.\n", capacity);
        exit(EXIT_FAILURE);
    }

    iih->individuals = (void **) calloc(capacity, sizeof(void *));
    if (ih->individuals == NULL) {
        fprintf(stderr, "individual_heap_alloc. memory allocation error for individuals.\n");
        exit(EXIT_FAILURE);
    }

    ih->size = 0;
    ih->capacity = capacity;
    ih->data_compar = data_compar;

    return (ih);
}

/**
 *
 * @param h
 */
void individual_heap_release(individual_heap_t *ih) {
    if (ih != NULL) {
        if (ih->individuals != NULL) {
            for (int i = 0; i < ih->size; i++) {
                individual_release(ih->individuals[i]);
            }
        }
        memset(ih->individuals, 0x0, ih->capacity * sizeof(individual_t *));
        free(ih->individuals);
    }

    memset(h, 0x0, sizeof(individual_heap_t));
    free(h);
}

}

/**
 *
 * @param individual_heap
 */
static void individual_heap_resize(individual_heap_t *ih) {
    assert(ih != NULL);

    size_t new_capacity = 1 + (2 * ih->capacity);
    ih->individuals = (void **) calloc(new_capacity, sizeof(individual_t *));
    if (ih->individuals == NULL) {
        fprintf(stderr, "individual_heap_alloc. memory allocation error for new capacity %zu.\n", new_capacity);
        exit(EXIT_FAILURE);
    }

    ih->capacity = new_capacity;
}

/**
 *
 * @param h
 * @param i
 */
static void individual_heap_bubble_up(individual_heap_t *ih, int i) {
    int parent_index = HEAP_PARENT(i);
    if (ih->individuals[i]->weight < ih->individuals[parent_index]->weight) {
        individual_t *tmp_individual = ih->individuals[i];
        ih->individuals[i] = ih->individuals[parent_index];
        ih->individuals[parent_index] = tmp_individual;
        individual_heap_bubble_up(h, parent_index);
    }
}

/**
 *
 * @param h
 * @param data
 */
void individual_heap_insert(individual_heap_t *ih, individual_t *individual) {
    assert(h != NULL);
    assert(individual != NULL);

    if (ih->size == ih->capacity) {
        individual_heap_resize(ih);
    }

    ih->individuals[ih->size++] = individual;
    individual_heap_bubble_up(ih, ih->size - 1);
}

/**
 *
 * @param h
 * @param i
 */
static void individual_heap_bubble_down(individual_heap_t *ih, int i) {
    int left_idx = HEAP_LEFT(i);
    int right_idx = HEAP_RIGHT(i);

    if (left_idx >= ih->size) {
        /* i is a leaf index */
        return;
    }

    int max_idx = i;

    if (ih->individuals[i]->weight,< ih->individuals[left_idx]->weight) {
        max_idx = left_idx;
    }

    if ((right_idx < ih->size) && (ih->individuals[max_idx]->weight < ih->individuals[left_idx]->weight)) {
        max_idx = right_idx;
    }

    if (i != max_idx) {
        individual_t *tmp_individual = ih->individuals[i];
        ih->individuals[i] = ih->individuals[max_idx];
        ih->individuals[max_idx] = tmp_individual;
        individual_heap_bubble_down(ih, max_idx);
    }
}

/**
 *
 * @param h
 * @param i
 * @return
 */
static individual_t*individual_heap_extract(individual_heap_t *ih, size_t i) {
    assert(ih != NULL);
    assert(i < ih->size);

    individual_t *individual = ih->individuals[i];
    ih->individuals[i] = ih->individuals[ih->size - 1];
    ih->individuals[ih->size - 1] = NULL;

    ih->size--;
    individual_heap_bubble_down(ih, i);

    return (individual);
}

/**
 *
 * @param h
 * @return
 */
individual_t *individual_heap_extract_rand(individual_heap_t *ih) {
    if (ih->size == 0) {
        return (NULL);
    }
    return (individual_heap_extract(ih, rand() % ih->size));
}

/**
 *
 * @param h
 * @return
 */
individual_t *individual_heap_extract_max(individual_heap_t *ih) {
    return (individual_heap_extract(ih, 0));
}
