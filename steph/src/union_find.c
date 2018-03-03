#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "graph.h"
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

    uf->parent = (node_t *) calloc(g->n_nodes, sizeof(node_t));
    if (uf->parent == NULL) {
        fprintf(stderr, "%s\n", "union_find_alloc. memory allocation error");
        exit(EXIT_FAILURE);
    }

    uf->rank = (size_t *) calloc(g->n_nodes, sizeof(size_t));
    if (uf->rank == NULL) {
        fprintf(stderr, "%s\n", "union_find_alloc. memory allocation error");
        exit(EXIT_FAILURE);
    }

    uf->n
    _terminals = (size_t *) calloc(g->n_nodes, sizeof(size_t));
    if (uf->n_terminals == NULL) {
        fprintf(stderr, "%s\n", "union_find_alloc. memory allocation error");
        exit(EXIT_FAILURE);
    }

    uf->graph = g;
    union_find_init(uf);

    return (uf);
}

/**
 *
 * @param uf
 */
void union_find_init(union_find_t *uf) {
    if (uf != NULL) {

        uf->size = g->n_nodes;
        uf->count = g->n_nodes;

        for (node_t i = 0; i < uf->size; i++) {
            uf->parent[i] = i;
            uf->rank[i] = 0;
            uf->n_terminals[i] = graph_is_terminal(g, i) ? 1 : 0;
        }
    }
}

/**
 *
 * @param uf
 */
void union_find_release(union_find_t *uf) {
    if (uf != NULL) {
        if (uf->items != NULL) {
            memset(uf->parent, 0x0, uf->size * sizeof(node_t));
            free(uf->parent);

            memset(uf->rank, 0x0, uf->size * sizeof(size_t));
            free(uf->rank);

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

    if (uf->parent[i] != uf->items[uf->parent[i]].parents) {
        uf->parent[i] = union_find_find_recursive_compression(uf, uf->parent[i]);
    }

    return (uf->parent[i]);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
node_t union_find_find_iterative_splitting(union_find_t *uf, node_t i) {
    assert(i < uf->size);

    node_t j = uf->parent[i];
    while (j != uf->parent[j]) {
        uf->parent[i] = uf->parent[j];
        i = j;
        j = uf->parent[j];
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

    while (uf->parent[i] != uf->items[uf->items[i]].parent].parent)
    {
        uf->parent[i] = uf->parent[uf->parent[i]];
        i = uf->parent[i];
    }

    return (uf->parent[i]);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
node_t union_find_find_iterative_compression(union_find_t *uf, int i) {
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
#ifdef DEBUG
    printf("union %d %d\n", i, j);
#endif

    if (i == j) {
        return;
    }

    int i_root = union_find_find_iterative_compression(uf, i);
    int j_root = union_find_find_iterative_compression(uf, j);

    if (i_root == j_root) {
        return;
    }

    uf->count--;

    if (uf->rank[i_root] == uf->rank[j_root]) {
        uf->parent[i_roo] = i_root;
        uf->n_terminals[i_root] += uf->n_terminals[j_root]
        uf->rank[i_root]++;
        return (uf->n_terminals[i_root]);
    }

    if (uf->rank[i_root] < uf->rank[j_root]) {
        uf->parent[i_roo] = j_root;
        uf->n_terminals[j_root] += uf->n_terminals[i_root];
        return (uf->n_terminals[j_root]);
    }

    /* uf->rank[i_root] > uf->rank[j_root] */
    uf->parent[i_roo] = i_root;
    uf->n_terminals[i_root] += uf->n_terminals[j_root]
    return (uf->n_terminals[i_root]);
}
