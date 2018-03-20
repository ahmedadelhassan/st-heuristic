#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "edge.h"
#include "individual.h"
#include "edge_list.h"
#include "random.h"
#include "probability.h"
#include "utils.h"

/**
 *
 * @param p_el
 * @return
 */
static individual_t individual_init(edge_list_t *p_el) {
    individual_t individual;

    individual.n_edges = 0;
    individual.p_edges = NULL;
    individual.total_weight = 0;

    if (!p_el) {
        return (individual);
    }

    individual.n_edges = edge_list_size(p_el);
    individual.p_edges = (edge_t *) calloc(individual.n_edges, sizeof(edge_t));
    if (!individual.p_edges) {
        fprintf(stderr, "individual_init. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    /* copy edges and compute total weight */
    int i = 0;
    while (p_el) {
        edge_t e = p_el->edge;
        individual.total_weight += e.weight;
        individual.p_edges[i] = e;
        p_el = p_el->p_next;
        i++;
    }

    /* edges are sorted according to their endpoints */
    qsort(individual.p_edges, individual.n_edges, sizeof(edge_t), edge_compar_by_endpoints);

    return (individual);
}

/**
 *
 * @param individual
 */
void individual_cleanup(individual_t individual) {
    if (individual.p_edges) {
        memset(individual.p_edges, 0x0, individual.n_edges * sizeof(edge_t));
        free(individual.p_edges);
    }
}

/**
 *
 * @param p_g A pointer to a graph
 * @param p_init_el A list of edges of \a p_g
 * @return a list of edges that induce a connected component including all terminal nodes.
 */
static edge_list_t *individual_mk_init(graph_t *p_g, edge_list_t *p_init_el) {
    /* new union find */
    graph_union_find_init(p_g);

    /* edge_list_ of kept edges */
    edge_list_t *p_el = NULL;

    /* add initializing edges (if any) */
    while (p_init_el) {
        edge_t e = p_init_el->edge;
        node_t n1 = e.n1;
        node_t n2 = e.n2;

        /* add the edge regardless of the union_find step */
        graph_union_find_union(p_g, n1, n2);
        p_el = edge_list_insert_front(p_el, e);
        p_init_el = p_init_el->p_next;
    }

    /* random shuffle the edges of the reference graph */
    graph_edges_random_shuffle(p_g);

    /* add edges one by one until all terminal nodes are part of the same connected component */
    int i = 0;
    int max_connected_terminal_nodes = graph_union_get_max_connected_terminal_nodes(p_g);
    while (i < p_g->n_edges && max_connected_terminal_nodes < p_g->n_terminal_nodes) {
        edge_t e = p_g->p_edges_no_order_guaranteed[i];
        if (graph_union_find_union(p_g, e.n1, e.n2)) {
            p_el = edge_list_insert_front(p_el, e);
        }
        i++;
        max_connected_terminal_nodes = graph_union_get_max_connected_terminal_nodes(p_g);
    }

    /* keep only edges that are part of the connected component that includes all terminal nodes */
    node_t fst_terminal_node = p_g->fst_terminal_node;
    node_t fst_terminal_node_root = graph_union_find_find(p_g, fst_terminal_node);
    edge_list_t *p_cc_el = NULL;
    while (p_el) {
        edge_t e = p_el->edge;
        edge_list_t *p_next_el = p_el->p_next;

        if (graph_union_find_find(p_g, e.n1) == fst_terminal_node_root) {
            assert(graph_union_find_find(p_g, e.n2) == fst_terminal_node_root);
            p_el->p_next = p_cc_el;
            p_cc_el = p_el;
        } else {
            p_el->p_next = NULL;
            edge_list_release(p_el);
        }
        p_el = p_next_el;
    }

    return (p_cc_el);
}

/**
 *
 * @param p_g
 * @param p_el
 * @return
 */
/*
static edge_list_t *individual_mk_spanning_tree(graph_t *p_g, edge_list_t *p_el) {
    assert(p_g);

    if (!p_el) {
        return (NULL);
    }

    */
/* sort edges by increasing weight *//*

    p_el = edge_list_copy(p_el);
    p_el = edge_list_sort_by_weight(p_el);


    */
/* minimum spanning tree in p_el *//*

    graph_union_find_init(p_g);
    edge_list_t *p_it_el = p_el;
    edge_list_t *p_sp_el = NULL;
    while (p_it_el) {
        edge_t e = p_it_el->edge;
        if (graph_union_find_union(p_g, e.n1, e.n2)) {
            p_sp_el = edge_list_insert_front(p_sp_el, e);
        }
        p_it_el = p_it_el->p_next;
    }

    return (p_sp_el);
}
*/

/**
 *
 * @param p_g
 * @param p_el a list of edges
 * @return a list of edges
 */
static int individual_mk_prune(graph_t *p_g, edge_list_t *p_el) {
    int do_white = 0;

    /* reset node's counters */
    edge_list_t *p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el->edge;
        graph_node_counter_reset(p_g, e.n1);
        graph_node_counter_reset(p_g, e.n2);
        p_it_el = p_it_el->p_next;
    }

    /* compute the degree of each node in the spanning tree */
    p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el->edge;
        graph_node_counter_increment(p_g, e.n1);
        graph_node_counter_increment(p_g, e.n2);
        p_it_el = p_it_el->p_next;
    }

    /* color WHITE all BLACK nodes of p_el that does contain a non-terminal node with degree 1 */
    p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el->edge;
        if (graph_node_is_non_terminal(p_g, e.n1) && graph_node_counter_get(p_g, e.n1) == 1) {
            graph_node_color_set(p_g, e.n1, WHITE);
            do_white++;
        }
        if (graph_node_is_non_terminal(p_g, e.n2) && graph_node_counter_get(p_g, e.n2) == 1) {
            graph_node_color_set(p_g, e.n2, WHITE);
            do_white++;
        }
        p_it_el = p_it_el->p_next;
    }

    return (do_white);
}
/*

*/
/**
 *
 * @param p_g
 * @param p_el
 * @return
 *//*

#ifdef INDIVIDUAL_MK_SIMPLE

static individual_t individual_mk_simple(graph_t *p_g, edge_list_t *p_el) {
    assert(p_g);

    */
