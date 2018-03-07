#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

/**
 *
 * @param u
 * @return
 */
node_t *node_alloc(node_t u) {
    node_t *p_u = (node_t *) malloc(sizeof(node_t));
    if (p_u == NULL) {
        fprintf(stderr, "node_alloc. memory allocation error for node %zu.\n", u);
        exit(EXIT_FAILURE);
    }
    *p_u = u;
    return (p_u);
}

/**
 *
 * @param p_u
 */
void individual_node_release(node_t *p_u) {
    if (p_u != NULL) {
        memset(p_u, 0x0, sizeof(node_t));
        free(p_u);
    }
}

/**
 *
 * @param p_u
 * @return
 */
node_t *node_copy(const node_t *p_u) {
    if (p_u == NULL) {
        return (NULL);
    }

    return (node_alloc(*p_u));
}

/**
 *
 * @param p_u
 * @param p_v
 * @return
 */
int node_compar(const void *p, const void *q) {
    assert(p_u != NULL);
    assert(p_v != NULL);

    const node_t *p_u = (const node_t*) p;
    const node_t *q_u = (const node_t*) q;

    return (*p_u - *q_u);
}