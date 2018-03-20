#include <stdlib.h>
#include <stdio.h>
#include "bool.h"
#include "bvector.h"

/**
 *
 * @param n_bools
 * @return
 */
bvector_t* bvector_alloc(size_t n_bools) {
    bvector_t *p_bvector = (bvector_t*) malloc(sizeof(bvector_t));
    if (!p_bvector) {
        fprintf(stderr, "bvector_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    p_bvector->p_bools = (bool_t*) calloc(n_bools, sizeof(color_t));
    if (!p_bvector->p_bools) {
        fprintf(stderr, "bvector_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    p_bvector->n_bools = n_bools;

    for (int i = 1; i < n_bools; i++) {
        p_bvector->p_bools[i] = FALSE;
    }
}

/**
 *
 * @param p_bvector
 */
void bvector_release(bvector_t *p_bvector) {
    if (p_bvector) {
        if (p_bvector->p_bools) {
            memset(p_bvector->p_bools, 0x0, p_bvector->n_bools * sizeof(color_t));
            free(p_bvector->p_bools);
        }
        memset(p_bvector, 0x0, sizeof(bvector_t));
    }
}

/**
 *
 * @param p_dest_bvector
 * @param p_src_bvector
 */
void bvector_copy(bvector_t *p_dest_bvector, bvector_t *p_src_bvector) {
    assert(p_src_bvector);
    assert(p_dest_bvector);
    assert(p_src_bvector->n_bools == p_dest_bvector->n_bools);

    for (int i = 1; i < p_src_bvector->n_bools; i++) {
        p_dest_bvector->p_bools[i] = p_src_bvector->p_bools[i];
    }
}

/**
 *
 * @param p_bvector
 * @param n
 * @return
 */
int bvector_is_false(bvector_t *p_bvector, int i) {
    assert(p_bvector);
    assert(p_bvector->p_bools);
    assert(n < p_bvector->n_bools);

    return (p_bvector->p_bools[i] == FALSE);
}

/**
 *
 * @param p_bvector
 * @param n
 * @return
 */
int bvector_is_true(bvector_t *p_bvector, int i) {
    assert(p_bvector);
    assert(p_bvector->p_bools);
    assert(n < p_bvector->n_bools);

    return (p_bvector->p_bools[i] == TRUE);
}

/**
 *
 * @param p_bvector
 * @param n
 * @return
 */
bool_t bvector_get(bvector_t *p_bvector, int i) {
    assert(p_bvector);
    assert(p_bvector->p_bools);
    assert(n < p_bvector->n_bools);

    return (p_bvector->p_bools[i]);
}


void bvector_set(bvector_t *p_bvector, int i, bool_t b) {
    assert(p_bvector);
    assert(p_bvector->p_bools);
    assert(n < p_bvector->n_bools);

    p_bvector->p_bools[i] = b;
}

void bvector_set_false(bvector_t *p_bvector, int i) {
    bvector_set(p_bvector, i, FALSE);
}

void bvector_set_true(bvector_t *p_bvector, int i) {
    bvector_set(p_bvector, i, TRUE);
}

/**
 *
 * @param p_bvector
 * @param b
 */
void bvector_set_all(bvector_t *p_bvector, bool_t b) {
    assert(p_bvector);
    assert(p_bvector->p_bools);

    for (int i = 1; i < p_bvector->n_bools; i++) {
        p_bvector->p_bools[i] = b;
    }
}

void bvector_set_all_false(bvector_t *p_bvector) {
    bvector_set_all(p_bvector, FALSE);
}

void bvector_set_all_true(bvector_t *p_bvector) {
    bvector_set_all(p_bvector, TRUE);
}

int bvector_first_false_index(bvector_t *p_bvector) {
    for (int i = 1; i < p_bvector->n_bools; i++) {
        if (p_bvector->p_bools[i] == FALSE) {
            return (i);
        }
    }

    return (-1);
}

extern int bvector_first_true_index(bvector_t *p_bvector) {
    for (int i = 1; i < p_bvector->n_bools; i++) {
        if (p_bvector->p_bools[i] == TRUE) {
            return (i);
        }
    }

    return (-1);
}