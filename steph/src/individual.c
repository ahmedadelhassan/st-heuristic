
#include "individual.h"

/**
 *
 * @return
 */
individual_t *individual_alloc(list_t *nodes) {
    individual_t *ind = (individual_t *) malloc(sizeof(individual_t));
    if (ind == NULL) {
        fprintf(stderr, "individual_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    st->nodes = nodes;
    st->n_nodes = list_size(nodes);
    st->min_weight_spanning_tree = 0;

    return (st);
}


/**
 *
 * @param st
 */
void individual_release(individual_t *st) {
    if (st != NULL) {
        list_release(st->nodes);
        memset(st, 0x0, sizeof(individual_t));
        free(st);
    }
}


individual_t *individual_mk(graph_t *g) {
    if (g == NULL) {
        fprintf(stderr, "individual_mk. NULL graph.\n");
        exit(EXIT_FAILURE);
    }

    /* */
    graph_random_shuffle_edges(g);
    edges_t *edges = g->edges;

    /* add edges one by one until all terminal nodes are part of the same connected component */
    union_find_t *uf = union_find_alloc(g->n_ndoes);
    size_t n_terminals = 0
    for (int i = 0; (n_terminals < g->n_terminals) && (i < g->n_edges); i++) {
        n_terminals = union_find_union(uf, edges[i].src, edges[i].dest);
    }

    /* color BLACK all nodes that are part of the terminal connected component, all other nodes are colored WHITE  */
    graph_set_all_color(g, WHITE);
    node_t root = union_find_find(uf, g->terminals[0]);
    for (node_t u = 0; u < g->n_nodes; u++) {
        if (union_find_find(uf, first_terminal) == root) {
            graph_set_color(g, u, BLACK);
        }
    }

    /*
     * inv 1: terminal nodes are colored BLACK.
     * inv 2: BLACK nodes do form a connected subgraph of g.
     */

    do {
        /* compute a minimum spanning tree on BLACK vertices */
        list_t *edges = graph_kruskal_min_spanning_tree(g);

        min_heap_t *h = min_heap_alloc();
        list_t *it = edges;
        while (it != NULL) {
            if (heap_lookup(h, e->src)) {
                head_increment(h, e->src);
            }
            else {
                heap_insert(h, e->src, 1);
            }

            if (heap_lookup(h, e->dest)) {
                head_increment(h, e->dest);
            }
            else {
                heap_insert(h, e->dest, 1);
            }
        }

        node_t u = heap_min(h);
        while () {
            graph_set_color(g, u, WHITE);
        }
    } while ();



    return(individual_alloc(l));
}