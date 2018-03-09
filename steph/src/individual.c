#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "individual.h"

/**
 *
 * @param el
 * @return
 */
static individual_t *individual_alloc(list_t *el) {
    individual_t *individual = (individual_t *) malloc(sizeof(individual_t));
    if (!individual) {
        fprintf(stderr, "individual_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    if (!el) {
        individual->n_edges = 0;
        individual->edges = NULL;
    } else {
        individual->n_edges = list_size(el);
        individual->edges = (edge_t *) calloc(individual->n_edges, sizeof(edge_t));
        individual->weight = 0;

        /* copy edges and compute total weight */
        int i = 0;
        while (el) {
            individual->edges[i].n1 = el->n1;
            individual->edges[i].n2 = el->n2;
            individual->edges[i].weight += el->weight;
            el = el->next;
            i++;
        }
    }

    /* edges are sorted according to the edge_compar function (i.e., n1 and next n2) */
    qsort(individual->edges, individual->n_edges, sizeof(edge_t), edge_compar);

    return (individual);
}

/**
 *
 * @param individual
 */
void individual_release(individual_t *individual) {
    if (!individual) {
        memset(individual->edges, 0x0, individual->n_edges * sizeof(edge_t));
        free(individual->edges);
        memset(individual, 0x0, sizeof(individual_t));
        free(individual);
    }
}

/**
 *
 * @param g
 * @param individual
 */
void individual_reduce(graph_t *g, individual_t *individual) {
    int done = 0;
    weight_t w = 0;
    list_t *edges = NULL;

    node_list_t *nl = individual->non_terminal_nodes;
    while (nl != NULL) {
        graph_color_set(g, nl->node);
    }

    do {
        /* compute a minimum spanning tree on BLACK vertices */
        el = graph_kruskal_min_spanning_tree(g);

        /* compute the degree of each node in the spanning tree */
        graph_node_counter_set_all(0);
        w = 0;
        edge_list_t *it_el = el;
        while (it_el != NULL) {
            graph_node_counter_increment(g, it_el->src);
            graph_node_counter_increment(g, it_el->dest);
            w += it_el->weight;
            it_el = it_el->next;
        }

        /* find (if it exists) a non-terminal BLACK node with degree 1 in the spanning tree */
        int found = 0;
        node_t n = 0;
        while ((n < g->n_nodes) && (found == 0)) {
            if ((graph_node_is_terminal(g, u) == 0) && (graph_node_color_get(g, u) == BLACK)) {
                if (graph_node_counter_get(g, u) == 1)) {
                    found = 1;
                }
            }
            n++;
        }

        if (found == 1) {
            /* node u is a non-terminal BLACK node with degree 1 in the spanning tree */
            graph_node_color_set(g, u, WHITE);
        } else {
            done = 1;
        }
    } while (done == 0);

}

/**
 *
 * @param g
 * @return
 */
individual_t *individual_mk_rand(graph_t *g) {
    individual_mk_rand_from_individual(g, NULL);
}

/**
 *
 * @param g
 * @param init_el
 * @return
 */
