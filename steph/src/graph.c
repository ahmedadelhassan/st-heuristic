#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "color.h"
#include "graph.h"
#include "list.h"
#include "union_find.h"
#include "random.h"

/**
 *
 * @param p_g
 */
static void graph_union_find_alloc(graph_t *p_g) {
    assert(p_g);

    p_g->union_find.p_parents = (node_t *) calloc(p_g->n_nodes, sizeof(node_t));
    if (!p_g->union_find.p_parents) {
        fprintf(stderr, "%s\n", "graph_union_find_alloc. memory allocation error: cannot allocate parents' array.\n");
        exit(EXIT_FAILURE);
    }

    p_g->union_find.p_ranks = (size_t *) calloc(p_g->n_nodes, sizeof(size_t));
    if (!p_g->union_find.p_ranks) {
        fprintf(stderr, "%s\n", "graph_union_find_alloc. memory allocation error: cannot allocate ranks' array.\n");
        exit(EXIT_FAILURE);
    }

    p_g->union_find.p_n_terminals = (size_t *) calloc(p_g->n_nodes, sizeof(size_t));
    if (!p_g->union_find.p_n_terminals) {
        fprintf(stderr, "%s\n",
                "graph_union_find_alloc. memory allocation error: cannot allocate n_terminals' array.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * @param p_g
 * @return
 */
void graph_union_find_init(graph_t *p_g) {
    assert(g);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_parents);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_ranks);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_n_terminals);

    p_g->union_find.count = p_g->n_nodes;
    p_g->max_n_terminals_in_part = 1;
    for (node_t n = 0; n < p_g->n_nodes; n++) {
        p_g->union_find.p_parents[n] = n;
        p_g->union_find.p_ranks[n] = 0;
        p_g->union_find.p_n_terminals[n] = graph_node_is_terminal(p_g, n) ? 1 : 0;
    }
}

/**
 *
 * @param p_g
 */
static void graph_union_find_release(graph_t *p_g) {
    assert(g);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_parents);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_ranks);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_n_terminals);

    memset(p_g->union_find.p_parents, 0x0, p_g->n_nodes * sizeof(node_t));
    free(p_g->union_find.p_parents);

    memset(p_g->union_find.p_ranks, 0x0, p_g->union_find.n_nodes * sizeof(size_t));
    free(p_g->union_find.p_ranks);

    memset(p_g->union_find.p_n_terminals, 0x0, p_g->union_find.size * sizeof(size_t));
    free(p_g->union_find.p_n_terminals);

    memset(&(p_g->union_find), 0x0, sizeof(union_find_t));
}

/**
 *
 * @param p_g
 * @param n
 * @return
 */
static node_t graph_union_find_find_recursive_compression(graph_t *p_g, node_t n) {
    assert(g);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_parents);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_ranks);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_n_terminals);
    assert(n < p_g->union_find.n_nodes);

    if (p_g->union_find.[n] != p_g->union_find.p_parents[p_g->union_find.p_parents[n]]) {
        p_g->union_find.p_parents[n] = union_find_find_recursive_compression(g, p_g->union_find.p_parents[n]);
    }

    return (p_g->union_find.p_parents[n]);
}


/**
 *
 * @param p_g
 * @param n
 * @return
 */
node_t graph_union_find_find(graph_t *p_g, node_t n) {
    return (graph_union_find_find_recursive_compression(g, n));
}

/**
 *
 * @param p_g
 * @return
 */
int graph_union_find_terminals_are_connected(graph_t *p_g) {
    assert(g);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_parents);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_ranks);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_n_terminals);

    return (p_g->union_find.max_n_terminals_in_part == p_g->n_terminals);
}

/**
 *
 * @param p_g
 * @param n1
 * @param n2
 * @return
 */
