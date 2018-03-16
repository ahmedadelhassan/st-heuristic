#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "edge.h"
#include "individual.h"
#include "list.h"
#include "random.h"
#include "probability.h"

/**
 *
 * @param el
 * @return
 */
static individual_t *individual_alloc(list_t *p_el) {
    individual_t *p_individual = (individual_t *) malloc(sizeof(individual_t));
    if (!p_individual) {
        fprintf(stderr, "individual_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "individual_alloc. p_el=%p\n", p_el),
    fflush(stdout);

    if (!p_el) {
        p_individual->n_edges = 0;
        p_individual->p_edges = NULL;
    } else {
        p_individual->n_edges = list_size(p_el);
        p_individual->p_edges = (edge_t **) calloc(p_individual->n_edges, sizeof(edge_t*));
        p_individual->total_weight = 0;

        fprintf(stdout, "n_edges=%lu\n", p_individual->n_edges);
        fflush(stdout);

        /* copy edges and compute total weight */
        int i = 0;
        while (p_el) {
            edge_t *p_e = (edge_t*) p_el->data;

            edge_print(*p_e);
            fflush(stdout);

            p_individual->p_edges[i] = p_e;
            p_individual->total_weight += p_e->weight;
            p_el = p_el->p_next;
            i++;
        }
    }

    /* edges are sorted according to the edge_compar function (i.e., n1 and next n2) */
    qsort(p_individual->p_edges, p_individual->n_edges, sizeof(edge_t*), edge_compar_p);

    return (p_individual);
}

/**
 *
 * @param p_individual
 */
void individual_release(individual_t *p_individual) {
    if (!p_individual) {
        memset(p_individual->p_edges, 0x0, p_individual->n_edges * sizeof(edge_t*));
        free(p_individual->p_edges);

        memset(p_individual, 0x0, sizeof(individual_t));
        free(p_individual);
    }
}

/**
 *
 * @param p_g
 * @return
 */
individual_t *individual_mk(graph_t *p_g) {
    return (individual_mk_with_init_edges(p_g, NULL));
}

/**
 *
 * @param p_g
 * @param p_init_el
 * @return
 */
individual_t *individual_mk_with_init_edges(graph_t *p_g, list_t *p_init_el) {
    assert(p_g);

    /* new union find */
    graph_union_find_init(p_g);

    /* list of kept edges */
    list_t *p_el = NULL;

    /* add initializing edges (if any) */
    while (p_init_el) {
        edge_t *p_e = (edge_t *) p_init_el->data;
        node_t n1 = p_e->n1;
        node_t n2 = p_e->n2;

        /* add the edge regardless of the union_find step */
        graph_union_find_union(p_g, n1, n2);
        list_insert_front(p_el, p_e);
        p_init_el->data = NULL;
        p_init_el = p_init_el->p_next;
    }

    /* random shuffle the edges of the reference graph */
    graph_edges_random_shuffle(p_g);

    /* add edges one by one until all terminal nodes are part of the same connected component */
    for (int i = 0; !graph_union_find_terminals_are_connected(p_g) && i < p_g->n_edges; i++) {
        edge_t e = p_g->p_edges_no_order_guaranteed[i];
        if (graph_union_find_union(p_g, e.n1, e.n2)) {
            edge_t *p_e = graph_search_edge_by_endpoints(p_g, e);
            if (!p_e) {
                fprintf(stderr, "individual_mk_with_init_edges. cannot find edge (%u, %u, %u)\n", e.n1, e.n2, e.weight);
                exit(EXIT_FAILURE);
            }
            list_insert_front(p_el, p_e);
        }
    }

    /*
     * inv: terminals nodes are part of the same connected component.
     */

    /* color BLACK all nodes that are part of the terminal connected component, all other nodes are colored WHITE  */
    graph_node_color_assert_all(p_g, WHITE);
    graph_node_color_set_all(p_g, WHITE);
    node_t fst_terminal_node = p_g->fst_terminal_node;
    node_t fst_terminal_node_root = graph_union_find_find(p_g, fst_terminal_node);
    fprintf(stdout, "fst_terminal_node=%u, fst_terminal_node_root=%u\n", fst_terminal_node, fst_terminal_node_root);
    for (node_t n = 1; n < p_g->n_nodes; n++) {
        if (graph_union_find_find(p_g, n) == fst_terminal_node_root) {
            graph_node_color_set(p_g, n, BLACK);
        }
    }

    fprintf(stdout, "p_g->union_find.max_n_terminal_nodes_in_part=%lu\n", p_g->union_find.max_n_terminal_nodes_in_part);
    fflush(stdout);
    for (node_t n = 1; n < p_g->n_terminal_nodes; n++) {
        if (graph_node_is_terminal(p_g, n)) {
            fprintf(stdout, "terminal node=%u with color=%d\n", n, graph_node_color_get(p_g, n));
            graph_node_color_assert(p_g, n, BLACK);
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
            edge_t *p_e = (edge_t *) p_it_mst_el->data;
            graph_node_counter_reset(p_g, p_e->n1);
            graph_node_counter_reset(p_g, p_e->n2);
        }

        /* compute the degree of each node in the spanning tree */
        p_it_mst_el = p_it_mst_el;
        while (p_it_mst_el) {
            edge_t *p_e = (edge_t *) p_it_mst_el->data;
            graph_node_counter_increment(p_g, p_e->n1);
            graph_node_counter_increment(p_g, p_e->n2);
            p_it_mst_el = p_it_mst_el->p_next;
        }

        /* find (if it exists) a non-terminal BLACK node with degree 1 in the spanning tree */
        int found = 0;
        for (node_t n = 0; n < p_g->n_nodes; n++) {
            int n_is_non_terminal = graph_node_is_non_terminal(p_g, n);
            int n_is_black = graph_node_color_get(p_g, n) == BLACK;
            int n_counter = graph_node_counter_get(p_g, n);

            if (n_is_non_terminal && n_is_black && n_counter == 1) {
                found = ~0;
                graph_node_color_set(p_g, n, WHITE);
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

    individual_t *p_individual = individual_alloc(p_mst_el);

    return (p_individual);
}

/**
 *
 * @param p_g
 * @param p_individual1
 * @param p_individual2
 * @return
 */
individual_t *individual_union(graph_t *p_g, individual_t *p_individual1, individual_t *p_individual2) {
    assert(p_g);
    assert(p_individual1);
    assert(p_individual2);

    /* empty list to store the edges from individual 1 and p_individual2 */
    list_t *p_l = NULL;

    /* add edges from individual 1 and p_individual2 (do not add duplicate) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < p_individual1->n_edges && i2 < p_individual2->n_edges) {
        edge_t *p_e1 = p_individual1->p_edges[i1];
        edge_t *p_e2 = p_individual2->p_edges[i2];

        if (edge_compar(p_e1, p_e2) < 0) {
            edge_t *p_e = p_individual1->p_edges[i1];
            p_l = list_insert_front(p_l, p_e);
            i1++;
        } else {
            if (edge_compar(p_e1, p_e2) > 0) {
                edge_t *p_e = p_individual2->p_edges[i1];
                p_l = list_insert_front(p_l, p_e);
                i2++;
            } else {
                /* edge_compar(p_e1, p_e2) == 0 */
                edge_t *p_e = p_individual1->p_edges[i1];
                p_l = list_insert_front(p_l, p_e);
                i1++;
                i2++;
            }
        }
    }

    /* add remaining edges from p_individual1 */
    while (i1 < p_individual1->n_edges) {
        edge_t *p_e = p_individual1->p_edges[i1];
        p_l = list_insert_front(p_l, p_e);
        i1++;
    }

    /* add remaining edges from p_individual2 */
    while (i2 < p_individual2->n_edges) {
        edge_t *p_e = p_individual2->p_edges[i1];
        p_l = list_insert_front(p_l, p_e);
        i2++;
    }

    /* create a new random individual based on l */
    individual_t *p_union_individual = individual_mk_with_init_edges(p_g, p_l);

    /* get rid of initializing edges (do not release data !) */
    list_release(p_l);

    return (p_union_individual);
}

/**
 *
 * @param p_g
 * @param p_individual1
 * @param p_individual2
 * @return
 */
individual_t *individual_intersection(graph_t *p_g, individual_t *p_individual1, individual_t *p_individual2) {
    assert(p_g);
    assert(p_individual1);
    assert(p_individual2);

    /* empty list to store the edges from individual 1 and p_individual2 */
    list_t *p_l = NULL;

    /* add edges from individual 1 and p_individual2 (edges have to be part of the two) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < p_individual1->n_edges && i2 < p_individual2->n_edges) {
        edge_t *p_e1 = p_individual1->p_edges[i1];
        edge_t *p_e2 = p_individual2->p_edges[i2];

        if (edge_compar(p_e1, p_e2) < 0) {
            i1++;
        } else {
            if (edge_compar(p_e1, p_e2) > 0) {
                i2++;
            } else {
                /* edge_compar(p_e1, p_e2) == 0 */
                p_l = list_insert_front(p_l, p_e1);
                i1++;
                i2++;
            }
        }
    }

    /* create a new random individual based on l */
    individual_t *p_intersection_individual = individual_mk_with_init_edges(p_g, p_l);

    /* get rid of initializing edges (do not release data !) */
    list_release(p_l);

    return (p_intersection_individual);
}

