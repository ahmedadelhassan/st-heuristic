#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "color.h"
#include "graph.h"
#include "list.h"
#include "old/union_find.h"
#include "random.h"

/**
 *
 */
#define GRAPH_UNION_FIND_FIND(g, u) (graph_union_find_find_recursive_compression(g, u))
/* #define GRAPH_UNION_FIND_FIND(g, u) (graph_union_find_find_iterative_splitting(g, u))   */
/* #define GRAPH_UNION_FIND_FIND(g, u) (graph_union_find_find_iterative_halving(g, u))     */
/* #define GRAPH_UNION_FIND_FIND(g, u) (graph_union_find_find_iterative_compression(g, u)) */

/**
 *
 * @param g
 */
static void graph_union_find_alloc(graph_t *g) {
    assert(g);

    g->union_find.parents = (node_t *) calloc(g->n_nodes, sizeof(node_t));
    if (!g->union_find.parents) {
        fprintf(stderr, "%s\n", "graph_union_find_alloc. memory allocation error: cannot allocate parents' array.\n");
        exit(EXIT_FAILURE);
    }

    g->union_find.ranks = (size_t *) calloc(g->n_nodes, sizeof(size_t));
    if (!g->union_find.ranks) {
        fprintf(stderr, "%s\n", "graph_union_find_alloc. memory allocation error: cannot allocate ranks' array.\n");
        exit(EXIT_FAILURE);
    }

    g->union_find.n_terminals = (size_t *) calloc(g->n_nodes, sizeof(size_t));
    if (!g->union_find.n_terminals) {
        fprintf(stderr, "%s\n",
                "graph_union_find_alloc. memory allocation error: cannot allocate n_terminals' array.\n");
        exit(EXIT_FAILURE);
    }


}

/**
 *
 * @param g
 * @return
 */
void graph_union_find_init(graph_t *g) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);

    g->union_find.count = g->n_nodes;
    g->max_n_terminals_in_part = 1;
    for (node_t u = 0; u < g->n_nodes; u++) {
        g->union_find.parents[u] = u;
        g->union_find.ranks[u] = 0;
        g->union_find.n_terminals[u] = graph_node_is_terminal(g, u) ? 1 : 0;
    }
}

/**
 *
 * @param g
 */
static void graph_union_find_release(graph_t *g) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);

    memset(g->union_find.parents, 0x0, g->n_nodes * sizeof(node_t));
    free(g->union_find.parents);

    memset(g->union_find.ranks, 0x0, g->union_find.n_nodes * sizeof(size_t));
    free(g->union_find.ranks);

    memset(g->union_find.n_terminals, 0x0, g->union_find.size * sizeof(size_t));
    free(g->union_find.n_terminals);

    memset(&(g->union_find), 0x0, sizeof(union_find_t));
}

/**
 *
 * @param g
 * @param u
 * @return
 */
static node_t graph_union_find_find_recursive_compression(graph_t *g, node_t u) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);
    assert(u < g->union_find.n_nodes);

    if (g->union_find.[u] != g->union_find.parents[g->union_find.parents[u]]) {
        g->union_find.parents[u] = union_find_find_recursive_compression(g, g->union_find.parents[u]);
    }

    return (g->union_find.parents[u]);
}

/**
 *
 * @param g
 * @param u
 * @return
 */
static node_t graph_union_find_find_iterative_splitting(graph_t *g, node_t u) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);
    assert(u < g->union_find.n_nodes);

    node_t v = g->union_find.parents[u];
    while (v != g->union_find.parents[v]) {
        g->union_find.parents[u] = g->union_find.parents[v];
        u = v;
        v = g->union_find.parents[v];
    }

    return (v);
}

/**
 *
 * @param g
 * @param u
 * @return
 */
static node_t graph_union_find_find_iterative_halving(graph_t *g, node_t u) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);
    assert(u < g->union_find.n_nodes);

    while (g->union_find.parents[u] != g->union_find.parents[g->union_find.parents[u]]) {
        g->union_find.parents[u] = g->union_find.parents[g->union_find.parents[u]];
        i = g->union_find.parents[u];
    }

    return (g->union_find.parents[u]);
}

/**
 *
 * @param g
 * @param u
 * @return
 */
