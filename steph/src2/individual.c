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
static individual_t individual_init(pool_t *p_pool) {
    assert(p_pool);

    individual_t individual;
    individual.p_bvector = pool_get(p_pool);
    individual.total_weight = 0;

    return (individual);
}

/**
 *
 * @param individual
 */
void individual_cleanup(pool_t *p_pool, individual_t individual) {
    pool_return(p_pool, individual.p_bvector);
}

/**
 *
 * @param p_graph A pointer to a graph
 * @param p_init_el A list of edges of \a p_graph
 * @return a list of edges that induce a connected component including all terminal nodes.
 */
static void individual_inflate(pool_t *p_pool, graph_t *p_graph) {
    assert(p_pool);
    assert(p_graph);

    /* always add terminal nodes */
    bvector_or(p_graph->p_bvector, p_graph->p_bvector, p_graph->p_terminal_bvector);

    /* init union find */
    graph_union_find_init(p_graph);

    int stage = 1;
    while (1) {
        graph_edges_random_shuffle(p_graph);

        for (int i = 0; i < p_graph->n_edges; i++) {
            /* test for a connected component in selected nodes */
            if (graph_union_find_done(p_graph)) {
               return;
            }

            /* add (modulo random rejection) another edge */
            edge_t e = p_graph->p_edges_no_order_guaranteed[i];
            graph_union_find_union(p_graph, e.n1, e.n2, stage);
        }

        stage++;
    }

 }

/**
 *
 * @param p_graph
 * @param p_el a list of edges
 * @return a list of edges
 */
static int individual_deflate(graph_t *p_graph, edge_list_t *p_el) {
    int done = 1;

    /* reset node's counters */
    edge_list_t *p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el->edge;
        graph_node_counter_reset(p_graph, e.n1);
        graph_node_counter_reset(p_graph, e.n2);
        p_it_el = p_it_el->p_next;
    }

    /* compute the degree of each node in the spanning tree */
    p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el->edge;
        graph_node_counter_increment(p_graph, e.n1);
        graph_node_counter_increment(p_graph, e.n2);
        p_it_el = p_it_el->p_next;
    }

    /* remove all non-terminal node degree 1 nodes */
    p_it_el = p_el;
    while (p_it_el) {
        edge_t e = p_it_el->edge;

        if (graph_node_is_non_terminal(p_graph, e.n1)) {
            if (graph_node_counter_get(p_graph, e.n1) == 1) {
                bvector_unset(p_graph->p_bvector, e.n1);
                done = 0;
            }
        }

        if (graph_node_is_non_terminal(p_graph, e.n2)) {
            if (graph_node_counter_get(p_graph, e.n2) == 1) {
                bvector_unset(p_graph->p_bvector, e.n2);
                done = 0;
            }
        }

        p_it_el = p_it_el->p_next;
    }

    return (done);
}

/**
 *
 * @param p_pool
 * @param p_graph
 * @param p_bvector
 * @return
 */
individual_t individual_mk(pool_t *p_pool, graph_t *p_graph) {
    assert(p_pool);
    assert(p_graph);

    /* random superset of selected nodes */
    individual_inflate(p_pool, p_graph);

    int done = 0;
    edge_list_t *p_mst_el = NULL;
    weight_t total_weight = 0;
    do {
        /* compute a minimum spanning tree on selected nodes */
        p_mst_el = graph_mst(p_graph);

        /* remove non-terminal degree 1 nodes in the minimum spanning tree */
        done = individual_deflate(p_graph, p_mst_el);

        if (done) {
            total_weight = edge_list_total_weight(p_mst_el);
        }

        edge_list_release(p_mst_el);
    } while (!done);

    /* make individual */
    individual_t individual = individual_init(p_pool);
    individual.total_weight = total_weight;
    bvector_copy(individual.p_bvector, p_graph->p_bvector);

    /* cleaning: unset all nodes */
    bvector_unset_all(p_graph->p_bvector);

    return (individual);
}

/**
 *
 * @param p_graph
 * @param individual1
 * @param individual2
 * @return
 */
