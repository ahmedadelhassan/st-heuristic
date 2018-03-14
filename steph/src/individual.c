#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        individual->total_weight = 0;

        /* copy edges and compute total weight */
        int i = 0;
        while (el) {
            individual->edges[i].n1 = el->n1;
            individual->edges[i].n2 = el->n2;
            individual->edges[i].total_weight += el->total_weight;
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
 * @return
 */
individual_t *individual_mk(graph_t *p_g) {
    individual_mk_rand_from_individual(g, NULL);
}

/**
 *
 * @param g
 * @param init_el
 * @return
 */
individual_t *individual_mk_with_init_edges(graph_t *p_g, list_t *p_init_el) {
    assert(g);

    /* random shuffle the edges of the reference graph */
    graph_edges_random_shuffle(p_g);

    /* new union find */
    graph_union_find_init(p_g);

    /* list of kept edges */
    list_t *p_el = NULL;

    /* add initializing edges (if any) */
    while (p_init_el) {
        edge_t *p_e = (edge_t *) p_init_el->data;
        node_t n1 = e->n1;
        node_t n2 = e->n2;

        /* add the edge regardless of the union_find step */
        graph_union_find_union(p_g, n1, n2);
        list_insert_front(p_el, p_e);
        p_init_el->data = NULL;
        p_init_el = p_init_el->next;
    }

    /* random shuffle the edges of the reference graph */
    graph_edges_random_shuffle(p_g);

    /* add edges one by one until all terminal nodes are part of the same connected component */
    size_t n_terminals = graph_union_find_max_n_terminals(p_g);
    for (int i = 0; n_terminals < p_g->n_terminals && i < g->n_edges; i++) {
        edge_t e = p_g->edges[i];
        node_t n1 = e.n1;
        note_t n2 = e.n2;
        if (graph_union_find_union(p_g, n1, n2)) {
            edge_t *p_e = graph_search_edge_by_endpoints(p_g, e);
            if (!p_e) {
                fprintf(stderr, "individual_mk_with_init_edges. cannot find edge (%u, %u, %u)\n", e.n1, e.n2, e.weight);
                exit(EXIT_FAILURE);
            }
            list_insert_front(p_el, p_e);
        }
        n_terminals = graph_union_find_max_n_terminals(p_g);
    }

    /*
     * inv: terminals nodes are part of the same connected component.
     */

    /* color BLACK all nodes that are part of the terminal connected component, all other nodes are colored WHITE  */
    graph_node_color_set_all(p_g, WHITE);
    node_t root = graph_union_find_find(p_g, p_g->min_terminal_node);
    for (node_t u = 0; u < p_g->n_nodes; u++) {
        if (graph_union_find_find(p_g, u) == root) {
            graph_node_color_set(p_g, u, BLACK);
        }
    }

    /* pruning degree 1 non-terminal nodes in minimum spanning tree */

    int done = 0;
    list_t *p_mst_el = NULL;
    do {
        /*
         * inv 1: terminal nodes are BLACK in graph p_g (some non-terminal nodes are also BLACK).
         * inv 2: BLACK nodes do form a connected subgraph of p_g.
         */

        /* compute a minimum spanning tree on BLACK vertices */
        p_mst_el = graph_kruskal_min_spanning_tree_on_black_nodes(p_g);

        /* reset node's counters */
        list_t *p_it_mst_el = p_it_mst_el;
        while (p_it_mst_el) {
            edge_t *p_e = (edge_t*) p_it_mst_el->data;
            graph_node_counter_reset(p_g, p_e->n1);
            graph_node_counter_reset(p_g, p_e->n2);
        }

        /* compute the degree of each node in the spanning tree */
        p_it_mst_el = p_it_mst_el;
        while (p_it_mst_el) {
            edge_t *p_e = (edge_t*) p_it_mst_el->data;
            graph_node_counter_increment(p_g, p_e->n1);
            graph_node_counter_increment(p_g, p_e->n2);
            p_it_mst_el = p_it_mst_el->next;
        }

        /* find (if it exists) a non-terminal BLACK node with degree 1 in the spanning tree */
        int found = 0;
        for (node_t u = 0; u < p_g->n_nodes; u++) {
            int u_is_non_terminal = graph_node_is_non_terminal(p_g, u);
            int u_is_black = graph_node_color_get(pg, u) == BLACK;
            int u_counter = graph_node_counter_get(pg, u);
            if (u_is_non_terminal && u_is_black && u_counter == 1) {
                found = ~0;
                graph_node_color_set(g, u, WHITE);
            }
        }

        if (!found) {
            /* we did find at least one non-terminal BLACK node with degree 1 in the mst */
            done = ~0;
        }
    } while (!done);

    /*
     * inv 1: terminal nodes are BLACK in graph g (some non-terminal nodes are BLACK).
     * inv 2: BLACK nodes do form a connected subgraph of g.
     * inv 3: edges is a min weight spanning tree of the BLACK nodes in which every non-terminal has degree at least 2.
     */

    individual_t *ind = individual_alloc(p_g, p_mst_el);

    return (ind);
}

/**
 *
 * @param g
 * @param individual1
 * @param individual2
 * @return
 */
individual_t *individual_union(graph_t *p_g, individual_t *individual1, individual_t *individual2) {
    assert(g);
    assert(individual1);
    assert(individual2);

    /* empty list to store the edges from individual 1 and individual2 */
    list_t *l = NULL;

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < individual1->n_edges && i2 < individual2->n_edges) {
        if (edge_compar(individual1->edges[i1], individual2->edges[i2]) < 0) {
            l = list_insert_front(l, individual1->edges[i1]);
            i1++;
        } else {
            if (edge_compar(individual1->edges[i1], individual2->edges[i2]) > 0) {
                l = list_insert_front(l, individual2->edges[i1]);
                i2++;
            } else {
                /* edge_compar(individual1->edges[i1], individual2->edges[i2]) == 0 */
                l = list_insert_front(l, individual1->edges[i1]);
                i1++;
                i2++;
            }
        }
    }

    /* add remaining edges from individual1 */
    while (i1 < individual1->n_edges) {
        l = list_insert_front(l, individual1->edges[i1]);
        i1++;
    }

    /* add remaining edges from individual2 */
    while (i2 < individual2->n_edges) {
        l = list_insert_front(l, individual2->edges[i2]);
        i2++;
    }

    /* create a new random individual based on l */
    individual_t *union_individual = individual_mk_with_init_edges(g, l);

    /* get rid of initializing edges (do not release data !) */
    list_release(il);

    return (union_individual);
}