static node_t graph_union_find_find_iterative_compression(graph_t *g, node_t u) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);
    assert(u < g->union_find.n_nodes);

    node_t p = g->union_find.parents[u];
    node_t v = u;

    /* pass 1 */
    while (p != g->union_find.parents[p]) {
        p = g->union_find.parents[p];
    }

    /* pass 2 */
    while (v != p) {
        g->union_find.parents[u] = p;
        u = v;
        v = g->union_find.parents[u];
    }

    return (p);
}

/**
 *
 * @param g
 * @param u
 * @return
 */
node_t graph_union_find_find(graph_t *g, node_t u) {
    return (GRAPH_UNION_FIND_FIND(g, u));
}

/**
 *
 * @param g
 * @return
 */
int graph_union_find_terminals_are_connected(graph_t *g) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);

    return (g->union_find.max_n_terminals_in_part == g->n_terminals);
}

/**
 *
 * @param uf
 * @param i
 * @param j
 */
int graph_union_find_union(graph_t *g, node_t u, node_t v) {
    assert(g);
    assert(g->n_nodes == 0 || g->union_find.parents);
    assert(g->n_nodes == 0 || g->union_find.ranks);
    assert(g->n_nodes == 0 || g->union_find.n_terminals);
    assert(u < g->union_find.n_nodes);
    assert(v < g->union_find.n_nodes);

    if (u == v) {
        /* no merge */
        return (0);
    }

    int u_root = union_find_find(g, u);
    int v_root = union_find_find(g, v);

    if (u_root == v_root) {
        /* no merge */
        return (0);
    }

    /* merging two parts */
    g->union_find.count--;

    if (g->union_find.ranks[u_root] == g->union_find.ranks[v_root]) {
        g->union_find.parents[v_root] = u_root;
        g->union_find.n_terminals[u_root] += g->union_find.n_terminals[v_root];
        if (g->union_find.n_terminals[u_root] > g->union_find.max_n_terminals_in_part) {
            g->union_find.max_n_terminals_in_part = g->union_find.n_terminals[u_root];
        }
        g->union_find.ranks[u_root]++;
    } else {
        if (g->union_find.ranks[u_root] < g->union_find.ranks[v_root]) {
            g->union_find.parents[u_root] = v_root;
            g->union_find.n_terminals[v_root] += g->union_find.n_terminals[u_root];
            if (g->union_find.n_terminals[v_root] > g->union_find.max_n_terminals_in_part) {
                g->union_find.max_n_terminals_in_part = g->union_find.n_terminals[v_root];
            }
        } else {
            /* g->union_find.ranks[u_root] > g->union_find.ranks[v_root] */
            g->union_find.parents[u_root] = u_root;
            g->union_find.n_terminals[u_root] += g->union_find.n_terminals[v_root];
            if (g->union_find.n_terminals[u_root] > g->union_find.max_n_terminals_in_part) {
                g->union_find.max_n_terminals_in_part = g->union_find.n_terminals[u_root];
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

    graph_t *g = graph_alloc();

    /* nodes init */
    g->node_colors = (color_t *) calloc(n_nodes, sizeof(color_t));
    if (!g->node_colors) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_colors\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int u = 0; u < n_nodes; u++) {
        g->node_colors[u] = WHITE;
    }

    g->node_counters = (int *) calloc(n_nodes, sizeof(int));
    if (!g->node_counters) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_counters\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int u = 0; u < n_nodes; u++) {
        g->node_counters[u] = 0;
    }

    g->node_terminals = (int *) calloc(n_nodes, sizeof(color_t));
    if (g->node_terminals == NULL) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_terminals\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int u = 0; u < n_nodes; u++) {
        /* init all nodes as non-terminal */
        g->node_terminals[u] = 0;
    }

    /* edges init */
    g->edges_sorted_by_endpoints = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!g->edges_sorted_by_endpoints) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges_sorted_by_endpoints\" array\n");
        exit(EXIT_FAILURE);
    }

    g->edges_sorted_by_weight = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!g->edges_sorted_by_weight) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges_sorted_by_weight\" array\n");
        exit(EXIT_FAILURE);
    }

    g->edges_no_order_guaranteed = (edge_t *) calloc(n_edges, sizeof(edge_t));
    if (!g->edges_no_order_guaranteed) {
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
        g->edges_sorted_by_endpoints[i].n1 = src;
        g->edges_sorted_by_endpoints[i].n2 = dest;
        g->edges_sorted_by_endpoints[i].weight = w;

        /* edges_sorted_by_weight */
        g->edges_sorted_by_weight[i].n1 = src;
        g->edges_sorted_by_weight[i].n2 = dest;
        g->edges_sorted_by_weight[i].weight = w;

        /* edges_no_order_guaranteed */
        g->edges_no_order_guaranteed[i].n1 = src;
        g->edges_no_order_guaranteed[i].n2 = dest;
        g->edges_no_order_guaranteed[i].weight = w;
    }

    /* order guarantee for edges_sorted_by_endpoints */
    qsort(g->edges_sorted_by_endpoints, n_edges, sizeof(edge_t), edge_compar);

    /* order guarantee for edges_sorted_by_weight */
    qsort(g->edges_sorted_by_endpoints, n_edges, sizeof(edge_t), edge_weight_compar);


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

    g->n_terminals = n_terminals;
    g->n_non_terminals = g->n_nodes - n_terminals;

    /* Read the terminals */
    for (int i = 0; i < g->n_terminals; i++) {
        node_t u;
        if (fscanf(stream, "%*s %d", &u) != 1) {
            fprintf(stderr, "graph_read. parse error: could not read %d-th terminal\n", i);
            exit(EXIT_FAILURE);
        }
        g->node_terminals[u] = 1;
        g->terminals[i] = u;
    }

    /**
     * i : current node.
     * j : terminal index.
     * k : non-terminal index.
     */
    for (int i = 0, j = 0, k = 0; i < n_nodes; i++) {
        if (j == n_terminals) {
            /* we are done with terminals, keep on adding non-terminals */
            g->non_terminals[k] = i;
            k++;
        } else {
            while (g->terminals[j] < i) {
                j++;
            }
            if (g->terminals[j] != i) {
                /* node i is not a terminal */
                g->non_terminals[k] = i;
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
graph_t *graph_alloc() {
    graph_t *g = (graph_t *) malloc(sizeof(graph_t));
    if (!g) {
        fprintf(stderr, "graph_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    /* nodes init */
    g->n_node = 0;
    g->n_terminals = 0;
    g->terminals = NULL;
    g->n_non_terminals = 0;
    g->non_terminals = NULL;
    g->node_colors = NULL;
    g->node_counters = NULL;

    /* edges init */
    g->n_edges = 0;
    g->edges_sorted_by_endpoints = NULL;
    g->edges_sorted_by_weight = NULL;
    g->edges_no_order_guaranteed = NULL;

    /* union find init */
    g->union_find.parents = NULL;
    g->union_find.ranks = NULL;
    g->union_find.n_terminals = NULL;
    g->union_find.count = 0;
    g->union_find.max_n_terminals_in_part = 0;

    return (g);
}

/**
 *
 * @param g
 */
void graph_release(graph_t *g) {
    if (g) {
        /* release nodes arrays */
        assert(g->n_nodes == 0 || g->node_colors);
        assert(g->n_nodes == 0 || g->node_counters);
        assert(g->n_nodes == 0 || g->node_terminals);

        memset(g->node_colors, 0X0, g->n_nodes * sizeof(color_t));
        free(g->node_colors);

        memset(g->node_counters, 0x0, g->n_nodes * sizeof(int));
        free(g->node_counters);

        memset(g->node_terminals, 0x0, g->n_nodes * sizeof(int));
        free(g->node_terminals);

        /* release edges arrays */
        assert(g->n_edges == 0 || g->edges_sorted_by_endpoints);
        assert(g->n_edges == 0 || g->edges_sorted_by_weight);
        assert(g->n_edges == 0 || g->edges_no_order_guaranteed);

        memset(g->edges_sorted_by_endpoints, 0x0, g->n_edges * sizeof(edge_t));
        free(g->edges);

        memset(g->edges_sorted_by_weight, 0x0, g->n_edges * sizeof(edge_t));
        free(g->edges);

        memset(g->edges_no_order_guaranteed, 0x0, g->n_edges * sizeof(edge_t));
        free(g->edges);

        /* release union find facility */
        graph_union_find_release(g);

        /* release the graph itself */
        memset(g, 0x0, sizeof(graph_t));
        free(g);
    }
}

/**
 *
 * @param g
 * @param u
 * @return
 */
int graph_node_is_terminal(graph_t *g, node_t u) {
    assert(g);
    assert(g->node_terminals);
    assert(u < g->n_nodes);

    return (g->node_terminals[u]);
}

/**
 *
 * @param g
 * @param i
 * @return
 */
int graph_node_is_non_terminal(graph_t *g, node_t u) {
    assert(g);
    assert(g->node_terminals);
    assert(u < g->n_nodes);

    return (g->node_terminals[u] == 0);
}

/**
 *
 * @param g
 * @param c
 */
void graph_color_set_all(graph_t *g, color_t c) {
    assert(g != NULL);

    for (node_t u = 0; u < g->n_nodes; u++) {
        g->node_colors[u] = c;
    }
}

/**
 *
 * @param g
 * @param u
 * @param c
 */
void graph_color_set(graph_t *g, node_t u, color_t c) {
    assert(g != NULL);
    assert(i < g->n_nodes);

    g->node_colors[u] = c;
}

/**
 *
 * @param g
 * @param u
 * @return the color of node i
 */
color_t graph_node_color_get(graph_t *g, node_t u) {
    assert(g != NULL);
    assert(u < g->n_nodes);

    return (g->node_colors[u]);
}

/**
 *
 * @param g
 * @param c
 */
void graph_node_counter_set_all(graph_t *g, int counter) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_counters != NULL));

    for (node_t u = 0; u < g->n_nodes; u++) {
        g->node_counters[u] = counter;
    }
}

/**
 *
 * @param g
 */
void graph_node_counter_reset_all(graph_t *g) {
    graph_node_counter_set_all(g, 0);
}

/**
 *
 * @param g
 */
void graph_node_counter_increment_all(graph_t *g) {
    assert(g != NULL);

    for (node_t u = 0; u < g->n_nodes; u++) {
        g->node_counters[u]++;
    }
}

/**
 *
 * @param g
 * @param u
 * @param c
 */
void graph_node_counter_set(graph_t *g, node_t u, int counter) {
    assert(g != NULL);
    assert(u < g->n_nodes);

    g->node_counters[u] = counter;
}

/**
 *
 * @param g
 * @param u
 */
void graph_node_counter_reset(graph_t *g, node_t u) {
    graph_node_counter_set(g, u, 0);
}

/**
 *
 * @param g
 * @param u
 */
void graph_node_counter_increment(graph_t *g, node_t u) {
    assert(g != NULL);
    assert(u < g->n_nodes);

    g->node_counters[u]++;
}

/**
 *
 * @param g
 * @param u
 * @return the counter associated to node u
 */
int graph_node_counter_get(graph_t *g, node_t u) {
    assert(g != NULL);
    assert(i < g->n_nodes);

    return (g->node_counters[u]);
}

/**
 *
 * @param g
 */
void graph_edge_random_shuffle(graph_t *g) {
    assert(g != NULL);

    random_shuffle(g->edges, g->n_edges, sizeof(edge_t));
}

/**
 *
 *
 * @param g
 * @return
 */
list_t *graph_kruskal_min_spanning_tree(graph_t *g) {
    assert(g != NULL);

    graph_union_find_init(g);

    list_t *l = NULL;
    for (int i = 0; i < g->n_edges; i++) {
        node_t n1 = g->edges_sorted_by_weight[i].n1;
        node_t n2 = g->edges_sorted_by_weight[i].n2;

        if ((g->node_colors[src] == BLACK) && (g->node_colors[dest] == BLACK)) {
            node_t root_n1 = graph_union_find_find(g, n1);
            node_t root_n2 = graph_union_find_find(g, n2);

            if (root_n1 != root_n2) {
                graph_union_find_union(g, root_n1, root_n2);
                l = list_insert_front(l, &(g->edges_sorted_by_weight[i]));
            }
        }
    }

    return (l);
}

ion_find_union(union_find_t
*uf,
node_t i, node_t
j);