#ifndef ST_HEURISTIC_EDGE_H
#define ST_HEURISTIC_EDGE_H

#include "node.h"
#include "weight.h"

typedef struct edge_t {
    node_t src;   /**< Source node id.      */
    node_t dest;  /**< Destination node id. */
    weight_t weight; /**< Edge's weight.       */
} edge_t;

extern edge_t *edge_alloc(node_t src, edge_t dest, weight w);

extern void edge_release(edge_t *p_e);

extern int edge_weight_compar(const void *p_e, const void *p_f);

#endif /* ST_HEURISTIC_EDGE_H */