individual_t *individual_mk(graph_t *g, list_t *init_el) {
    assert(g);

    /* random shuffle the edges of the reference graph */
    graph_edges_random_shuffle(g);

    /* new union find */
    union_find_t *uf = union_find_alloc(g->n_nodes);

    /* list of kept edges */
    list_t *el = NULL;

    /* add initializing edges (if any) */
    list_t *it_init_el = init_el;
    while (it_el) {
        edge_t *e = (edge_t*) it_init_el->data;
        node_t n1 = e->n1;
        node_t n2 = e->n2;
        if (union_find_union(uf, n1, n2)) {
            list_insert_front(el, e);
            it_el->data = NULL;
        }
        it_init_el = it_init_el->next;
    }

    /* get rid of initializing edges (do not release data !) */
    list_release(init_el);
    init_el = NULL;

    /* add edges one by one until all terminal nodes are part of the same connected component */
    size_t n_terminals = union_find_max_n_terminals(uf);
    for (int i = 0; union_find_max_n_terminals(uf) < g->n_terminals && i < g->n_edges; i++) {
        node_t n1 = g->edges[i].n1;
        note_t n2 = g->edges[i].n2;
        if (union_find_union(uf, n1, n2)) {
            list_insert_front(el, &(g-edges[i]));
        }
    }

    /*
     * inv: terminals nodes are part of the same connected component.
     */

    /* color BLACK all nodes that are part of the terminal connected component, all other nodes are colored WHITE  */
    graph_node_color_set_all(g, WHITE);
    node_t root = union_find_find(uf, g->min_terminal_node);
    for (node_t u = 0; u < g->n_nodes; u++) {
        if (union_find_find(uf, u) == root) {
            graph_node_color_set(g, u, BLACK);
        }
    }

    int done = 0;
    weight_t w = 0;
    list_t *edges = NULL;

    do {
        /*
         * inv 1: terminal nodes are BLACK in graph g (some non-terminal nodes are BLACK).
         * inv 2: BLACK nodes do form a connected subgraph of g.
         */

        /* compute a minimum spanning tree on BLACK vertices */
        edges = graph_kruskal_min_spanning_tree(g);

        /* compute the degree of each node in the spanning tree */
        graph_node_counter_set_all(0);
        w = 0;
        list_t *it_l = edges;
        while (it_l != NULL) {
            edge_t *pe = (edge_t) it_l->data;
            graph_node_counter_increment(g, pe->src);
            graph_node_counter_increment(g, pe->dest);
            w += pe->weight;
            it_l = it_l->next;
        }

        /* find (if it exists) a non-terminal BLACK node with degree 1 in the spanning tree */
        int found = 0;
        node_t u = 0;
        while ((u < g->n_nodes) && (found == 0)) {
            if ((graph_node_is_terminal(g, u) == 0) && (graph_node_color_get(g, u) == BLACK) &&
                (graph_node_counter_get(g, u) == 1)) {
                found = 1;
            }
        }

        if (found == 1) {
            /* node u is a non-terminal BLACK node with degree 1 in the spanning tree */
            graph_node_color_set(g, u, WHITE);
        } else {
            done = 1;
        }
    } while (done == 0);

    /*
     * inv 1: terminal nodes are BLACK in graph g (some non-terminal nodes are BLACK).
     * inv 2: BLACK nodes do form a connected subgraph of g.
     * inv 3: edges is a min weight spanning tree of the BLACK nodes in which every non-terminal has degree at least 2.
     */

    individual_t *ind = individual_alloc(g, w);

    return (ind);
}

/**
 *
 * @param g
 * @param ind1
 * @param ind2
 * @return
 */
individual_t *individual_union(graph_t *g, individual_t *ind1, individual_t *ind2) {
    assert(g != NULL);
    assert(ind1 != NULL);
    assert(ind2 != NULL);

    list_t *union_l = sorted_list_union(ind1->nodes, ind1->nodes, individual_node_compar, individual_node_copy);

    individual_t *union_ind = individual_mk_rand_from_nodes(g, union_l);

    list_release_with_data_release(union_l, individual_node_release);

    return (union_ind);
}

/**
 *
 * @param g
 * @param ind1
 * @param ind2
 * @return
 */
individual_t *individual_intersection(graph_t *g, individual_t *ind1, individual_t *ind2) {
    assert(g != NULL);
    assert(ind1 != NULL);
    assert(ind2 != NULL);

    list_t *inter_l = sorted_list_intersection(ind1->nodes, ind1->nodes, individual_node_compar, individual_node_copy);

    individual_t *intersection_ind = individual_mk_rand_from_nodes(g, inter_l);

    list_release_with_data_release(mut_l, individual_node_release);

    return (intersection_ind);
}

/**
 *
 * @param g
 * @param ind
 * @return
 */
individual_t *individual_mutation(graph_t *g, individual_t *ind) {
    assert(g != NULL);
    assert(ind != NULL);

    /* mutate individual ind by deleting its i-th non-terminal */
    int i = rand() % (ind->n_nodes - g->n_non_terminals);

    list_t *l = ind->nodes;
    list_t *mut_l = NULL;

    int counter = 0;
    while (l != NULL) {
        node_t u = *((node_t *) l->data);

        if (graph_node_is_terminal(g, u) || (graph_node_is_non_terminal(g, u) && (counter != i))) {
            node_t *pu = individual_node_alloc(u);
            mut_l = list_insert_front(mut_l, pu);
        }

        if (graph_node_is_non_terminal(g, u)) {
            counter++;
        }

        l = l->next;
    }

    mut_l = list_reverse(mut_l);

    individual_t *mut_ind = individual_mk_rand_from_nodes(g, mut_l);

    list_release_with_data_release(mut_l, individual_node_release);

    return (mut_ind);
}