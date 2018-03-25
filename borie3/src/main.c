/* Macro requirement for getopt */
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../include/load_graph.h"
#include "../include/individual.h"
#include "../include/graph.h"
#include "../include/tree.h"
#include "../include/rand_perm.h"
#include "../include/union_find.h"
#include "../include/touch_reduce.h"
#include "../include/genetic.h"


/* How to use the program in command line. */
void usage(int argc, char* argv[]){
  fprintf(stderr, "Usage: ./%s [-s random_seed] < file_graph.gr > output.ost\n", argv[0]);
}


int main(int argc, char* argv[]){
  int opt;
  int seed = time(NULL);

  while ((opt = getopt(argc, argv, "s:")) != -1){
    switch (opt){
    case 's':
      seed = atoi(optarg);
      break;
    default: /* In case of invalid option */
      fprintf(stderr, "Undefined option\n");
      usage(argc, argv);
      exit(EXIT_FAILURE);
    }
  }

  /* Initilization of the seed */
  srand(seed);

  Graph_sparse g;

  load_graph_sparse(stdin, &g);

  /* All initialisation allocation */
  sort_terminals(&g);
  init_union_find(g.nb_nodes, g.terminals, g.nb_terminals);
  init_perm_n(g.nb_edges);
  init_perm_set(g.nb_edges);
  init_touch_reduce(g.nb_edges);
  init_edge_reduce(g.nb_edges);

  /* Genetic time */
  Population pop;
  int i;
  init_population(&pop, &g);
  while(1){
    for (i=0 ; i<100 ; i++){
      evol_population(&pop, &g);
    }
    sort_population(&pop);
    display_info_population(&pop);
  }
  printf("OK\n");
  
  return 0;
}
