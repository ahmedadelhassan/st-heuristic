#ifndef ST_HEURISTIC_NODE_H
#define ST_HEURISTIC_NODE_H

/** \typedef  node_t: node's graph */
typedef unsigned int node_t;

extern node_t *node_alloc(node_t n);

extern void node_release(node_t *p_n);

extern node_t *node_copy(const node_t *p_n);

extern int node_compar(const void *p1, const void *p2);

#endif /* ST_HEURISTIC_NODE_H */
