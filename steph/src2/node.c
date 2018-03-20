#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
/*

*/
/**
 * \brief Allocate a new node
 * @param u The init point
 * @return A new node initialized with \a u.
 *//*

node_t *node_alloc(node_t n) {
    node_t *p_n = (node_t *) malloc(sizeof(node_t));
    if (!p_n) {
        fprintf(stderr, "node_alloc. memory allocation error for node %u.\n", n);
        exit(EXIT_FAILURE);
    }
    *p_n = n;
    return (p_n);
}

*/
/**
 * \brief Release a node
 * @param p_n Pointer to the node to be released
 *//*

void node_release(node_t *p_u) {
    if (p_u) {
        memset(p_u, 0x0, sizeof(node_t));
        free(p_u);
    }
}

*/
/**
 * \brief Copy a node.
 * @param p_u Pointer to a node
 * @return a copy of the node \a p_u holding the same data.
 *//*

node_t *node_copy(const node_t *p_n) {
    if (!p_n) {
        return (NULL);
    }

    return (node_alloc(*p_n));
}

*/
/**
 * \brief Compare two nodes.
 * @param p1 Pointer to the first node
 * @param p2 Pointer to the second node
 * @return  An integer less than, equal to, or greater than zero if the first node is
 * considered to be respectively less than, equal to, or greater than the second node.
 *//*

int node_compar(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    node_t n1 = *((const node_t *) p1);
    node_t n2 = *((const node_t *) p2);

    return (n1 - n2);
}
*/
