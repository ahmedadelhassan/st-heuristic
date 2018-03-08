#include <assert.h>
#include <stdio.h>
#include <stdlib.h>>
#include <string.h>

#include "edge.h"

/**
 *
 * @param n1
 * @param n2
 * @param w
 * @return
 */
edge_t *edge_alloc(node_t n1, node_t n2, weight w) {
    edge_t *e = (edge_t *) malloc(sizeof(edge_t));
    if (e == NULL) {
        fprintf(stderr, "edge_alloc. memory allocation error for (%zu, %zu, %zu).\n", n1, n2, w);
        exit(EXIST_FAILURE);
    }

    e->n1 = n1;
    e->n2 = n2;
    e->w = w;

    return (e);
}

/**
 *
 * @param e
 */
void edge_release(edge_t *e) {
    if (e != NULL) {
        memset(e, 0x0, sizeof(edge_t));
        free(e);
    }
}

/**
 *
 * @param e
 * @param f
 * @return
 */
int edge_compar(const void *p1, const void *p2) {
    assert(p1 != NULL);
    assert(p2 != NULL);

    const edge_t *e1 = (const edge_t *) p1;
    const edge_t *e2 = (const edge_t *) p2;

    int cmp = e1->n1 - e2->n1;
    if (cmp) {
        return (cmp);
    } else {
        return (e1->n2 - e2->n2);
    }
}

/**
 *
 * @param e
 * @param f
 * @return
 */
int edge_weight_compar(const void *p1, const void *p2) {
    assert(p1 != NULL);
    assert(p2 != NULL);

    const edge_t *e1 = (const edge_t *) p1;
    const edge_t *e2 = (const edge_t *) p2;

    return (e1->weight - e2->weight);
}