/**
 *
 * @param p_g
 * @param p_individual1
 * @param p_individual2
 * @param p
 * @return
 */
pair_t individual_crossing(graph_t *p_g, individual_t *p_individual1, individual_t *p_individual2, double probability) {
    assert(p_g);
    assert(p_individual1);
    assert(p_individual2);

    /* empty list to store the edges from individual 1 and p_individual2 */
    list_t *p_el1 = NULL;
    list_t *p_el2 = NULL;

    /* add edges from individual 1 and p_individual2 (do not add duplicate) */
    for (int i = 0; i < p_individual1->n_edges; i++) {
        if (probability_rand() <= probability) {
            edge_t *p_e = p_individual1->p_edges[i];
            p_el1 = list_insert_front(p_el1, p_e);
        } else {
            edge_t *p_e = p_individual1->p_edges[i];
            p_el2 = list_insert_front(p_el2, p_e);
        }
    }

    /* add edges from individual 1 and p_individual2 (do not add duplicate) */
    for (int i = 0; i < p_individual2->n_edges; i++) {
        if (probability_rand() <= probability) {
            edge_t *p_e = p_individual2->p_edges[i];
            p_el1 = list_insert_front(p_el1, p_e);
        } else {
            edge_t *p_e = p_individual2->p_edges[i];
            p_el2 = list_insert_front(p_el2, p_e);
        }
    }

    /* create a new random individual based on l */
    individual_t *p_crossed_individual1 = individual_mk_with_init_edges(p_g, p_el1);
    individual_t *p_crossed_individual2 = individual_mk_with_init_edges(p_g, p_el2);

    /* get rid of initializing edges (do not release data !) */
    list_release(p_el1);
    list_release(p_el2);

    pair_t pair;
    pair.data1 = p_crossed_individual1;
    pair.data2 = p_crossed_individual2;

    return (pair);
}

