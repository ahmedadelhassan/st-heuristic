#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edge.h"

edge_t edge_mk(node_t n1, node_t n2, weight_t w) {
    edge_t e;
    e.n1 = (n1 < n2) ? n1 : n2;
    e.n2 = (n1 < n2) ? n2 : n1;
    e.weight = w;
    return (e);
}

/**
 * \brief Compare two edges according to their first node and next their second node.
 * @param p1 Pointer to the first edge
 * @param p2 Pointer to the second edge
 * @return An integer less than, equal to, or greater than zero if the first edge is
 * considered to be respectively less than, equal to, or greater than the second.
 * Edges are sorted on the first node, and next on the second node.
 */
int edge_compar_by_endpoints(const void *p1, const void *p2) {
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


/**
 * \brief Compare two edges according to their weight.
 * @param p1 Pointer to the first edge
 * @param p2 Pointer to the second edge
 * @return An integer less than, equal to, or greater than zero if the first edge's weight is
 * considered to be respectively less than, equal to, or greater than the second.
 */
int edge_compar_by_weight(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    const edge_t *p_e1 = (const edge_t *) p1;
    const edge_t *p_e2 = (const edge_t *) p2;
    return (p_e1->weight - p_e2->weight);
}

/**
 *
 * @param e
 */
void edge_fprint(FILE *f, edge_t e) {
    fprintf(f, "(%u - %u, %u)", e.n1, e.n2, e.weight);
}