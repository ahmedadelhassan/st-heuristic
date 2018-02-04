#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "graph.h"
#include "sp_table.h"

/**
 *
 */
graph* graph_alloc(size_t n_vertices)
{
  graph* g = (graph*) malloc(sizeof(graph));
  if (g == NULL)
  {
    perror("mem alloc");
    exit(EXIT_FAILURE);
  }

  g->n_vertices  = n_vertices;
  g->n_edges     = 0;
  g->n_terminals = 0;

  g->vertices = (vertex*) calloc(n_vertices, sizeof(vertex));
  if (g->vertices == NULL)
  {
    graph_release(g);
    perror("mem alloc");
    exit(EXIT_FAILURE);
  }

  for (int u = 0; u < n_vertices + 1; u++)
  {
    g->vertices[u].label    = u;
    g->vertices[u].color    = WHITE;
    g->vertices[u].terminal = 0;
    g->vertices[u].degree   = 0;
    g->vertices[u].n_alloc  = 0;
    g->vertices[u].edges    = NULL;
  }

  return(g);
}

/**
 *
 */
void graph_release(graph* g)
{
  if (g)
  {
    if (g->vertices)
    {
      for (int u = 0; u < g->n_vertices + 1; u++)
      {
        if (g->vertices[u].edges)
        {
          memset(g->vertices[u].edges, 0x0, g->vertices[u].n_alloc * sizeof(edge));
          free(g->vertices[u].edges);
        }
      }

      memset(g->vertices, 0x0, (g->n_vertices + 1) * sizeof(vertex));
      free(g->vertices);
    }

    memset(g, 0x0, sizeof(graph));
    free(g);
  }
}

/**
 *
 */
void graph_adjust(graph* g)
{
  if (g)
  {
    for (int u = 0; u < g->n_vertices + 1; u++)
    {
      if (g->vertices[u].n_alloc == g->vertices[u].degree)
      {
        edge* edges = (edge*) realloc(g->vertices[u].edges, g->vertices[u].degree * sizeof(edge));
        if (edges == NULL)
        {
          graph_release(g);
          perror("mem alloc");
          exit(EXIT_FAILURE);
        }
        g->vertices[u].edges = edges;
      }
    }
  }
}

/**
 *
 */
static
void _graph_add_edge(graph* g, vertex_idx u, vertex_idx v, weight w)
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
void graph_add_edge(graph* g, vertex_idx u, vertex_idx v, weight w)
{
  if (g == NULL)
  {
    fprintf(stderr, "graph_add_edge. NULL graph\n");
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
void graph_set_terminal(graph* g, vertex_idx u)
{
  if (g == NULL)
  {
    fprintf(stderr, "graph_set_terminal. NULL graph\n");
    exit(EXIT_FAILURE);
  }

  if (u > g->n_vertices)
  {
    fprintf(stderr, "graph_set_terminal: bad vertex label \"%ud\"\n", u);
    exit(EXIT_FAILURE);
  }

  g->vertices[u].terminal = 1;
}


sp_table* floyd_warshall(graph* g)
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
