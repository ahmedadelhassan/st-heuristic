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
 * @param stream
 * @return
 */
graph_t *graph_read(FILE *stream) {
    char buffer[64];
    int n_nodes;
    int n_edges;


    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "SECTION") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"SECTION\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Graph") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"Graph\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Nodes") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"Nodes\" token.\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", &n_nodes) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of nodes.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Edges") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"Edges\" token.\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", &n_edges) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of edges.\n");
        exit(EXIT_FAILURE);
    }

    graph_t *g = (graph_t *) malloc(sizeof(graph_t));
    if (g == NULL) {
        fprintf(stderr, "graph_read. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    g->n_nodes = n_nodes;
    g->n_edges = n_edges;

    g->node_colors = (color_t *) calloc(n_nodes, sizeof(color_t));
    if (g->node_colors == NULL) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_colors\" array.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n_nodes; i++) {
        g->node_colors[i] = WHITE;
    }

    g->node_counters = (int *) calloc(n_nodes, sizeof(int));
    if (g->node_counters == NULL) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_counters\" array.\n");
        exit(EXIT_FAILURE);
    }

    g->node_terminals = (int *) calloc(n_nodes, sizeof(color_t));
    if (g->node_terminals == NULL) {
        fprintf(stderr, "graph_read. memory allocation error: could not alloc \"node_terminals\" array.\n");
        exit(EXIT_FAILURE);
    }

    /* Memory allocation for colors */
    g->edges = (edge_t *) calloc(n_nodes, sizeof(edge_t));
    if (g->edges == NULL) {
        fprintf(stderr, "memory allocation error: could not alloc \"edges\" array\n");
        exit(EXIT_FAILURE);
    }

    /* Read the edges */
    for (int i = 0; i < n_edges; i++) {
        node_t src;
        node_t dest;
        weight_t w;
        if (fscanf(stream, "%*s %d %d %d", &src, &dest, &w) != 3) {
            fprintf(stderr, "graph_read. parse error: could not read %d-th edge\n.", i);
            exit(EXIT_FAILURE);
        }
        g->edges[i].src = src;
        g->edges[i].dest = dest;
        g->edges[i].weight = w;
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "END") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"END\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "SECTION") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"SECTION\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Terminals") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"Terminals\" token.\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Terminals") != 0)) {
        fprintf(stderr, "graph_read. parse error: could not read \"Terminals\" token.\n");
        exit(EXIT_FAILURE);
    }

    int n_terminals;
    if (fscanf(stream, "%d", &n_terminals) != 1) {
        fprintf(stderr, "graph_read. parse error: could not read number of terminal nodes.\n");
        exit(EXIT_FAILURE);
    }

    g->n_terminals = n_terminals;
    g->min_terminal_index = n_nodes;

    /* Read the terminals */
    for (int i = 0; i < g->n_terminals; i++) {
        node_t t;
        if (fscanf(stream, "%*s %d", &t) != 1) {
            fprintf(stderr, "graph_read. parse error: could not read %d-th terminal\n", i);
            exit(EXIT_FAILURE);
        }
        g->node_terminals[t] = 1;
        if (g->min_terminal_index > i) {
            g->min_terminal_index = i;
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
 * @param g
 */
void graph_release(graph_t *g) {
    if (g != NULL) {
        if (g->node_colors) {
            memset(g->node_colors, 0X0, g->n_nodes * sizeof(color_t));
            free(g->node_colors);
        }

        if (g->node_counters != NULL) {
            memset(g->node_counters, 0x0, g->n_nodes * sizeof(int));
            free(g->node_counters);
        }

        if (g->node_terminals != NULL) {
            memset(g->node_terminals, 0x0, g->n_nodes * sizeof(int));
            free(g->node_terminals);
        }

        if (g->edges != NULL) {
            memset(g->edges, 0x0, g->n_edges * sizeof(edge_t));
            free(g->edges);
        }

        memset(g, 0x0, sizeof(graph_t));
        free(g);
    }
}

/**
 *
 * @param g
 * @param i
 * @return
 */
int graph_node_is_terminal(graph_t *g, node_t i) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_terminals != NULL));
    assert(i < g->n_nodes);

    return (g->node_terminals[i]);
}

/**
 *
 * @param g
 * @param c
 */
void graph_color_set_all(graph_t *g, color_t c) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_colors != NULL));

    for (int i = 0; i < g->n_nodes; i++) {
        g->node_colors[i] = c;
    }
}

/**
 *
 * @param g
 * @param i
 * @param c
 */
void graph_color_set(graph_t *g, node_t i, color_t c) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_colors != NULL));
    assert(i < g->n_nodes);

    g->node_colors[i] = c;
}

/**
 *
 * @param g
 * @param i
 * @return the color of node i
 */
color_t graph_node_color_get(graph_t *g, node_t i) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_colors != NULL));
    assert(i < g->n_nodes);

    return (g->node_colors[i]);
}

/**
 *
 * @param g
 * @param c
 */
void graph_node_counter_set_all(graph_t *g, int val) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_counters != NULL));

    for (int i = 0; i < g->n_nodes; i++) {
        g->node_counters[i] = val;
    }
}

/**
 *
 * @param g
 * @param i
 * @param c
 */
void graph_counter_set(graph_t *g, node_t i, int val) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_counters != NULL));
    assert(i < g->n_nodes);

    g->node_counters[i] = val;
}

/**
 *
 * @param g
 * @param i
 * @return the color of node i
 */
int graph_counter_get(graph_t *g, node_t i) {
    assert(g != NULL);
    assert((g->n_nodes == 0) || (g->node_counters != NULL));
    assert(i < g->n_nodes);

    return (g->node_counters[i]);
}

/**
 *
 * @param pe1
 * @param pe2
 * @return
 */
static int cmp_edge(const void *pe1, const void *pe2) {
    const edge_t *e1 = pe1;
    const edge_t *e2 = pe2;

    assert(e1 != NULL);
    assert(e2 != NULL);

    int c = e1->src - e2->src;
    if (c != 0) {
        return (c);
    }

    return (e1->dest - e2->dest);
}

/**
 *
 * @param g
 */
void graph_random_shuffle_edges(graph_t *g) {
    assert(g != NULL);

    random_shuffle(g->edges, g->n_edges, sizeof(edge_t));
}

/**
 *
 * @param g
 * @return
 */
list_t *graph_kruskal_min_spanning_tree(graph_t *g) {
    assert(g != NULL);

    union_find_t *uf = union_find_alloc(g);

    list_t *l = NULL;
    for (int i = 0; i < g->n_edges; i++) {
        node_t src = g->edges[i].src;
        node_t dest = g->edges[i].dest;

        if ((g->node_colors[src] == BLACK) && (g->node_colors[dest] == BLACK)) {
            node_t root_src = union_find_find_recursive_compression(uf, src);
            node_t root_dest = union_find_find_recursive_compression(uf, dest);

            if (root_src != root_dest) {
                union_find_union(uf, root_src, root_dest);
                l = list_insert_front(l, &(g->edges[i]));
            }
        }
    }

    union_find_release(uf);

    return (l);
}