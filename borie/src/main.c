/* Macro requirement for getopt */
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/load_graph.h"

/* How to use the program in command line. */
void usage(int argc, char* argv[]){
  fprintf(stderr, "Usage: ./%s [-s random_seed] file_name\n", argv[0]);
}


int main(int argc, char* argv[]){
  int opt;
  int seed = 0;
  char* path_file_output;

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

  if (optind >= argc) {
    fprintf(stderr, "Expected argument for graph file\n");
    usage(argc, argv);
    exit(EXIT_FAILURE);
  } 
  path_file_output = argv[optind];

  /* Initilization of the seed */
  srand(seed);

  Graph_dense g;
  load_graph_dense(path_file_output, &g);
  
  return 0;
}