/**
 *
 * @param p_g
 * @param p_individual
 * @param probability
 * @return
 */
individual_t *individual_drop_out(graph_t *p_g, individual_t *p_individual, double probability) {
    assert(p_g);
    assert(p_individual);

    /* empty list to store the edges from individual 1 and p_individual2 */
    list_t *p_l = NULL;

    /* drop out edges with probability p */
    for (int i = 0; i < p_individual->n_edges; i++) {
        if (probability_rand() > probability) {
            /* probability is for dropping out edges */
            edge_t *p_e = p_individual->p_edges[i];
            p_l = list_insert_front(p_l, p_e);
        }
    }

    /* create a new random individual based on l */
    individual_t *p_dropped_individual = individual_mk_with_init_edges(p_g, p_l);

    /* get rid of initializing edges (do not release data !) */
    list_release(p_l);

    return (p_dropped_individual);
}

/**
 *
 * @param p_g
 * @param p_individual
 */
void individual_print(graph_t *p_g, individual_t *p_individual) {
    assert(p_individual);

    graph_node_color_assert_all_white(p_g);
    graph_node_color_set_all(p_g, WHITE);

    /* count the number of non-terminal nodes */
    for (int i = 0; i < p_individual->n_edges; i++) {
        node_t n1 = p_individual->p_edges[i]->n1;
        node_t n2 = p_individual->p_edges[i]->n2;
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

    fprintf(stdout, "individual. total weight=%u\t#edges=%zu\t#non terminals=%zu\n", p_individual->total_weight,
            p_individual->n_edges, n_non_terminals);
}

/**
 *
 * @param p1
 * @param p2
 * @return
 */
int individual_compar(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    const individual_t *p_individual1 = (individual_t *) p1;
    const individual_t *p_individual2 = (individual_t *) p2;

    return (p_individual1->total_weight - p_individual2->total_weight);
}

int individual_compar_p(const void *p1, const void *p2) {
    assert(p1);
    assert(p2);

    const individual_t *p_individual1 = *((individual_t **) p1);
    const individual_t *p_individual2 = *((individual_t **) p2);

    return (p_individual1->total_weight - p_individual2->total_weight);
}