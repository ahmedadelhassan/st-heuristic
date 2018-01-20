#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "graph.h"
#include "sp_table.h"

static edge* edge_alloc(unsigned int v_label, unsigned int w)
{
  edge* e = (edge*) malloc(sizeof(edge));
  if (! e)
  {
    perror("mem alloc");
    exit(EXIT_FAILURE);
  }
  e->v_label = v_label;
  e->w       = w;

  return(e);
}

static void edge_release(edge* e)
{
  if (e)
  {
    memset(e, 0x0, sizeof(edge));
    free(e);
  }
}

graph* graph_alloc(size_t n_vertices)
{
  graph* g = (graph*) malloc(sizeof(graph));
  if (! g)
  {
    perror("mem alloc");
    exit(EXIT_FAILURE);
  }

  g->n_vertices = n_vertices;
  g->n_edges    = 0;

  g->vertices = (vertex*) calloc(n_vertices, sizeof(vertex));
  if (! g->vertices)
  {
    graph_release(g);
    perror("mem alloc");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < n_vertices; i++)
  {
    g->vertices[i].label = i;
    g->vertices[i].flag  = 0;
    g->vertices[i].edges = NULL;
  }

  return(g);
}

void graph_release(graph* g)
{
  if (g)
  {
    if (g->vertices)
    {
      for (int i = 0; i < g->n_vertices; i++)
      {
        list* es = g->vertices[i].edges;
        while (es)
        {
          edge_release((edge*) es->data);
          es = es->next;
        }
        list_release_all(g->vertices[i].edges);

        g->vertices[i].label = 0;
        g->vertices[i].flag  = 0;
        g->vertices[i].edges = NULL;
      }
    }
    memset(g, 0x0, sizeof(graph));
    free(g);
  }
}

static void graph_insert_edge(graph* g, unsigned int u_label, unsigned int v_label, unsigned int w)
{
  assert(g->vertices[u_label].label == u_label);

  list* l = list_alloc();
  l->data = (void*) edge_alloc(v_label, w);
  l->next = g->vertices[u_label].edges;
  g->vertices[u_label].edges = l;
}

void graph_add_edge(graph* g, unsigned int u_label, unsigned int v_label, unsigned int w)
{
  if (! g)
  {
    fprintf(stderr, "graph_add_edge. NULL graph\n");
    exit(EXIT_FAILURE);
  }

  if (u_label >= g->n_vertices)
  {
    fprintf(stderr, "graph_add_edge: bad vertex label \"%ud\"\n", u_label);
    exit(EXIT_FAILURE);
  }

  if (v_label >= g->n_vertices)
  {
    fprintf(stderr, "graph_add_edge: bad vertex label \"%ud\"\n", v_label);
    exit(EXIT_FAILURE);
  }

  if (u_label == v_label)
  {
    fprintf(stderr, "graph_add_edge: self-loop add for label \"%ud\"\n", u_label);
    exit(EXIT_FAILURE);
  }

  graph_insert_edge(g, u_label, v_label, w);
  graph_insert_edge(g, v_label, u_label, w);
  g-> n_edges += 1;
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