individual_t individual_union(pool_t *p_pool, graph_t *p_graph, individual_t individual1, individual_t individual2) {
    assert(p_pool);
    assert(p_graph);

    bvector_t *p_bvector = pool_get(p_pool);

    for (int i = 0; i < p_graph->n_nodes; i++) {
        if (bvector_get(individual1.p_bvector, i) || bvector_get(individual2.p_bvector, i)) {
            bvector_set(p_bvector, i);
        }
    }

    bvector_copy(p_graph->p_bvector, p_bvector);
    pool_return(p_pool, p_bvector);
    individual_t new_individual = individual_mk(p_pool, p_graph);

    return (new_individual);
}

/**
 *
 * @param p_graph
 * @param individual1
 * @param individual2
 * @return
 */
individual_t
individual_intersection(pool_t *p_pool, graph_t *p_graph, individual_t individual1, individual_t individual2) {
    assert(p_pool);
    assert(p_graph);

    bvector_t *p_bvector = pool_get(p_pool);

    for (int i = 0; i < p_graph->n_nodes; i++) {
        if (bvector_get(individual1.p_bvector, i) && bvector_get(individual2.p_bvector, i)) {
            bvector_set(p_bvector, i);
        }
    }

    bvector_copy(p_graph->p_bvector, p_bvector);
    pool_return(p_pool, p_bvector);
    individual_t new_individual = individual_mk(p_pool, p_graph);

    return (new_individual);
}


individuals_t
individual_crossing(pool_t *p_pool, graph_t *p_graph, individual_t individual1, individual_t individual2) {
    assert(p_graph);

    bvector_t *p_bvector1 = pool_get(p_pool);
    bvector_t *p_bvector2 = pool_get(p_pool);

    int cross_point = rand() % p_graph->n_nodes;

    for (int i = 0; i < cross_point; i++) {
        if (bvector_get(individual1.p_bvector, i)) {
            bvector_set(p_bvector1, i);
        }
        if (bvector_get(individual2.p_bvector, i)) {
            bvector_set(p_bvector2, i);
        }
    }

    for (int i = cross_point; i < p_graph->n_nodes; i++) {
        if (bvector_get(individual1.p_bvector, i)) {
            bvector_set(p_bvector2, i);
        }
        if (bvector_get(individual2.p_bvector, i)) {
            bvector_set(p_bvector1, i);
        }
    }

    individuals_t individuals;

    bvector_copy(p_graph->p_bvector, p_bvector1);
    pool_return(p_pool, p_bvector1);
    p_bvector1 = NULL;
    individuals.individual1 = individual_mk(p_pool, p_graph);

    bvector_copy(p_graph->p_bvector, p_bvector2);
    pool_return(p_pool, p_bvector2);
    p_bvector2 = NULL;
    individuals.individual2 = individual_mk(p_pool, p_graph);

    return (individuals);
}

/**
 *
 * @param p_pool
 * @param p_graph
 * @param individual
 * @param probability
 * @return
 */
individual_t individual_drop_out(pool_t *p_pool, graph_t *p_graph, individual_t individual, double probability) {
    assert(p_graph);
    assert(p_pool);

    bvector_t *p_bvector = pool_get(p_pool);

    for (int i = 1; i < p_graph->n_nodes; i++) {
        if (bvector_get(individual.p_bvector, i) && probability_rand() > probability) {
            bvector_set(p_bvector, i);
        }
    }

    bvector_copy(p_graph->p_bvector, p_bvector);
    pool_return(p_pool, p_bvector);
    individual_t new_individual = individual_mk(p_pool, p_graph);

    return (new_individual);
}

/**
 *
 * @param p_pool
 * @param p_graph
 * @param individual
 * @param probability
 * @return
 */
individual_t individual_insert(pool_t *p_pool, graph_t *p_graph, individual_t individual, double probability) {
    assert(p_pool);
    assert(p_graph);

    bvector_t *p_bvector = pool_get(p_pool);

    for (int i = 1; i < p_graph->n_nodes; i++) {
        if (!bvector_get(individual.p_bvector, i) && probability_rand() <= probability) {
            bvector_set(p_bvector, i);
        }
    }

    bvector_copy(p_graph->p_bvector, p_bvector);
    pool_return(p_pool, p_bvector);
    individual_t new_individual = individual_mk(p_pool, p_graph);

    return (new_individual);
}

/**
 *
 * @param p_graph
 * @param individual
 */
void individual_fprint(FILE *f, graph_t *p_graph, individual_t individual) {

}
