#ifndef ST_HEURISTIC_NODE_H
#define ST_HEURISTIC_NODE_H

typedef unsigned int node_t;

extern node_t *node_alloc(node_t u);

extern void individual_node_release(node_t *p_u);

extern node_t *node_copy(const node_t *p_u);

extern int node_compar(const void *p_u, const void *p_v);

#endif /* ST_HEURISTIC_NODE_H */
