#ifndef ST_HEURISTIC_EDGE_H
#define ST_HEURISTIC_EDGE_H

#include "node.h"
#include "weight.h"

typedef struct edge_t {
    node_t n1;       /**< First node id. */
    node_t n2;       /**< Second node id. */
    weight_t weight; /**< Edge's weight.  */
} edge_t;

extern edge_t *edge_alloc(node_t n1, edge_t n2, weight_t w);

extern void edge_release(edge_t *p_e);

extern int edge_compar(const void *p1, const void *p2);

extern int edge_weight_compar(const void *p1, const void *p2);

#endif /* ST_HEURISTIC_EDGE_H */
