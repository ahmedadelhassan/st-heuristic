#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "../graph.h"
#include "union_find.h"

/**
 *
 * @param g
 * @return
 */
union_find_t *union_find_alloc(graph_t *g) {
    if (g == NULL) {
        return (NULL);
    }

    union_find_t *uf = (union_find_t *) malloc(sizeof(union_find_t));
    if (uf == NULL) {
        fprintf(stderr, "%s\n", "union_find_alloc. memory allocation error");
        exit(EXIT_FAILURE);
    }

    uf->parents = (node_t *) calloc(g->n_nodes, sizeof(node_t));
    if (uf->parents == NULL) {
        fprintf(stderr, "%s\n", "union_find_alloc. memory allocation error");
        exit(EXIT_FAILURE);
    }

    uf->ranks = (size_t *) calloc(g->n_nodes, sizeof(size_t));
    if (uf->ranks == NULL) {
        fprintf(stderr, "%s\n", "union_find_alloc. memory allocation error");
        exit(EXIT_FAILURE);
    }

    uf->n_terminals = (size_t *) calloc(g->n_nodes, sizeof(size_t));
    if (uf->n_terminals == NULL) {
        fprintf(stderr, "%s\n", "union_find_alloc. memory allocation error");
        exit(EXIT_FAILURE);
    }

    uf->graph = g;
    uf->size = g->n_nodes;
    uf->count = g->n_nodes;

    for (node_t i = 0; i < uf->size; i++) {
        uf->parents[i] = i;
        uf->ranks[i] = 0;
        uf->n_terminals[i] = graph_node_is_terminal(g, i) ? 1 : 0;
    }

    return (uf);
}

/**
 *
 * @param uf
 */
void union_find_release(union_find_t *uf) {
    if (uf != NULL) {
        if (uf->parents != NULL) {
            memset(uf->parents, 0x0, uf->size * sizeof(node_t));
            free(uf->parents);
        }

        if (uf->ranks != NULL) {
            memset(uf->ranks, 0x0, uf->size * sizeof(size_t));
            free(uf->ranks);
        }

        if (uf->n_terminals != NULL) {
            memset(uf->n_terminals, 0x0, uf->size * sizeof(size_t));
            free(uf->n_terminals);
        }

        memset(uf, 0x0, sizeof(union_find_t));
        free(uf);
    }
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
node_t union_find_find_recursive_compression(union_find_t *uf, node_t i) {
    assert(i < uf->size);

    if (uf->parents[i] != uf->parents[uf->parents[i]]) {
        uf->parents[i] = union_find_find_recursive_compression(uf, uf->parents[i]);
    }

    return (uf->parents[i]);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
node_t union_find_find_iterative_splitting(union_find_t *uf, node_t i) {
    assert(i < uf->size);

    node_t j = uf->parents[i];
    while (j != uf->parents[j]) {
        uf->parents[i] = uf->parents[j];
        i = j;
        j = uf->parents[j];
    }

    return (j);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
node_t union_find_find_iterative_halving(union_find_t *uf, node_t i) {
    assert(i < uf->size);

    while (uf->parents[i] != uf->parents[uf->parents[i]]) {
        uf->parents[i] = uf->parents[uf->parents[i]];
        i = uf->parents[i];
    }

    return (uf->parents[i]);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
node_t union_find_find_iterative_compression(union_find_t *uf, node_t i) {
    assert(uf != NULL);
    assert((uf->size == 0) || (uf->ranks != NULL));
    assert((uf->parents == 0) || (uf->parents != NULL));
    assert((i >= 0) && (i < uf->size));

    int p = uf->parents[i];
    int j = i;

    /* pass 1 */
    while (p != uf->parents[p]) {
        p = uf->parents[p];
    }

    /* pass 2 */
    while (j != p) {
        uf->parents[i] = p;
        i = j;
        j = uf->parents[i];
    }

    return (p);
}

/**
 *
 * @param uf
 * @param i
 * @param j
 */
size_t union_find_union(union_find_t *uf, node_t i, node_t j) {
    if (i == j) {
        return (uf->n_terminals[union_find_find_iterative_compression(uf, i)]);
    }

    int i_root = union_find_find_iterative_compression(uf, i);
    int j_root = union_find_find_iterative_compression(uf, j);

    if (i_root == j_root) {
        /* i and j are in the same part */
        return (uf->n_terminals[i_root]);
    }

    /* merging two parts */
    uf->count--;

    if (uf->ranks[i_root] == uf->ranks[j_root]) {
        uf->parents[j_root] = i_root;
        uf->n_terminals[i_root] += uf->n_terminals[j_root];
        uf->ranks[i_root]++;
        return (uf->n_terminals[i_root]);
    }

    if (uf->ranks[i_root] < uf->ranks[j_root]) {
        uf->parents[i_root] = j_root;
        uf->n_terminals[j_root] += uf->n_terminals[i_root];
        return (uf->n_terminals[j_root]);
    }

    /* uf->ranks[i_root] > uf->ranks[j_root] */
    uf->parents[i_root] = i_root;
    uf->n_terminals[i_root] += uf->n_terminals[j_root];
    return (uf->n_terminals[i_root]);
}
