/* Macro requirement for getopt */
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../include/load_graph.h"
/* #include "../include/graph_common.h" */
#include "../include/soft_strategy.h"


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
  /* Short_paths shorts; */
  /* Union_find components; */

  load_graph_sparse(stdin, &g);
  sort_edges_sparse(&g);
  /* display_edges_sparse(&g); */

  /* allocate_short_paths(&g, &shorts); */
  /* allocate_union_find(&g, &components); */
  /* merge_all_balanced(&g, &shorts, &components); */

  Pop pop;
  int i=0;
  
  init_population(&g, &pop);

  while(1){
    fprintf(stderr, "Generation %d :\n", i);
    fprintf(stderr, "Best %ld :\n", pop.solution[0]->weight);    
    next_generation(&g, &pop);
    i++;
  }
  
  fprintf(stderr, "Everything connected : it works!\n");
  
  return 0;
}
