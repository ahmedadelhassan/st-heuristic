#ifndef ST_HEURISTIC_EDGE_H
#define ST_HEURISTIC_EDGE_H

#include "node.h"
#include "weight.h"

typedef struct {
    node_t n1;       /**< First node id. */
    node_t n2;       /**< Second node id. */
    weight_t weight; /**< Edge's weight.  */
} edge_t;

extern edge_t edge_mk(node_t n1, node_t n2, weight_t w);

extern int edge_compar(const void *p1, const void *p2);

extern int edge_weight_compar(const void *p1, const void *p2);

extern void edge_print(edge_t e);

#endif /* ST_HEURISTIC_EDGE_H */
