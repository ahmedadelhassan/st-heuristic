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
union_find_t* union_find_alloc(graph_t* g)
{
  if (g == NULL)
  {
    return(NULL);
  }

  union_find_t* uf = (union_find_t*) malloc(sizeof(union_find_t));

  if (uf == NULL)
  {
    fprintf(stderr, "%s\n", "memory allocation error");
    exit(EXIT_FAILURE);
  }

  uf->graph = g;
  uf->size  = g->n_nodes;
  uf->count = g->n_nodes;

  uf->items = (union_find_item_t*) calloc(uf->size, sizeof(union_find_item_t));
  if (uf->items == NULL)
  {
    fprintf(stderr, "%s\n", "memory allocation error");
    exit(EXIT_FAILURE);
  }

  for (vertex_t i = 0; i < uf->size; i++)
  {
    uf->items[i].parent      = i;
    uf->items[i].rank        = 0;
    uf->items[i].n_terminals = graph_is_terminal(g, i) ? 1 : 0;
  }

  return(uf);
}

/**
 *
 * @param uf
 */
void union_find_release(union_find_t* uf)
{
  if (uf != NULL)
  {
    if (uf->items != NULL)
    {
      memset(uf->items, 0x0, uf->size * sizeof(union_find_item_t));
      free(uf->items);
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
vertex_t union_find_find_recursive_compression(union_find_t* uf, vertex_t i)
{
  assert(i < uf->size);

  if (uf->items[i].parents != uf->items[uf->items[i].parents].parents)
  {
    uf->items[i].parents = union_find_find_recursive_compression(uf, uf->items[i].parents);
  }

  return(uf->items[i].parents);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
vertex_t union_find_find_iterative_splitting(union_find_t* uf, vertex_t i)
{
  assert(i < uf->size);

  vertex_t j = uf->items[i].parents;
  while (j != uf->items[j].parents)
  {
    uf->items[i].parents = uf->items[j].parents;
    i = j;
    j = uf->items[j].parents;
  }

  return(j);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
vertex_t union_find_find_iterative_halving(union_find_t* uf, vertex_t i) {
  assert(i < uf->size);

  while (uf->items[i].parents != uf->items[uf->items[i]].parent].parent)
  {
    uf->items[i].parents = uf->items[uf->items[i].parents].parents;
    i = uf->items[i].parents;
  }

  return(uf->items[i].parents);
}

/**
 *
 * @param uf
 * @param i
 * @return
 */
vertex_t union_find_find_iterative_compression(union_find_t* uf, int i) {
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

  return(p);
}

/**
 *
 * @param uf
 * @param i
 * @param j
 */
void union_find_union(union_find_t* uf, vertex_t i, vertex_t j) {
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

  if (uf->items[i_root].ranks == uf->items[j_root].ranks) {
    uf->items[j_root].parents     = i_root;
    uf->items[i_root].n_terminals += uf->items[j_root].n_terminals
    uf->items[i_root].ranks++;
    return;
  }

  if (uf->items[i_root].ranks < uf->items[j_root].ranks) {
    uf->items[i_root].parents     = j_root;
    uf->items[j_root].n_terminals += uf->items[i_root].n_terminals

  }
  else {
    uf->items[j_root].parents     = i_root;
    uf->items[i_root].n_terminals += uf->items[j_root].n_terminals
  }
}