/**
 *
 * @param g
 * @param individual1
 * @param individual2
 * @return
 */
individual_t *individual_intersection(graph_t *p_g, individual_t *individual1, individual_t *individual2) {
    assert(g);
    assert(individual1);
    assert(individual2);

    /* empty list to store the edges from individual 1 and individual2 */
    list_t *l = NULL;

    /* add edges from individual 1 and individual2 (edges have to be part of the two) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < individual1->n_edges && i2 < individual2->n_edges) {
        if (edge_compar(individual1->edges[i1], individual2->edges[i2]) < 0) {
            i1++;
        } else {
            if (edge_compar(individual1->edges[i1], individual2->edges[i2]) > 0) {
                i2++;
            } else {
                /* edge_compar(individual1->edges[i1], individual2->edges[i2]) == 0 */
                l = list_insert_front(l, individual1->edges[i1]);
                i1++;
                i2++;
            }
        }
    }

    /* create a new random individual based on l */
    individual_t *intersection_individual = individual_mk_with_init_edges(g, l);

    /* get rid of initializing edges (do not release data !) */
    list_release(il);

    return (intersection_individual);
}

/**
 *
 * @param g
 * @param individual1
 * @param individual2
 * @param p
 * @return
 */
pair_t individual_crossing(graph_t *p_g, individual_t *individual1, individual_t *individual2, double p) {
    assert(g);
    assert(individual1);
    assert(individual2);

    /* empty list to store the edges from individual 1 and individual2 */
    list_t *l1 = NULL;
    list_t *l2 = NULL;

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    int i = 0;
    while (i < individual1->n_edges) {
        if (random_probability() <= p) {
            l1 = list_insert_front(l1, individual1->edges[i]);
        } else {
            l2 = list_insert_front(l2, individual1->edges[i]);
        }
        i++;
    }

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    int i = 0;
    while (i < individual2->n_edges) {
        if (random_probability() <= p) {
            l1 = list_insert_front(l1, individual2->edges[i]);
        } else {
            l2 = list_insert_front(l2, individual2->edges[i]);
        }
        i++;
    }

    /* create a new random individual based on l */
    individual_t *cross1_individual = individual_mk_with_init_edges(g, l1);
    individual_t *cross2_individual = individual_mk_with_init_edges(g, l2);

    /* get rid of initializing edges (do not release data !) */
    list_release(l1);
    list_release(l2);

    pair_t pair;
    pair.data1 = cross1_individual;
    pair.data2 = cross2_individual;

    return (pair);
}

/**
 *
 * @param g
 * @param individual
 * @param p
 * @return
 */
individual_t *individual_mutate(graph_t *p_g, individual_t *individual, double p) {
    assert(g);
    assert(individual);

    /* empty list to store the edges from individual 1 and individual2 */
    list_t *l = NULL;

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    int i = 0;
    while (i < individual->n_edges) {
        if (random_probability() <= p) {
            l1 = list_insert_front(l1, individual->edges[i]);
        }
        i++;
    }

    /* create a new random individual based on l */
    individual_t *mutate_individual = individual_mk_with_init_edges(g, l);

    /* get rid of initializing edges (do not release data !) */
    list_release(l);

    return (mutate_individual);
}

/**
 *
 * @param p_g
 * @param individual
 */
void individual_print_brief(graph_t *p_g, individual_t *individual) {
    assert(individual);

    graph_node_color_assert_all_white(p_g);
    graph_node_color_set_all(p_g, WHITE);

    /* count the number of non-terminal nodes */
    for (int i = 0; i < individual->n_edges; i++) {
        node_t n1 = individual->edges[i].n1;
        node_t n2 = individual->edges[i].n2;
        graph_node_color_set(p_g, n1, BLACK);
        graph_node_color_set(p_g, n2, BLACK);
    }
    size_t n_non_terminals = 0;
    for (node_t u = 0; u < p_g->n_nodes; u++) {
        if (graph_node_is_non_terminal(p_g, u) && graph_node_color_get(p_g, u) == BLACK) {
            n_non_terminals++;
        }
    }

    /* reset graph node's color */
    graph_node_color_set_all(p_g, WHITE);

    fprintf(stdout, "individual. total weight=%u\t#edges=%u\t#non terminals\n", individual->total_weight, individual->n_edges, n_non_terminals);
}
