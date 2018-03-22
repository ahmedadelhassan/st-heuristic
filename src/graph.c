#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "bvector.h"
#include "graph.h"
#include "edge_list.h"
#include "probability.h"
#include "random.h"
#include "utils.h"

/**
 *
 * @param p_graph
 */
static void graph_union_find_alloc(graph_t *p_graph) {
    assert(p_graph);

    p_graph->union_find.p_parents = (node_t *) calloc(p_graph->n_nodes, sizeof(node_t));
    if (!p_graph->union_find.p_parents) {
        fprintf(stderr, "%s\n", "graph_union_find_alloc. memory allocation error: cannot allocate parents' array.\n");
        exit(EXIT_FAILURE);
    }

    p_graph->union_find.p_ranks = (size_t *) calloc(p_graph->n_nodes, sizeof(size_t));
    if (!p_graph->union_find.p_ranks) {
        fprintf(stderr, "%s\n", "graph_union_find_alloc. memory allocation error: cannot allocate ranks' array.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * @param p_graph
 * @return
 */
void graph_union_find_init(graph_t *p_graph) {
    assert(p_graph);

    p_graph->union_find.count = bvector_n_trues(p_graph->p_bvector);
    for (node_t n = 1; n < p_graph->n_nodes; n++) {
        p_graph->union_find.p_parents[n] = n;
        p_graph->union_find.p_ranks[n] = 0;
    }
}

/**
 *
 * @param p_graph
 */
static void graph_union_find_release(graph_t *p_graph) {
    assert(p_graph);

    memset(p_graph->union_find.p_parents, 0x0, p_graph->n_nodes * sizeof(node_t));
    free(p_graph->union_find.p_parents);

    memset(p_graph->union_find.p_ranks, 0x0, p_graph->n_nodes * sizeof(size_t));
    free(p_graph->union_find.p_ranks);

    memset(ADDR(p_graph->union_find), 0x0, sizeof(union_find_t));
}

/**
 * Find with recursive compression.
 * @param p_graph
 * @param n
 * @return
 */
node_t graph_union_find_find(graph_t *p_graph, node_t n) {
    assert(p_graph);
    assert(n < p_graph->n_nodes);

    node_t *p_parents = p_graph->union_find.p_parents;
    if (p_parents[n] != p_parents[p_parents[n]]) {
        p_parents[n] = graph_union_find_find(p_graph, p_parents[n]);
    }

    return (p_parents[n]);
}

/**
 *
 * @param p_graph
 * @return
 */
int graph_union_find_done(graph_t *p_graph) {
    assert(p_graph);

    return (p_graph->union_find.count == 1);
}

/**
 *
 * @param p_graph
 * @param n1
 * @param n2
 * @param stage
 * @return
 */
int graph_union_find_union(graph_t *p_graph, node_t n1, node_t n2, int stage) {
    assert(p_graph);
    assert(n1 < p_graph->n_nodes);
    assert(n2 < p_graph->n_nodes);

    if (n1 == n2) {
        /* no merge */
        return (0);
    }

    int n1_root = graph_union_find_find(p_graph, n1);
    int n2_root = graph_union_find_find(p_graph, n2);

    if (n1_root == n2_root) {
        /* no merge */
        return (0);
    }

    if (stage > 0) {
        int black_n1 = bvector_get(p_graph->p_bvector, n1);
        int black_n2 = bvector_get(p_graph->p_bvector, n2);

        if (!black_n1 && !black_n2) {
            probability_t probability = probability_mk((stage - 1) * 0.01);
            if (probability_rand() <= probability) {
                p_graph->union_find.count += 2;
            } else {
                return (0);
            }
        }

        if (black_n1 && !black_n2) {
            probability_t probability = probability_mk(stage * 0.1);
            if (probability_rand() <= probability) {
                p_graph->union_find.count += 1;
            } else {
                return (0);
            }
        }

        if (!black_n1 && black_n2) {
            probability_t probability = probability_mk(stage * 0.1);
            if (probability_rand() <= probability) {
                p_graph->union_find.count += 1;
            } else {
                return (0);
            }
        }
    }

    bvector_set(p_graph->p_bvector, n1);
    bvector_set(p_graph->p_bvector, n2);

    /* merging two parts */
    p_graph->union_find.count--;

    if (p_graph->union_find.p_ranks[n1_root] == p_graph->union_find.p_ranks[n2_root]) {
        p_graph->union_find.p_parents[n2_root] = n1_root;
        p_graph->union_find.p_ranks[n1_root]++;
    } else {
        if (p_graph->union_find.p_ranks[n1_root] < p_graph->union_find.p_ranks[n2_root]) {
            p_graph->union_find.p_parents[n1_root] = n2_root;
        } else {
            /* p_graph->union_find.p_ranks[n1_root] > p_graph->union_find.p_ranks[n2_root] */
            p_graph->union_find.p_parents[n2_root] = n1_root;
        }
    }

    /* merge */
    return (1);
}

static void graph_mk_plateau(graph_t *p_graph) {
    /* make plateaux facility */
    p_graph->plateaux.p_plateaux = (plateau_t *) calloc(p_graph->n_edges, sizeof(plateau_t));
    if (!p_graph->plateaux.p_plateaux) {
        fprintf(stderr, "memory allocation error: could not alloc \"p_graph->plateaux.p_plateaux\" array\n");
        exit(EXIT_FAILURE);
    }

    /* read edges by increasing weight */
    p_graph->plateaux.n_plateaux = 0;
    weight_t current_weight = 0;
    int len = 0;
    for (int i = 0; i < n_edges; i++) {
        edge_t e = p_graph->p_edges_sorted_by_weight[i];
        if (current_weight == e.weight) {
            len++;
        } else {
            if (len > 1) {
                plateau_t plateau;
                plateau.start_idx = i - len + 1;
                plateau.len = len;
                p_graph->plateaux.p_plateaux[p_graph->plateaux.n_plateaux] = plateau;
                p_graph->plateaux.n_plateaux++;
            }
            len = 1;
            current_weight = e.weight;
        }
    }

    /* last plateau (if len > 1) */
    if (len > 1) {
        plateau_t plateau;
        plateau.start_idx = n_edges - len;
        plateau.len = len;
        p_graph->plateaux.p_plateaux[p_graph->plateaux.n_plateaux] = plateau;
        p_graph->plateaux.n_plateaux++;
    }

    /* resize plateau array */
    p_graph->plateaux.p_plateaux = (plateau_t *) realloc(p_graph->plateaux.p_plateaux, n_plateaux * sizeof(plateau_t));
    if (!p_graph->plateaux.p_plateaux) {
        fprintf(stderr, "memory allocation error: could not realloc \"p_graph->plateaux.p_plateaux\" array for %zu plateaux\n", p_graph->plateaux.n_plateaux);
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * @param stream
 * @return
 */
graph_t *graph_read(FILE *stream) {
    char buffer[64];
    int n_nodes;
    int n_edges;

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "SECTION")) {
        fprintf(stderr, "graph_read. parse error: could not read \"SECTION\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "Graph")) {
        fprintf(stderr, "graph_read. parse error: could not read \"Graph\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "Nodes")) {
        fprintf(stderr, "graph_read. parse error: could not read \"Nodes\" token.\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", ADDR(n_nodes)) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of nodes.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "Edges")) {
        fprintf(stderr, "graph_read. parse error: could not read \"Edges\" token.\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", ADDR(n_edges)) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of edges.\n");
        exit(EXIT_FAILURE);
    }

    /* nodes ranges from 1 to n_nodes*/
    ++n_nodes;

    graph_t *p_graph = graph_alloc();
    p_graph->n_nodes = n_nodes;
    p_graph->n_edges = n_edges;
    graph_union_find_alloc(p_graph);

    /* nodes init */
    p_graph->p_bvector = bvector_alloc(n_nodes);
    p_graph->p_terminal_bvector = bvector_alloc(n_nodes);

    p_graph->p_node_counters = (int *) calloc(n_nodes, sizeof(int));
    if (!p_graph->p_node_counters) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_counters\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int n = 0; n < n_nodes; n++) {
        /* init all nods with zero counter */
        p_graph->p_node_counters[n] = 0;
    }

    /* edges init */
    p_graph->p_edges_sorted_by_weight = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!p_graph->p_edges_sorted_by_weight) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges_sorted_by_weight\" array\n");
        exit(EXIT_FAILURE);
    }

    p_graph->p_edges_no_order_guaranteed = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!p_graph->p_edges_no_order_guaranteed) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges_no_order_guaranteed\" array\n");
        exit(EXIT_FAILURE);
    }

    /* Read the edges */
    for (int i = 0; i < n_edges; i++) {
        node_t n1;
        node_t n2;
        weight_t weight;
        if (fscanf(stream, "%*s %d %d %ld", ADDR(n1), ADDR(n2), ADDR(weight)) != 3) {
            fprintf(stderr, "graph_read. parse error: could not read %d-th edge\n.", i);
            exit(EXIT_FAILURE);
        }

        /* store edges with minimum node as n1 */
        edge_t e;
        e.n1 = (n1 < n2) ? n1 : n2;
        e.n2 = (n1 < n2) ? n2 : n1,
                e.weight = weight;

        /* sanity check */
        assert(n1 < n_nodes);
        assert(n2 < n_nodes);

        /* edges_sorted_by_weight */
        p_graph->p_edges_sorted_by_weight[i] = e;

        /* edges_no_order_guaranteed */
        p_graph->p_edges_no_order_guaranteed[i] = e;
    }

    /* order guarantee for edges_sorted_by_weight */
    qsort(p_graph->p_edges_sorted_by_weight, n_edges, sizeof(edge_t), edge_compar_by_weight);

    graph_mk_plateau(p_graph);

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "END")) {
        fprintf(stderr, "graph_read. parse error: could not read \"END\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "SECTION")) {
        fprintf(stderr, "graph_read. parse error: could not read \"SECTION\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "Terminals")) {
        fprintf(stderr, "graph_read. parse error: could not read \"Terminals\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "Terminals")) {
        fprintf(stderr, "graph_read. parse error: could not read \"Terminals\" token.\n");
        exit(EXIT_FAILURE);
    }

    int n_terminal_nodes;
    if (fscanf(stream, "%d", ADDR(n_terminal_nodes)) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of terminal nodes.\n");
        exit(EXIT_FAILURE);
    }

    p_graph->n_terminal_nodes = n_terminal_nodes;
    p_graph->n_non_terminal_nodes = p_graph->n_nodes - n_terminal_nodes;

    /* Read the terminals */
    for (int i = 0; i < p_graph->n_terminal_nodes; i++) {
        node_t n;
        if (fscanf(stream, "%*s %d", ADDR(n)) != 1) {
            fprintf(stderr, "graph_read. parse error: could not read %d-th terminal node\n", i);
            exit(EXIT_FAILURE);
        }
        bvector_set(p_graph->p_terminal_bvector, n);
    }
    fprintf(stdout, "bvector_n_trues(p_graph->p_terminal_bvector)=%zu\n", bvector_n_trues(p_graph->p_terminal_bvector));

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "END") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"END\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "EOF") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"EOF\" token.\n");
        exit(EXIT_FAILURE);
    }

    return (p_graph);
}

