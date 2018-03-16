#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edge.h"

/**
 * \brief Allocate a new edge.
 * @param n1 First node of the edge
 * @param n2 Second node of the edge
 * @param w Weight of the edge
 * @return a new edge with first node \a n1, second node \a n2 and weight \a w.
 */
edge_t *edge_alloc(node_t n1, node_t n2, weight_t w) {
    assert(n1 != n2);

    edge_t *p_e = (edge_t *) malloc(sizeof(edge_t));
    if (!p_e) {
        fprintf(stderr, "edge_alloc. memory allocation error for (%u, %u, %u).\n", n1, n2, w);
        exit(EXIT_FAILURE);
    }

    p_e->n1 = (n1 < n2) ? n1 : n2;
    p_e->n2 = (n1 < n2) ? n2 : n1;
    p_e->weight = w;

    return (p_e);
}

/**
 * \brief Release an edge.
 * @param e Pointer to the edge
 */
void edge_release(edge_t *p_e) {
    if (p_e) {
        memset(p_e, 0x0, sizeof(edge_t));
        free(p_e);
    }
}

/**
 * \brief Compare two edges according to their first node and next their second node.
 * @param p1 Pointer to the first edge
 * @param p2 Pointer to the second edge
 * @return An integer less than, equal to, or greater than zero if the first edge is
 * considered to be respectively less than, equal to, or greater than the second.
 * Edges are sorted on the first node, and next on the second node.
 */
int edge_compar(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    const edge_t *p_e1 = (const edge_t *) p1;
    const edge_t *p_e2 = (const edge_t *) p2;

    int cmp = p_e1->n1 - p_e2->n1;
    if (cmp) {
        return (cmp);
    } else {
        return (p_e1->n2 - p_e2->n2);
    }
}

int edge_compar_p(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    const edge_t *p_e1 = *((const edge_t **) p1);
    const edge_t *p_e2 = *((const edge_t **) p2);
    return (edge_compar(p_e1, p_e2));
}

/**
 * \brief Compare two edges according to their weight.
 * @param p1 Pointer to the first edge
 * @param p2 Pointer to the second edge
 * @return An integer less than, equal to, or greater than zero if the first edge's weight is
 * considered to be respectively less than, equal to, or greater than the second.
 */
int edge_weight_compar(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    const edge_t *p_e1 = (const edge_t *) p1;
    const edge_t *p_e2 = (const edge_t *) p2;
    return (p_e1->weight - p_e2->weight);
}

/**
 *
 * @param p1
 * @param p2
 * @return
 */
int edge_weight_compar_p(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    const edge_t *p_e1 = *((const edge_t **) p1);
    const edge_t *p_e2 = *((const edge_t **) p2);
    return (edge_weight_compar(p_e1, p_e2));
}

/**
 *
 * @param e
 */
void edge_print(edge_t e) {
    fprintf(stdout, "(%u, %u, %u)", e.n1, e.n2, e.weight);
}