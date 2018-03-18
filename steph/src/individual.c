#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "edge.h"
#include "individual.h"
#include "edge_list.h"
#include "random.h"
#include "probability.h"

static individual_t individual_init(edge_list_t *p_el) {
    individual_t individual;

    individual.n_edges = 0;
    individual.p_edges = NULL;
    individual.total_weight = 0;

    if (!p_el) {
        return (individual);
    }

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
        individual.n_edges += 1;
        individual.p_edges[i++] = e;
        p_el = p_el->p_next;
    }

    /* edges are sorted according to the edge_compar function (i.e., n1 and next n2) */
    qsort(individual.p_edges, individual.n_edges, sizeof(edge_t), edge_compar);

    return (individual);
}

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
        edge_t e = p_init_el.edge;
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
    while (graph_union_get_max_connected_terminal_nodes(p_g) < p_g->n_terminal_nodes && i < p_g->n_edges) {
        edge_t e = p_g->p_edges_no_order_guaranteed[i];
        if (graph_union_find_union(p_g, e.n1, e.n2)) {
            p_el = edge_list_insert_front(p_el, e);
        }
        i++;
    }

    /* keep only edges that are part of the connected component that includes all terminal nodes */
    node_t fst_terminal_node = p_g->fst_terminal_node;
    node_t fst_terminal_node_root = graph_union_find_find(p_g, fst_terminal_node);
    edge_list_t *p_cc_el = NULL;
    while (p_el) {
        edge_t e = p_el->edge;
        int drop_n1 = graph_union_find_find(p_g, n1) != fst_terminal_node_root;
        int drop_n2 = graph_union_find_find(p_g, n2) != fst_terminal_node_root;
        assert(drop_n1 == drop_n2);
        if (!drop_n1 && !drop_n2) {
            edge_list_t *p_next_el = p_el->p_next;
            p_el->p_next = p_cc_el;
            p_cc_el = p_el;
            p_el = p_next_el;
        } else {
            edge_list_t *p_next_el = p_el->p_next;
            p_el->p_next = NULL;
            edge_list_release(p_el);
            p_el = p_next_el;
        }
    }

    return (p_cc_el);
}

/**
 *
 * @param p_g
 * @param p_el
 * @return
 */
static edge_list_t individual_mk_spanning_tree(graph_t *p_g, edge_list_t *p_el) {
    assert(p_g);

    if (!p_el) {
        return (nULL);

    }
    /* sort edges by increasing weight */
    p_el = edge_list_copy(p_el);
    p_el = edge_list_sort_by_weight(p_el);

    /* */
    graph_union_find_init(p_g);
    edge_list_t *p_it_el = p_el;
    edge_list_t *p_kept_el = NULL;
    while (p_it_el) {
        edge_t e = p_it_el->edge;
        if (graph_union_find_union(p_g, e.n1, e.n2)) {
            edge_list_insert_front(p_kept_el, e);
        }
        p_it_el = p_it_el->p_next;
    }

    graph_union_find_cleanup(p_g);

    return (p_kept_el);
}

/**
 *
 * @param p_g
 * @param p_el a list of edges
 * @return a list of edges
 */
static edge_list_t *individual_mk_prune(graph_t *p_g, edge_list_t *p_el) {

    /* reset node's counters */
    edge_list_t *p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el.edge;
        graph_node_counter_reset(p_g, e.n1);
        graph_node_counter_reset(p_g, e.n2);
        p_it_el = p_it_el->p_next;
    }

    /* compute the degree of each node in the spanning tree */
    p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el.edge;
        graph_node_counter_increment(p_g, e.n1);
        graph_node_counter_increment(p_g, e.n2);
        p_it_el = p_it_el->p_next;
    }

    /* keep edges of p_el that does not contain a non-terminal node with degree 1 */
    edge_list_t *p_pruned_el = NULL;
    p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el.edge;
        int drop_n1 = graph_node_is_non_terminal(p_g, n1) && graph_node_counter_get(p_g, n1) == 1;
        int drop_n2 = graph_node_is_non_terminal(p_g, n2) && graph_node_counter_get(p_g, n2) == 1;
        if (!drop_n1 && !drop_n2) {
            p_pruned_el = edge_list_insert_front(p_pruned_el, e);
        }
        p_it_el = p_it_el->p_next;
    }

    return (p_pruned_el);
}