int graph_union_find_union(graph_t *p_g, node_t n1, node_t n2) {
    assert(g);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_parents);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_ranks);
    assert(p_g->n_nodes == 0 || p_g->union_find.p_n_terminals);
    assert(n1 < p_g->union_find.n_nodes);
    assert(n2 < p_g->union_find.n_nodes);

    if (n1 == n2) {
        /* no merge */
        return (0);
    }

    int n1_root = union_find_find(g, n1);
    int n2_root = union_find_find(g, n2);

    if (n1_root == n2_root) {
        /* no merge */
        return (0);
    }

    /* merging two parts */
    p_g->union_find.count--;

    if (p_g->union_find.p_ranks[n1_root] == p_g->union_find.p_ranks[n2_root]) {
        p_g->union_find.p_parents[n2_root] = n1_root;
        p_g->union_find.p_n_terminals[n1_root] += p_g->union_find.p_n_terminals[n2_root];
        if (p_g->union_find.p_n_terminals[n1_root] > p_g->union_find.max_n_terminals_in_part) {
            p_g->union_find.max_n_terminals_in_part = p_g->union_find.p_n_terminals[n1_root];
        }
        p_g->union_find.p_ranks[n1_root]++;
    } else {
        if (p_g->union_find.p_ranks[n1_root] < p_g->union_find.p_ranks[n2_root]) {
            p_g->union_find.p_parents[n1_root] = n2_root;
            p_g->union_find.p_n_terminals[n2_root] += p_g->union_find.p_n_terminals[n1_root];
            if (p_g->union_find.p_n_terminals[n2_root] > p_g->union_find.max_n_terminals_in_part) {
                p_g->union_find.max_n_terminals_in_part = p_g->union_find.p_n_terminals[n2_root];
            }
        } else {
            /* p_g->union_find.p_ranks[n1_root] > p_g->union_find.p_ranks[n2_root] */
            p_g->union_find.p_parents[n1_root] = n1_root;
            p_g->union_find.p_n_terminals[n1_root] += p_g->union_find.p_n_terminals[n2_root];
            if (p_g->union_find.p_n_terminals[n1_root] > p_g->union_find.max_n_terminals_in_part) {
                p_g->union_find.max_n_terminals_in_part = p_g->union_find.p_n_terminals[n1_root];
            }
        };
    };

    /* merge */
    return (1);
}


/**
 *
 * @param stream
 * @return
 */
