#ifndef ST_HEURISTIC_EDGE_edge_list_H
#define ST_HEURISTIC_EDGE_edge_list_H

#include <stdlib.h>

#include "edge.h"

typedef struct edge_list_t {
    struct edge_list_t *p_next;
    edge_t edge;
} edge_list_t;

extern edge_list_t *edge_list_alloc(edge_t e);

extern void edge_list_release(edge_list_t *p_el);

extern edge_list_t *edge_list_copy(edge_list_t *p_el);

extern edge_list_t *edge_list_reverse(edge_list_t *p_el);

extern size_t edge_list_size(edge_list_t *p_el);

extern edge_list_t *edge_list_insert_front(edge_list_t *p_el, edge_t e);

extern edge_list_t *edge_list_sort_by_endpoints(edge_list_t *p_el);

extern edge_list_t *edge_list_sort_by_weight(edge_list_t *p_el);

extern void edge_list_fprint(const edge_list_t *p_el, FILE *f);

#endif /* ST_HEURISTIC_EDGE_edge_list_H */