/* random edges that induce a connected component including all terminal nodes *//*

    edge_list_t *p_init_el = individual_mk_init(p_g, p_el);

    */
/* (non necessarily minimum) spanning tree that includes all terminal nodes *//*

    edge_list_t *p_sp_el = individual_mk_spanning_tree(p_g, p_init_el);

    */
/* releasing *//*

    edge_list_release(p_init_el);

    */
/* (non necessarily minimum) spanning tree that includes all terminal nodes, all non-terminal nodes have degree at least 2 *//*

    int dont_care_flag = 0;
    edge_list_t *p_pruned_sp_el = individual_mk_prune(p_g, p_sp_el, ADDR(dont_care_flag));

    */
/* releasing *//*

    edge_list_release(p_sp_el);

    */
/* build an individual from edges *//*

    individual_t individual = individual_init(p_pruned_sp_el);

    */
/* releasing *//*

    edge_list_release(p_pruned_sp_el);

    return (individual);
}
*/

/**
 *
 * @param p_g
 * @param p_el
 * @return
 */
static individual_t individual_mk_reduced(graph_t *p_g, edge_list_t *p_el) {
    assert(p_g);

    /* random edges that induce a connected component including all terminal nodes */
    edge_list_t *p_init_el = individual_mk_init(p_g, p_el);

    /* color the nodes of p_init_el BLACK */
    graph_node_color_assert_all(p_g, WHITE);
    edge_list_t *p_it_init_el = p_init_el;
    while (p_it_init_el) {
        edge_t e = p_it_init_el->edge;
        graph_node_color_set(p_g, e.n1, BLACK);
        graph_node_color_set(p_g, e.n2, BLACK);
        p_it_init_el = p_it_init_el->p_next;
    }

    int done = 0;
    edge_list_t *p_mst_el = NULL;
    do {
        /* compute a minimum spanning tree on BLACK vertices */
        p_mst_el = graph_kruskal_min_spanning_tree_on_black_nodes(p_g);

        /* remove edges that do contain a non-terminal node with degree 1 */
        int pruned = individual_mk_prune(p_g, p_mst_el);

        if (pruned) {
            /* cleaning: release the edges of the minimum spanning tree on BLACK nodes */
            edge_list_release(p_mst_el);
            p_mst_el = NULL;
            done = 0;
        } else {
            done = 1;
        }
    } while (!done);

    /* cleaning: color WHITE the nodes of p_mst_el */
    edge_list_t *p_it_mst_el = p_mst_el;
    while (p_it_mst_el) {
        edge_t e = p_it_mst_el->edge;
        graph_node_color_set(p_g, e.n1, WHITE);
        graph_node_color_set(p_g, e.n2, WHITE);
        p_it_mst_el = p_it_mst_el->p_next;
    }

    /* make individual */
    individual_t individual = individual_init(p_mst_el);

    /* cleaning: release the edges of the minimum spanning tree on BLACK nodes */
    edge_list_release(p_mst_el);

    return (individual);
}

