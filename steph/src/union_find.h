#ifndef ST_HEURISTIC_UNION_FIND_H
#define ST_HEURISTIC_UNION_FIND_H

#include "graph.h"

typedef struct union_find_item_t
{
    vertex_t parent;
    size_t   n_terminals;
    size_t   rank;
} union_find_item_t;

typedef struct union_find_t {
    union_find_item_t* items;
    size_t             size;
    size_t             count;
    graph_t *          graph;
} union_find_t;

extern union_find_t* union_find_alloc(graph_t* g);
extern void          union_find_reset(union_find_t* uf);
extern void          union_find_release(union_find_t* uf);
extern int           union_find_find_recursive_compression(union_find_t* uf, int i);
extern int           union_find_find_iterative_splitting(union_find_t* uf, int i);
extern int           union_find_find_iterative_halving(union_find_t* uf, int i);
extern int           union_find_find_iterative_compression(union_find_t* uf, int i);
extern void          union_find_union(union_find_t* uf, int i, int j);

#endif /* ST_HEURISTIC_UNION_FIND_H */