graph_t *p_graph_read(FILE *stream) {
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

    if (fscanf(stream, "%d", &n_nodes) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of nodes.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || strcmp(buffer, "Edges")) {
        fprintf(stderr, "graph_read. parse error: could not read \"Edges\" token.\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", &n_edges) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of edges.\n");
        exit(EXIT_FAILURE);
    }

    graph_t *p_g = graph_alloc();

    /* nodes init */
    p_g->p_node_colors = (color_t *) calloc(n_nodes, sizeof(color_t));
    if (!p_g->p_node_colors) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_colors\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int u = 0; u < n_nodes; u++) {
        p_g->p_node_colors[u] = WHITE;
    }

    p_g->p_node_counters = (int *) calloc(n_nodes, sizeof(int));
    if (!p_g->p_node_counters) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_counters\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int u = 0; u < n_nodes; u++) {
        p_g->p_node_counters[u] = 0;
    }

    p_g->p_node_terminals = (int *) calloc(n_nodes, sizeof(color_t));
    if (p_g->p_node_terminals == NULL) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_terminals\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int u = 0; u < n_nodes; u++) {
        /* init all nodes as non-terminal */
        p_g->p_node_terminals[u] = 0;
    }

    /* edges init */
    p_g->p_edges_sorted_by_endpoints = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!p_g->p_edges_sorted_by_endpoints) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges_sorted_by_endpoints\" array\n");
        exit(EXIT_FAILURE);
    }

    p_g->p_edges_sorted_by_weight = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!p_g->p_edges_sorted_by_weight) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges_sorted_by_weight\" array\n");
        exit(EXIT_FAILURE);
    }

    p_g->p_edges_no_order_guaranteed = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!p_g->p_edges_no_order_guaranteed) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges_no_order_guaranteed\" array\n");
        exit(EXIT_FAILURE);
    }

    /* Read the edges */
    for (int i = 0; i < n_edges; i++) {
        node_t n1;
        node_t n2;
        weight_t w;
        if (fscanf(stream, "%*s %d %d %d", &n1, &n2, &w) != 3) {
            fprintf(stderr, "graph_read. parse error: could not read %d-th edge\n.", i);
            exit(EXIT_FAILURE);
        }

        /* edges_sorted_by_endpoints */
        p_g->p_edges_sorted_by_endpoints[i].n1 = n1;
        p_g->p_edges_sorted_by_endpoints[i].n2 = n2;
        p_g->p_edges_sorted_by_endpoints[i].weight = w;

        /* edges_sorted_by_weight */
        p_g->p_edges_sorted_by_weight[i].n1 = n1;
        p_g->p_edges_sorted_by_weight[i].n2 = n2;
        p_g->p_edges_sorted_by_weight[i].weight = w;

        /* edges_no_order_guaranteed */
        p_g->p_edges_no_order_guaranteed[i].n1 = n1;
        p_g->p_edges_no_order_guaranteed[i].n2 = n2;
        p_g->p_edges_no_order_guaranteed[i].weight = w;
    }

    /* order guarantee for edges_sorted_by_endpoints */
    qsort(p_g->p_edges_sorted_by_endpoints, n_edges, sizeof(edge_t), edge_compar);

    /* order guarantee for edges_sorted_by_weight */
    qsort(p_g->p_edges_sorted_by_endpoints, n_edges, sizeof(edge_t), edge_weight_compar);


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

    int n_terminals;
    if (fscanf(stream, "%d", &n_terminals) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of terminal nodes.\n");
        exit(EXIT_FAILURE);
    }

    p_g->n_terminals = n_terminals;
    p_g->n_non_terminals = p_g->n_nodes - n_terminals;

    /* Read the terminals */
    for (int i = 0; i < p_g->n_terminals; i++) {
        node_t u;
        if (fscanf(stream, "%*s %d", &u) != 1) {
            fprintf(stderr, "graph_read. parse error: could not read %d-th terminal\n", i);
            exit(EXIT_FAILURE);
        }
        p_g->p_node_terminals[u] = 1;
        p_g->terminals[i] = u;
    }

    /**
     * i : current node.
     * j : terminal index.
     * k : non-terminal index.
     */
    for (int i = 0, j = 0, k = 0; i < n_nodes; i++) {
        if (j == n_terminals) {
            /* we are done with terminals, keep on adding non-terminals */
            p_g->p_non_terminals[k] = i;
            k++;
        } else {
            while (p_g->terminals[j] < i) {
                j++;
            }
            if (p_g->terminals[j] != i) {
                /* node i is not a terminal */
                p_g->p_non_terminals[k] = i;
                k++;
            }
        }
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "END") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"END\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "EOF") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"EOF\" token.\n");
        exit(EXIT_FAILURE);
    }

    return (g);
}

/**
 *
 * @return an empty graph.
 */
