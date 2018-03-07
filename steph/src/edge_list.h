#ifndef ST_HEURISTIC_EDGE_LIST_H
#define ST_HEURISTIC_EDGE_LIST_H

#include "edge.h"

typedef struct edge_list_t {
    struct edge_list_t *next;
    edge_t edge;
} edge_list_t;

extern edge_list_t *edge_list_alloc(edge_t e);

extern void edge_list_release(edge_list_t *el);

#endif /* ST_HEURISTIC_EDGE_LIST_H */