/**
 *
 * @param p_g
 * @param p_el
 * @return
 */
individual_t individual_mk(graph_t *p_g, edge_list_t *p_el) {
    return (individual_mk_reduced(p_g, p_el));
}

/**
 *
 * @param p_g
 * @param individual1
 * @param individual2
 * @return
 */
individual_t individual_union(graph_t *p_g, individual_t individual1, individual_t individual2) {
    assert(p_g);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_el = NULL;

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < individual1.n_edges && i2 < individual2.n_edges) {
        edge_t e1 = individual1.p_edges[i1];
        edge_t e2 = individual2.p_edges[i2];

        if (edge_compar_by_endpoints(ADDR(e1), ADDR(e2)) < 0) {
            p_el = edge_list_insert_front(p_el, e1);
            i1++;
        } else {
            if (edge_compar_by_endpoints(ADDR(e1), ADDR(e2)) > 0) {
                p_el = edge_list_insert_front(p_el, e2);
                i2++;
            } else {
                /* edge_compar_by_endpoints(ADDR(e1), ADDR(e2)) == 0 */
                p_el = edge_list_insert_front(p_el, e1);
                i1++;
                i2++;
            }
        }
    }

    /* add remaining edges from individual1 */
    while (i1 < individual1.n_edges) {
        edge_t e = individual1.p_edges[i1];
        p_el = edge_list_insert_front(p_el, e);
        i1++;
    }

    /* add remaining edges from individual2 */
    while (i2 < individual2.n_edges) {
        edge_t e = individual2.p_edges[i1];
        p_el = edge_list_insert_front(p_el, e);
        i2++;
    }

    /* create a new random individual based on l */
    individual_t union_individual = individual_mk(p_g, p_el);

    /* get rid of initializing edges (do not release data !) */
    edge_list_release(p_el);

    return (union_individual);
}

/**
 *
 * @param p_g
 * @param individual1
 * @param individual2
 * @return
 */
individual_t individual_intersection(graph_t *p_g, individual_t individual1, individual_t individual2) {
    assert(p_g);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_el = NULL;

    /* add edges from individual 1 and individual2 (edges have to be part of the two) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < individual1.n_edges && i2 < individual2.n_edges) {
        edge_t e1 = individual1.p_edges[i1];
        edge_t e2 = individual2.p_edges[i2];

        if (edge_compar_by_endpoints(ADDR(e1), ADDR(e2)) < 0) {
            i1++;
        } else {
            if (edge_compar_by_endpoints(ADDR(e1), ADDR(e2)) > 0) {
                i2++;
            } else {
                /* edge_compar_by_endpoints(ADDR(e1), ADDR(e2)) == 0 */
                p_el = edge_list_insert_front(p_el, e1);
                i1++;
                i2++;
            }
        }
    }

    /* create a new random individual based on l */
    individual_t intersection_individual = individual_mk(p_g, p_el);

    /* get rid of initializing edges (do not release data !) */
    edge_list_release(p_el);

    return (intersection_individual);
}

