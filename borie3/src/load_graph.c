#include "../include/graph.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/* Parse to build a sparse graph */
int load_graph_sparse(FILE* stream, Graph_sparse* g){
  char word[64];
  int i, j, src, dst, weight;
  
  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "SECTION"));
  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "Graph"));  
  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "Nodes"));
  
  assert(fscanf(stream, "%d", &(g->nb_nodes)) == 1);
  fprintf(stderr, "%d nodes in the graph\n", g->nb_nodes);

  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "Edges"));

  assert(fscanf(stream, "%d", &(g->nb_edges)) == 1);
  fprintf(stderr, "%d edges in the graph\n", g->nb_edges);

  /* Memory allocation for edges list */
  g->edges = (Edge*)malloc((g->nb_edges)*sizeof(Edge));
  if (g->edges == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }

  /* Read the edges */
  for (i=0 ; i<g->nb_edges ; i++){
    if (fscanf(stream, "%*s %d %d %d", &src, &dst, &weight) != 3){
      fprintf(stderr, "Error in graph file : can't read E src dst weigth\n");
      return 0;
    }
    g->edges[i].src = src;
    g->edges[i].dst = dst;
    g->edges[i].weight = weight;
  }

  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "END"));

  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "SECTION"));
  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "Terminals"));
  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "Terminals"));
  
  assert(fscanf(stream, "%d", &(g->nb_terminals)) == 1);
  fprintf(stderr, "%d are terminal nodes\n", g->nb_terminals);

  /* Memory allocation for terminal nodes */
  g->terminals = (int*)malloc((g->nb_terminals)*sizeof(int));
  if (g->terminals == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }

  /* Read the terminals */
  for (i=0 ; i<g->nb_terminals ; i++){
    if (fscanf(stream, "%*s %d", &j) != 1){
      fprintf(stderr, "Error in graph file : can't read T index\n");
      return 0;
    }
    g->terminals[i] = j;
  }

  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "END"));
  assert(fscanf(stream, "%s", word) == 1);
  assert(!strcmp(word, "EOF"));

  fprintf(stderr, "Reading EOF done... parsing OK!\n");
  
  return 1;
}
