#ifndef ST_HEURISTIC_NODE_LIST_H
#define ST_HEURISTIC_NODE_LIST_H

#include "node.h"

typedef struct node_list_t {
    struct node_list_t *next;
    node_t node;
} node_list_t;

extern node_list_t *node_list_alloc(node_t n);

extern void node_list_release(node_list_t *nl);

extern node_list_t *node_list_copy(const node_list_t *nl);

extern size_t node_list_size(node_list_t *nl);

extern node_list_t *node_list_delete_ith(node_list_t *nl, unsigned int i);

extern node_list_t *node_list_insert_front(node_list_t *nl, node_t n);

extern node_list_t *node_list_reverse(node_list_t *nl);

extern node_list_t *node_list_sort(node_list_t *nl);

extern node_list_t *node_list_union(node_list_t *nl1, node_list_t *nl2);

extern node_list_t *node_list_intersection(node_list_t *nl1, node_list_t *nl2);

extern node_list_t *node_list_difference(node_list_t *nl1, node_list_t *nl2);

#endif /* ST_HEURISTIC_NODE_LIST_H */