graph_t *p_graph_alloc() {
    graph_t *p_g = (graph_t *) malloc(sizeof(graph_t));
    if (!p_g) {
        fprintf(stderr, "graph_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    /* nodes init */
    p_g->n_node = 0;
    p_g->n_terminals = 0;
    p_g->terminals = NULL;
    p_g->n_non_terminals = 0;
    p_g->p_non_terminals = NULL;
    p_g->p_node_colors = NULL;
    p_g->p_node_counters = NULL;

    /* edges init */
    p_g->n_edges = 0;
    p_g->p_edges_sorted_by_endpoints = NULL;
    p_g->p_edges_sorted_by_weight = NULL;
    p_g->p_edges_no_order_guaranteed = NULL;

    /* union find init */
    p_g->union_find.p_parents = NULL;
    p_g->union_find.p_ranks = NULL;
    p_g->union_find.p_n_terminals = NULL;
    p_g->union_find.count = 0;
    p_g->union_find.max_n_terminals_in_part = 0;

    return (g);
}

/**
 *
 * @param p_g
 */
void graph_release(graph_t *p_g) {
    if (p_g) {
        /* release nodes arrays */
        assert(p_g->n_nodes == 0 || p_g->p_node_colors);
        assert(p_g->n_nodes == 0 || p_g->p_node_counters);
        assert(p_g->n_nodes == 0 || p_g->p_node_terminals);

        memset(p_g->p_node_colors, 0X0, p_g->n_nodes * sizeof(color_t));
        free(p_g->p_node_colors);

        memset(p_g->p_node_counters, 0x0, p_g->n_nodes * sizeof(int));
        free(p_g->p_node_counters);

        memset(p_g->p_node_terminals, 0x0, p_g->n_nodes * sizeof(int));
        free(p_g->p_node_terminals);

        /* release edges arrays */
        assert(p_g->n_edges == 0 || p_g->p_edges_sorted_by_endpoints);
        assert(p_g->n_edges == 0 || p_g->p_edges_sorted_by_weight);
        assert(p_g->n_edges == 0 || p_g->p_edges_no_order_guaranteed);

        memset(p_g->p_edges_sorted_by_endpoints, 0x0, p_g->n_edges * sizeof(edge_t));
        free(p_g->edges);

        memset(p_g->p_edges_sorted_by_weight, 0x0, p_g->n_edges * sizeof(edge_t));
        free(p_g->edges);

        memset(p_g->p_edges_no_order_guaranteed, 0x0, p_g->n_edges * sizeof(edge_t));
        free(p_g->edges);

        /* release union find facility */
        graph_union_find_release(p_g);

        /* release the graph itself */
        memset(p_g, 0x0, sizeof(graph_t));
        free(p_g);
    }
}

/**
 *
 * @param p_g
 * @param n
 * @return
 */
int graph_node_is_terminal(graph_t *p_g, node_t n) {
    assert(g);
    assert(p_g->p_node_terminals);
    assert(n < p_g->n_nodes);

    return (p_g->p_node_terminals[n]);
}

/**
 *
 * @param p_g
 * @param n
 * @return
 */
int graph_node_is_non_terminal(graph_t *p_g, node_t n) {
    assert(g);
    assert(p_g->p_node_terminals);
    assert(n < p_g->n_nodes);

    return (p_g->p_node_terminals[n] == 0);
}

/**
 *
 * @param p_g
 * @param c
 */
void graph_color_assert_all(graph_t *p_g, color_t c) {
    assert(p_g);

    for (node_t n = 0; n < p_g->n_nodes; n++) {
        assert(p_g->p_node_colors[n] == c);
    }
}

/**
 *
 * @param p_g
 */
void graph_color_assert_all_white(graph_t *p_g) {
    graph_color_assert_all(p_g, WHITE);
}

/**
 *
 * @param p_g
 */
void graph_color_assert_all_black(graph_t *p_g) {
    graph_color_assert_all(p_g, BLACK);
}

/**
 *
 * @param p_g
 * @param c
 */
void graph_color_set_all(graph_t *p_g, color_t c) {
    assert(p_g);

    for (node_t n = 0; n < p_g->n_nodes; n++) {
        if (p_g->p_node_colors[n] != c;
    }
}

/**
 *
 * @param p_g
 * @param n
 * @param c
 */
void graph_color_set(graph_t *p_g, node_t n, color_t c) {
    assert(p_g);
    assert(n < p_g->n_nodes);

    p_g->p_node_colors[n] = c;
}

/**
 *
 * @param p_g
 * @param n
 * @return
 */
color_t graph_node_color_get(graph_t *p_g, node_t n) {
    assert(p_g);
    assert(n < p_g->n_nodes);

    return (p_g->p_node_colors[n]);
}

/**
 *
 * @param p_g
 * @param counter_val
 */
void graph_node_counter_assert_all(graph_t *p_g, int counter_val) {
    assert(p_g);

    for (node_t n = 0; n < p_g->n_nodes; n++) {
        assert(p_g->p_node_counters[n] == counter_val);
    }
}

/**
 *
 * @param p_g
 */
void graph_node_counter_assert_all_zero(graph_t *p_g) {
    graph_node_counter_assert_all(p_g, 0);
}

/**
 *
 * @param p_g
 * @param counter_val
 */
void graph_node_counter_set_all(graph_t *p_g, int counter_val) {
    assert(p_g);

    for (node_t n = 0; n < p_g->n_nodes; n++) {
        p_g->p_node_counters[n] = counter_val;
    }
}

/**
 *
 * @param p_g
 */
void graph_node_counter_reset_all(graph_t *p_g) {
    graph_node_counter_set_all(p_g, 0);
}

/**
 *
 * @param p_g
 */
void graph_node_counter_increment_all(graph_t *p_g) {
    assert(p_g);

    for (node_t n = 0; n < p_g->n_nodes; n++) {
        p_g->p_node_counters[n]++;
    }
}

/**
 *
 * @param p_g
 * @param n
 * @param counter_val
 */
void graph_node_counter_set(graph_t *p_g, node_t n, int counter_val) {
    assert(p_g);
    assert(n < p_g->n_nodes);

    p_g->p_node_counters[n] = counter_val;
}

/**
 *
 * @param p_g
 * @param n
 */
void graph_node_counter_reset(graph_t *p_g, node_t n) {
    graph_node_counter_set(g, n, 0);
}

/**
 *
 * @param p_g
 * @param n
 */
void graph_node_counter_increment(graph_t *p_g, node_t n) {
    assert(p_g);
    assert(n < p_g->n_nodes);

    p_g->p_node_counters[n]++;
}

/**
 *
 * @param p_g
 * @param n
 * @return
 */
int graph_node_counter_get(graph_t *p_g, node_t n) {
    assert(p_g);
    assert(n < p_g->n_nodes);

    return (p_g->p_node_counters[n]);
}

/**
 *
 * @param p_g
 */
void graph_edge_random_shuffle(graph_t *p_g) {
    assert(p_g);

    random_shuffle(p_g->p_edges_no_order_guaranteed, p_g->n_edges, sizeof(edge_t));
}

/**
 *
 *
 * @param p_g
 * @return
 */
list_t *p_graph_kruskal_min_spanning_tree_on_black_nodes(graph_t *p_g) {
    assert(p_g);

    graph_union_find_init(p_g);

    list_t *p_el = NULL;
    for (int i = 0; i < p_g->n_edges; i++) {
        edge_t e = p_g->p_edges_sorted_by_weight[i];
        node_t n1 = e.n1;
        node_t n2 = e.n2;

        if ((p_g->p_node_colors[n1] == BLACK) && (p_g->p_node_colors[n2] == BLACK)) {
            node_t root_n1 = graph_union_find_find(p_g, n1);
            node_t root_n2 = graph_union_find_find(p_g, n2);

            if (root_n1 != root_n2) {
                graph_union_find_union(p_g, root_n1, root_n2);
                edge_t *p_e = graph_search_edge_by_endpoints(p_g, e);
                if (!p_e) {
                    fprintf(stderr, "graph_kruskal_min_spanning_tree_on_black_nodes. cannot find edge (%u, %u, %u)\n", e.n1, e.n2, e.weight);
                    exit(EXIT_FAILURE);
                }
                list_insert_front(p_el, p_e);
            }
        }
    }

    return (p_el);
}

/**
 * 
 * @param p_g
 * @param p_e
 * @return
 */
edge_t *p_graph_search_edge_by_endpoints(graph_t *p_g, edge_t p_e) {
    asert(p_g);

    int start = 0;
    int end = p_g->nb_edges - 1;
    int middle;

    if (p_g->p_edges_sorted_by_endpoints[0] == p_e) {
        return (&(p_g->p_edges_sorted_by_endpoints[0]));
    } else {
        while (end - start > 1) {
            middle = (start + end) / 2;
            if (edge_compar(&(p_g->p_edges_sorted_by_endpoints[middle]), &p_e) < 0) {
                start = middle;
            } else {
                end = middle;
            }
        }
    }

    if (p_g->p_edges_sorted_by_endpoints[end] == p_e) {
        return (&(p_g->p_edges_sorted_by_endpoints[end]));
    } else {
        return (NULL);
    }
}
