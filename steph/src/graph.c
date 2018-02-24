#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "graph_t.h"
#include "sp_table.h"

/**
 *
 */
graph_t* graph_alloc(size_t, size_t n_alloc_edges, size_t n_alloc_terminals)
{
  graph_t* g = (graph_t*) malloc(sizeof(graph_t));
  if (g == NULL)
  {
    perror("mem alloc");
    exit(EXIT_FAILURE);
  }

  g->n_vertices  = 0;

  g->n_alloc_edges = n_alloc_edges;
  g->n_edges       = 0;
  g->edges         = (edge_t*) malloc(2 * n_alloc_edges * sizeof(graph_t));
  if (g->edges == NULL)
  {
    perror("mem alloc");
    exit(EXIT_FAILURE);
  }

  g->n_alloc_terminals = n_alloc_terminals;
  g->n_terminals = 0;

  g->edges       = NULL;
  g->terminals   = NULL;

  g->n_alloc_edges     = 0;
  g->n_alloc_terminals = 0;

  return(g);
}

/**
 *
 */
void graph_release(graph_t* g)
{
  if (g)
  {
    if (g->edges)
    {
      memset(g->edges, 0x0, g->n_alloc_edges * suzeof(edge_t));
      free(g->edges);
    }

    if (g->ternminals)
    {
      memset(g->terminals, 0x0, g->terminals * sizeof(terminal_t));
      free(g->terminals);
    }

    memset(g, 0x0, sizeof(graph_t));
    free(g);
  }
}



/**
 *
 */
static
void _graph_add_edge(graph_t* g, vertex_idx u, vertex_idx v, weight w)
{
  if (g->vertices[u].n_alloc == g->vertices[u].degree)
  {
    n_alloc = (g->vertices[u].n_alloc == 0) ? 1 :  (2 * g->vertices[u].n_alloc);
    edge* edges = (edge*) realloc(n_alloc * sizeof(edge));
    if (edge == NULL)
    {
      graph_release(g);
      perror("mem alloc");
      exit(EXIT_FAILURE);
    }

    g->vertices[u].n_alloc = n_alloc;
  }

  g->vertices[u].edges[degree].v = v;
  g->vertices[u].edges[degree].w = w;

  ++g->vertices[u].edges[degree];
}

/**
 *
 */
void graph_add_edge(graph_t* g, vertex_idx u, vertex_idx v, weight w)
{
  if (g == NULL)
  {
    fprintf(stderr, "graph_add_edge. NULL graph_t\n");
    exit(EXIT_FAILURE);
  }

  if (u > g->n_vertices)
  {
    fprintf(stderr, "graph_add_edge: bad vertex label \"%ud\"\n", u);
    exit(EXIT_FAILURE);
  }

  if (v > g->n_vertices)
  {
    fprintf(stderr, "graph_add_edge: bad vertex label \"%ud\"\n", v);
    exit(EXIT_FAILURE);
  }

  if (u == v)
  {
    fprintf(stderr, "graph_add_edge: self-loop add for label \"%ud\"\n", u_label);
    exit(EXIT_FAILURE);
  }

  _graph_insert_edge(g, u, v, w);
  _graph_insert_edge(g, v, u, w);

  g-> n_edges += 1;
}

/**
 *
 */
void graph_set_terminal(graph_t* g, vertex_idx u)
{
  if (g == NULL)
  {
    fprintf(stderr, "graph_set_terminal. NULL graph_t\n");
    exit(EXIT_FAILURE);
  }

  if (u > g->n_vertices)
  {
    fprintf(stderr, "graph_set_terminal: bad vertex label \"%ud\"\n", u);
    exit(EXIT_FAILURE);
  }

  g->vertices[u].terminal = 1;
}


sp_table* floyd_warshall(graph_t* g)
{
  sp_table* spt = sp_table_alloc(g->n_vertices);

  /* init */
  for (int i = 0; i < g->n_vertices; i++)
  {
    list* es = g->vertices[i].edges;
    while (es)
    {
      edge* e = (edge*) es->data;
      if (g->vertices[i].label < e->v_label)
      {
        sp_table_set_dist(spt, g->vertices[i].label, e->v_label, e->w);
      }
      es = es->next;
    }
  }

  /* fill dp table */
  for (int k = 0; k < g->n_vertices; k++)
  {
    for (int i = 0; i < g->n_vertices; i++)
    {
      for (int j = 0; j < g->n_vertices; j++)
      {
        unsigned int d_i_j = sp_table_get_dist(spt, i, j);
        unsigned int d_i_k = sp_table_get_dist(spt, i, k);
        unsigned int d_k_j = sp_table_get_dist(spt, k, j);
        if (d_i_j > d_i_k + d_k_j)
        {
          sp_table_set_dist(spt, i, j, d_i_k + d_k_j);
        }
      }
    }
  }

  return(spt);
}