/**
 *
 * @param p_g
 * @param p_el
 * @return
 */
individual_t individual_mk(graph_t *p_g, edge_list_t *p_el) {
    assert(p_g);

    /* random edges that induce a connected component including all terminal nodes */
    edge_list_t *p_init_el = individual_mk_init(p_g, p_el);

    /* (non necessarily minimum) spanning tree that includes all terminal nodes */
    edge_list_t *p_sp_el = individual_mk_spanning_tree(p_g, p_init_el);

    /* releasing */
    edge_list_release(p_init_el);

    /* (non necessarily minimum) spanning tree that includes all terminal nodes, all non-terminal nodes have degree at least 2 */
    edge_list_t *p_pruned_sp_el = individual_mk_prune(p_g, p_sp_el);

    /* releasing */
    edge_list_release(p_sp_el);

    /* build an individual from edges */
    individual_t individual = individual_init(p_pruned_sp_el);

    /* releasing */
    edge_list_release(p_pruned_sp_el);

    return(individual);
}

/**
 *
 * @param p_g
 * @param p_el
 * @return
 */
individual_t individual_mk_reduced(graph_t *p_g, edge_list_t *p_el) {
    assert(p_g);

    /* random edges that induce a connected component including all terminal nodes */
    edge_list_t *p_el = individual_mk_init(p_g, p_el);


    /* pruning degree 1 non-terminal nodes in minimum spanning tree */

    int done = 0;
    edge_list_t *p_mst_el = NULL;
    do {
        /*
         * inv 1: terminal nodes are BLACK in graph p_g (some non-terminal nodes are also BLACK).
         * inv 2: BLACK nodes do form a connected subgraph of p_g.
         */

        /* compute a minimum spanning tree on BLACK vertices */
        p_mst_el = graph_kruskal_min_spanning_tree_on_black_nodes(p_g);



        fprintf(stdout, "found=%d\n", found);
        fflush(stdout);

        if (!found) {
            /* we did find at least one non-terminal BLACK node with degree 1 in the mst */
            done = ~0;
        }
    } while (!done);

    fprintf(stdout, "p_mst_el=%p\n", p_mst_el);
    fprintf(stdout, "edge_list__size(p_mst_el)=%lu\n", edge_list__size(p_mst_el));
    fprintf(stdout, "temp exit\n");

    /*
     * inv 1: terminal nodes are BLACK in graph g (some non-terminal nodes are BLACK).
     * inv 2: BLACK nodes do form a connected subgraph of g.
     * inv 3: edges is a min weight spanning tree of the BLACK nodes in which every non-terminal has degree at least 2.
     */

    individual_t individual = individual_init(p_mst_el);

    return (individual);
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
    assert(individual1);
    assert(individual2);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_l = NULL;

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < individual1.n_edges && i2 < individual2.n_edges) {
        edge_t *p_e1 = individual1.p_edges[i1];
        edge_t *p_e2 = individual2.p_edges[i2];

        if (edge_compar(p_e1, p_e2) < 0) {
            edge_t *p_e = individual1.p_edges[i1];
            p_l = edge_list_insert_front(p_l, p_e);
            i1++;
        } else {
            if (edge_compar(p_e1, p_e2) > 0) {
                edge_t *p_e = individual2.p_edges[i1];
                p_l = edge_list_insert_front(p_l, p_e);
                i2++;
            } else {
                /* edge_compar(p_e1, p_e2) == 0 */
                edge_t *p_e = individual1.p_edges[i1];
                p_l = edge_list_insert_front(p_l, p_e);
                i1++;
                i2++;
            }
        }
    }

    /* add remaining edges from individual1 */
    while (i1 < individual1.n_edges) {
        edge_t *p_e = individual1.p_edges[i1];
        p_l = edge_list_insert_front(p_l, p_e);
        i1++;
    }

    /* add remaining edges from individual2 */
    while (i2 < individual2.n_edges) {
        edge_t *p_e = individual2.p_edges[i1];
        p_l = edge_list_insert_front(p_l, p_e);
        i2++;
    }

    /* create a new random individual based on l */
    individual_t union_individual = individual_mk_with_init_edges(p_g, p_l);

    /* get rid of initializing edges (do not release data !) */
    edge_list__release(p_l);

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
    assert(individual1);
    assert(individual2);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_l = NULL;

    /* add edges from individual 1 and individual2 (edges have to be part of the two) */
    int i1 = 0;
    int i2 = 0;
    while (i1 < individual1.n_edges && i2 < individual2.n_edges) {
        edge_t *p_e1 = individual1.p_edges[i1];
        edge_t *p_e2 = individual2.p_edges[i2];

        if (edge_compar(p_e1, p_e2) < 0) {
            i1++;
        } else {
            if (edge_compar(p_e1, p_e2) > 0) {
                i2++;
            } else {
                /* edge_compar(p_e1, p_e2) == 0 */
                p_l = edge_list_insert_front(p_l, p_e1);
                i1++;
                i2++;
            }
        }
    }

    /* create a new random individual based on l */
    individual_t intersection_individual = individual_mk_with_init_edges(p_g, p_l);

    /* get rid of initializing edges (do not release data !) */
    edge_list__release(p_l);

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
individuals2 individual_crossing(graph_t *p_g, individual_t individual1, individual_t individual2, double probability) {
    assert(p_g);
    assert(individual1);
    assert(individual2);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_el1 = NULL;
    edge_list_t *p_el2 = NULL;

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    for (int i = 0; i < individual1.n_edges; i++) {
        if (probability_rand() <= probability) {
            edge_t *p_e = individual1.p_edges[i];
            p_el1 = edge_list_insert_front(p_el1, p_e);
        } else {
            edge_t *p_e = individual1.p_edges[i];
            p_el2 = edge_list_insert_front(p_el2, p_e);
        }
    }

    /* add edges from individual 1 and individual2 (do not add duplicate) */
    for (int i = 0; i < individual2.n_edges; i++) {
        if (probability_rand() <= probability) {
            edge_t *p_e = individual2.p_edges[i];
            p_el1 = edge_list_insert_front(p_el1, p_e);
        } else {
            edge_t *p_e = individual2.p_edges[i];
            p_el2 = edge_list_insert_front(p_el2, p_e);
        }
    }

    /* create a new random individual based on l */
    individual_t crossed_individual1 = individual_mk_with_init_edges(p_g, p_el1);
    individual_t crossed_individual2 = individual_mk_with_init_edges(p_g, p_el2);

    /* get rid of initializing edges (do not release data !) */
    edge_list__release(p_el1);
    edge_list__release(p_el2);

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
    assert(individual);

    /* empty edge_list_ to store the edges from individual 1 and individual2 */
    edge_list_t *p_l = NULL;

    /* drop out edges with probability p */
    for (int i = 0; i < individual.n_edges; i++) {
        if (probability_rand() > probability) {
            /* probability is for dropping out edges */
            edge_t *p_e = individual.p_edges[i];
            p_l = edge_list_insert_front(p_l, p_e);
        }
    }

    /* create a new random individual based on l */
    individual_t p_dropped_individual = individual_mk_with_init_edges(p_g, p_l);

    /* get rid of initializing edges (do not release data !) */
    edge_list__release(p_l);

    return (p_dropped_individual);
}

/**
 *
 * @param p_g
 * @param individual
 */
void individual_print(graph_t *p_g, individual_t individual) {
    assert(individual);

    graph_node_color_assert_all_white(p_g);
    graph_node_color_set_all(p_g, WHITE);

    /* count the number of non-terminal nodes */
    for (int i = 0; i < individual.n_edges; i++) {
        node_t n1 = individual.p_edges[i]->n1;
        node_t n2 = individual.p_edges[i]->n2;
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

    fprintf(stdout, "individual. total weight=%u\t#edges=%zu\t#non terminals=%zu\n", individual.total_weight,
            individual.n_edges, n_non_terminals);
}

/**
 *
 * @param p1
 * @param p2
 * @return
 */
int int individual_compar(individual_t individual1, individual_t individual2) {
    return (individual1.total_weight - individual2.total_weight);
}
