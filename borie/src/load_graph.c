#include "../include/load_graph.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>

/* Load incidence matrix from a file name of type instanceXYZ.ost */
/* Old version : optil.io give another way to use programms */
int load_graph_dense(const char* path_file_output, Graph_dense* g){
  char path_file_graph[PATH_GRAPH_SIZE];
  FILE* in;
  int i, j;
  int nb_nodes;
  int nb_edges;
  int nb_terminals;
  char word[64];
  int src, dst, weigth;
  
  strcpy(path_file_graph, PATH_PREFIX);
  strcat(path_file_graph, path_file_output);
  i = strlen(path_file_graph);
  path_file_graph[i-1] = '\0';
  path_file_graph[i-3] = 'g';
  path_file_graph[i-2] = 'r';
  
  in = fopen(path_file_graph, "r");
  if (in == NULL){
    fprintf(stderr, "Failure opening file %s\n", path_file_graph);
    return 0;
  }

  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "SECTION"));
  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "Graph"));  
  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "Nodes"));
  
  assert(fscanf(in, "%d", &nb_nodes) == 1);
  printf("%d nodes in the graph\n", nb_nodes);

  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "Edges"));

  assert(fscanf(in, "%d", &nb_edges) == 1);
  printf("%d edges in the graph\n", nb_edges);

  /* Memory allocation for edges matrix */
  g->edges = (int**)malloc((nb_nodes+1)*sizeof(int*));
  if (g->edges == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }
  for (i=0 ; i<nb_nodes+1 ; i++){
    g->edges[i] = (int*)malloc((nb_nodes+1)*sizeof(int));
    if (g->edges[i] == NULL){
      fprintf(stderr, "Memory allocation error\n");
      return 0;
    }
    for (j=0 ; j<nb_nodes+1 ; j++){
      g->edges[i][j] = -1;
    }
  }
  
  /* Read the edges */
  for (i=0 ; i<nb_edges ; i++){
    if (fscanf(in, "%*s %d %d %d", &src, &dst, &weigth) != 3){
      fprintf(stderr, "Error in graph file : can't read E src dst weigth\n");
      return 0;
    }
    g->edges[src][dst] = weigth;
    g->edges[dst][src] = weigth;
  }
  
  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "END"));

  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "SECTION"));
  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "Terminals"));
  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "Terminals"));
  
  assert(fscanf(in, "%d", &nb_terminals) == 1);
  printf("%d are terminal nodes\n", nb_terminals);
  
  /* Memory allocation for terminal nodes */
  g->terminals = (int*)malloc((nb_nodes+1)*sizeof(int));
  if (g->terminals == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }  
  for (i=0 ; i<nb_nodes+1 ; i++){
    g->terminals[i] = 0;
  }

  /* Read the terminals */
  for (i=0 ; i<nb_terminals ; i++){
    if (fscanf(in, "%*s %d", &j) != 1){
      fprintf(stderr, "Error in graph file : can't read T index\n");
      return 0;
    }
    g->terminals[j] = 1;
  }

  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "END"));
  assert(fscanf(in, "%s", word) == 1);
  assert(!strcmp(word, "EOF"));

  printf("Reading EOF done... parsing OK!\n");
  fclose(in);
  
  return 1;
}


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
  g->edges = (Edge*)malloc((g->nb_edges*2)*sizeof(Edge));
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
    g->edges[2*i].src = src;
    g->edges[2*i].dst = dst;
    g->edges[2*i].weight = weight;
    
    g->edges[2*i+1].src = dst;
    g->edges[2*i+1].dst = src;
    g->edges[2*i+1].weight = weight;
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
