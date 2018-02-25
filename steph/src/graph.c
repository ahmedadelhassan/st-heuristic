#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "graph_t.h"

/**
 *
 * @param stream
 * @return
 */
graph_t* graph_read(FILE* stream)
{
    char     buffer[64];
    int      n_nodes;
    int      n_edges;
    node_t   src;
    node_t   dst;
    weight_t w;

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "SECTION") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Graph") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Nodes") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", &n_nodes) != 1)
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Edges") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", &n_edges) != 1)
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    graph_t* g = graph_alloc();
    g->n_nodes = n_nodes;
    g->n_edges = n_edges;

    /* Memory allocation for edges list */
    g->edges = (edge_t*) calloc(2 * n_edges * sizeof(edge_t));
    if (g->edges == NULL)
    {
        fprintf(stderr, "memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    /* Read the edges */
    for (i = 0 ; i < n_edges ; i++){
        if (fscanf(stream, "%*s %d %d %d", &src, &dst, &x) != 3){
            fprintf(stderr, "parse error\n");
            exit(EXIT_FAILURE);
        }
        g->edges[2*i].src      = src;
        g->edges[2*i].dst      = dst;
        g->edges[2*i].weight   = w;

        g->edges[2*i+1].src    = dst;
        g->edges[2*i+1].dst    = src;
        g->edges[2*i+1].weight = w;
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "END") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "SECTION") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Terminals") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "Terminals") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(stream, "%d", &n_terminals) != 1)
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    g->n_terminals = n_terminals;

    /* Memory allocation for terminal nodes */
    g->terminals = (node_t*) calloc(n_terminals, sizeof(node_t));
    if (g->terminals == NULL){
        fprintf(stderr, "memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    /* Read the terminals */
    for (i = 0 ; i < g->nb_terminals ; i++){
        if (fscanf(stream, "%*s %d", &t) != 1){
            fprintf(stderr, "parse error\n");
            exit(EXIT_FAILURE);
        }
        g->terminals[i] = t;
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "END") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(stream, "%s", buffer) != 1) || (strcmp(buffer, "EOF") != 0))
    {
        fprintf(stderr, "parse error\n");
        exit(EXIT_FAILURE);
    }

    return(g);
}

/**
 *
 * @param n_alloc_edges
 * @param n_alloc_terminals
 * @return
 */
graph_t* graph_alloc()
{
    graph_t* g = (graph_t*) malloc(sizeof(graph_t));
    if (g == NULL)
    {
        perror("mem alloc");
        exit(EXIT_FAILURE);
    }

    g->n_nodes     = 0;
    g->n_edges     = 0;
    g->n_terminals = 0;

    g->edges       = NULL;
    g->terminals   = NULL;

    return(g);
}

/**
 *
 * @param g
 */
void graph_release(graph_t* g)
{
  if (g)
  {
    if (g->edges)
    {
      memset(g->edges, 0x0, 2 * g->n_edges * sizeof(edge_t));
      free(g->edges);
    }

    if (g->ternminals)
    {
      memset(g->terminals, 0x0, 2 * g->terminals * sizeof(terminal_t));
      free(g->terminals);
    }

    memset(g, 0x0, sizeof(graph_t));
    free(g);
  }
}

/**
 *
 * @param pe1
 * @param pe2
 * @return
 */
static int cmp_edge(const void* pe1, const void* pe2)
{
    const edge_t* e1 = pe1;
    const edge_t* e2 = pe2;

    int c = e1->src - e2->src;
    if (c != 0)
    {
        return(c);
    }

    return(e1->dest - e2>dest);
}


/**
 * 
 * @param g
 * @return
 */
list_t* graph_kruskal_min_spanning_tree(graph_t* g)
{
    list_t* l = NULL;
    union_find_t* uf = union_find_alloc(g);
    for (int e = 0; i < g->n_edges; e += 2)
    {
        node_t i = g->nodes[g->edges[e].src];
        node_t j = g->nodes[g->edges[e].dest];

        if ((g->nodes[i].color == BLACK) && (g->nodes[i].color == BLACK))
        {
            vertex_t rep_i = union_find_find_recursive_compression(uf, i);
            vertex_t rep_j = union_find_find_recursive_compression(uf, j);

            if (rep_i != rep_j)
            {
                l = l.insert(g->edges[e]);
                union_find_union(rep_i, rep_j);
            }
        }
    }
    union_find_release(uf);

    return(l);
}