/**
 *
 * @return an empty graph.
 */
graph_t *graph_alloc() {
    graph_t *p_graph = (graph_t *) malloc(sizeof(graph_t));
    if (!p_graph) {
        fprintf(stderr, "graph_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    /* nodes init */
    p_graph->n_nodes = 0;
    p_graph->n_terminal_nodes = 0;
    p_graph->n_non_terminal_nodes = 0;
    p_graph->p_bvector = NULL;
    p_graph->p_terminal_bvector = NULL;
    p_graph->p_node_counters = NULL;

    /* edges init */
    p_graph->n_edges = 0;
    p_graph->p_edges_sorted_by_weight = NULL;
    p_graph->p_edges_no_order_guaranteed = NULL;

    /* union find init */
    p_graph->union_find.p_parents = NULL;
    p_graph->union_find.p_ranks = NULL;
    p_graph->union_find.count = 0;

    return (p_graph);
}

/**
 *
 * @param p_graph
 */
void graph_release(graph_t *p_graph) {
    if (p_graph) {
        /* release nodes arrays */
        bvector_release(p_graph->p_bvector);
        bvector_release(p_graph->p_terminal_bvector);

        memset(p_graph->p_node_counters, 0x0, p_graph->n_nodes * sizeof(int));
        free(p_graph->p_node_counters);

        /* release edges arrays */
        assert(p_graph->n_edges == 0 || p_graph->p_edges_sorted_by_weight);
        assert(p_graph->n_edges == 0 || p_graph->p_edges_no_order_guaranteed);

        memset(p_graph->p_edges_sorted_by_weight, 0x0, p_graph->n_edges * sizeof(edge_t));
        free(p_graph->p_edges_sorted_by_weight);

        memset(p_graph->p_edges_no_order_guaranteed, 0x0, p_graph->n_edges * sizeof(edge_t));
        free(p_graph->p_edges_no_order_guaranteed);

        /* release plateaux */
        if (p_graph->plateaux.p_plateaux) {
            memset(p_graph->plateaux.p_plateaux, 0x0, p_graph->plateaux.n_plateaux * sizeof(plateau_t));
            free(p_graph->plateaux.p_plateaux);
        }

        /* release union find facility */
        graph_union_find_release(p_graph);

        /* release the graph itself */
        memset(p_graph, 0x0, sizeof(graph_t));
        free(p_graph);
    }
}

/**
 *
 * @param p_graph
 * @param n
 * @return
 */
int graph_node_is_terminal(graph_t *p_graph, node_t n) {
    return (bvector_get(p_graph->p_terminal_bvector, n));
}

/**
 *
 * @param p_graph
 * @param n
 * @return
 */
int graph_node_is_non_terminal(graph_t *p_graph, node_t n) {
    return (!graph_node_is_terminal(p_graph, n));
}

/**
 *
 * @param p_graph
 * @param counter_val
 */
void graph_node_counter_assert_all(graph_t *p_graph, int counter_val) {
    assert(p_graph);

    for (node_t n = 1; n < p_graph->n_nodes; n++) {
        assert(p_graph->p_node_counters[n] == counter_val);
    }
}

/**
 *
 * @param p_graph
 */
void graph_node_counter_assert_all_zero(graph_t *p_graph) {
    graph_node_counter_assert_all(p_graph, 0);
}

/**
 *
 * @param p_graph
 * @param counter_val
 */
void graph_node_counter_set_all(graph_t *p_graph, int counter_val) {
    assert(p_graph);

    for (node_t n = 1; n < p_graph->n_nodes; n++) {
        p_graph->p_node_counters[n] = counter_val;
    }
}

/**
 *
 * @param p_graph
 */
void graph_node_counter_reset_all(graph_t *p_graph) {
    graph_node_counter_set_all(p_graph, 0);
}

/**
 *
 * @param p_graph
 */
void graph_node_counter_increment_all(graph_t *p_graph) {
    assert(p_graph);

    for (node_t n = 1; n < p_graph->n_nodes; n++) {
        p_graph->p_node_counters[n]++;
    }
}

/**
 *
 * @param p_graph
 * @param n
 * @param counter_val
 */
void graph_node_counter_set(graph_t *p_graph, node_t n, int counter_val) {
    assert(p_graph);
    assert(n < p_graph->n_nodes);

    p_graph->p_node_counters[n] = counter_val;
}

/**
 *
 * @param p_graph
 * @param n
 */
void graph_node_counter_reset(graph_t *p_graph, node_t n) {
    graph_node_counter_set(p_graph, n, 0);
}

/**
 *
 * @param p_graph
 * @param n
 */
void graph_node_counter_increment(graph_t *p_graph, node_t n) {
    assert(p_graph);
    assert(n < p_graph->n_nodes);

    p_graph->p_node_counters[n]++;
}

/**
 *
 * @param p_graph
 * @param n
 * @return
 */
int graph_node_counter_get(graph_t *p_graph, node_t n) {
    assert(p_graph);
    assert(n < p_graph->n_nodes);

    return (p_graph->p_node_counters[n]);
}

/**
 *
 * @param p_graph
 */
void graph_edges_random_shuffle(graph_t *p_graph) {
    assert(p_graph);

    for (int i = p_graph->n_edges - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        if (i != j) {
            edge_t tmp_e = p_graph->p_edges_no_order_guaranteed[i];
            p_graph->p_edges_no_order_guaranteed[i] = p_graph->p_edges_no_order_guaranteed[j];
            p_graph->p_edges_no_order_guaranteed[j] = tmp_e;
        }
    }
}

/**
 *
 * @param p_graph
 */
void graph_edges_random_shuffle_weight_respectful(graph_t *p_graph) {
    assert(p_graph);

    for (int i = 0; i < p_graph->plateaux.n_plateaux; i++) {
        plateau_t plateau = p_graph->plateaux.p_plateaux[i];
        int start_idx = plateau.start_idx;
        size_t len = plateau.len;

        for (int j = start_idx + len - 1; j > start_idx; j--) {
            int k = rand() % (i + 1);
            if (k != j) {
                edge_t tmp_e = p_graph->p_edges_sorted_by_weight[j];
                p_graph->p_edges_sorted_by_weight[j] = p_graph->p_edges_sorted_by_weight[k];
                p_graph->p_edges_sorted_by_weight[k] = tmp_e;
            }
        }

    }
}

/**
 * 
 * @param p_graph
 * @param p_e
 * @return
 *//*
edge_t *graph_search_edge_by_endpoints(graph_t *p_graph, edge_t e) {
    assert(p_graph);

    if (p_graph->n_edges == 0) {
        return (NULL);
    }

    int left = 0;
    int right = p_graph->n_edges - 1;
    while (right >= left) {
        int middle = left + (right - left) / 2;
        edge_t middle_e = p_graph->p_edges_sorted_by_endpoints[middle];

        *//* if the edge is present at the middle itself *//*
        if (middle_e.n1 == e.n1 && middle_e.n2 == e.n2) {
            return (ADDR(p_graph->p_edges_sorted_by_endpoints[middle]));
        } else {
            *//* if the edge is smaller than middle, then it can only be present in left sub-array *//*
            if (e.n1 < middle_e.n1 || (e.n1 == middle_e.n1 && e.n2 < middle_e.n2)) {
                right = middle - 1;
            } else {
                left = middle + 1;
            }
        }
    }

    *//* not found *//*
    return (NULL);
}*/

/**
 *
 * @param p_graph
 * @return
 */
edge_list_t *graph_mst(graph_t *p_graph) {
    assert(p_graph);

    graph_union_find_init(p_graph);

    graph_edges_random_shuffle_weight_respectful(p_graph);

    edge_list_t *p_mst_el = NULL;
    int i = 0;
    do {
        edge_t e = p_graph->p_edges_sorted_by_weight[i];
        node_t n1 = e.n1;
        node_t n2 = e.n2;

        if (bvector_get(p_graph->p_bvector, n1) && bvector_get(p_graph->p_bvector, n2)) {
            node_t root_n1 = graph_union_find_find(p_graph, n1);
            node_t root_n2 = graph_union_find_find(p_graph, n2);

            if (root_n1 != root_n2) {
                /* add an edge in the minimum spanning tree */
                graph_union_find_union(p_graph, n1, n2, 0);
                p_mst_el = edge_list_insert_front(p_mst_el, e);
            }
        }

        i++;
    } while (i < p_graph->n_edges && !graph_union_find_done(p_graph));

    return (p_mst_el);
}