/**
 *
 * @param p_g
 * @param individual1
 * @param individual2
 * @param p
 * @return
 */
individuals2_t
individual_crossing(graph_t *p_g, individual_t individual1, individual_t individual2, double probability) {
    assert(p_g);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_el1 = NULL;
    edge_list_t *p_el2 = NULL;

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    for (int i = 0; i < individual1.n_edges; i++) {
        if (probability_rand() <= probability) {
            edge_t e = individual1.p_edges[i];
            p_el1 = edge_list_insert_front(p_el1, e);
        } else {
            edge_t e = individual1.p_edges[i];
            p_el2 = edge_list_insert_front(p_el2, e);
        }
    }

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    for (int i = 0; i < individual2.n_edges; i++) {
        if (probability_rand() <= probability) {
            edge_t e = individual2.p_edges[i];
            p_el1 = edge_list_insert_front(p_el1, e);
        } else {
            edge_t e = individual2.p_edges[i];
            p_el2 = edge_list_insert_front(p_el2, e);
        }
    }

    /* create a new random individual based on l */
    individual_t crossed_individual1 = individual_mk(p_g, p_el1);
    individual_t crossed_individual2 = individual_mk(p_g, p_el2);

    /* get rid of initializing edges (do not release data !) */
    edge_list_release(p_el1);
    edge_list_release(p_el2);

    individuals2_t individuals2;
    individuals2.individual1 = crossed_individual1;
    individuals2.individual2 = crossed_individual2;

    return (individuals2);
}

/**
 *
 * @param p_g
 * @param individual
 * @param probability
 * @return
 */
individual_t individual_drop_out(graph_t *p_g, individual_t individual, double probability) {
    assert(p_g);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_l = NULL;

    /* drop out edges with probability p */
    for (int i = 0; i < individual.n_edges; i++) {
        if (probability_rand() > probability) {
            /* probability is for dropping out edges */
            edge_t e = individual.p_edges[i];
            p_l = edge_list_insert_front(p_l, e);
        }
    }

    /* create a new random individual based on l */
    individual_t p_dropped_individual = individual_mk(p_g, p_l);

    /* get rid of initializing edges (do not release data !) */
    edge_list_release(p_l);

    return (p_dropped_individual);
}

/**
 *
 * @param p_g
 * @param individual
 */
void individual_fprint(FILE *f, graph_t *p_g, individual_t individual) {
    graph_node_color_assert_all_white(p_g);
    graph_node_color_set_all(p_g, WHITE);

    /* color non-terminal nodes BLACK */
    for (int i = 0; i < individual.n_edges; i++) {
        edge_t e = individual.p_edges[i];
        node_t n1 = e.n1;
        node_t n2 = e.n2;
        graph_node_color_set(p_g, n1, BLACK);
        graph_node_color_set(p_g, n2, BLACK);
    }
    size_t n_non_terminals = 0;
    for (node_t u = 0; u < p_g->n_nodes; u++) {
        if (graph_node_is_non_terminal(p_g, u) && graph_node_color_get(p_g, u) == BLACK) {
            n_non_terminals++;
        }
    }

    /* color non-terminal nodes WHITE */
    for (int i = 0; i < individual.n_edges; i++) {
        edge_t e = individual.p_edges[i];
        node_t n1 = e.n1;
        node_t n2 = e.n2;
        graph_node_color_set(p_g, n1, WHITE);
        graph_node_color_set(p_g, n2, WHITE);
    }

    /* reset graph node's color */
    graph_node_color_set_all(p_g, WHITE);

    fprintf(f, "individual. total weight=%u\t#edges=%zu\t#non terminals=%zu\n", individual.total_weight,
            individual.n_edges, n_non_terminals);
}
