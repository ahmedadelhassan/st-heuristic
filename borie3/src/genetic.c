#include "../include/genetic.h"

#include <stdio.h>
#include <stdlib.h>

#define SIZE_POP_START 300
#define REALLOC_FACTOR 2
#define NB_TRY 5
#define TRY_FACTOR 1
#define TRY_INCR 10

void init_population(Population* pop, Graph_sparse* g){
  int i;
  
  pop->trees = (Tree**)malloc(SIZE_POP_START*sizeof(Tree*));
  if (pop->trees == NULL){
    fprintf(stderr, "Memory Allocation Error\n");
    return ;
  }
  pop->size = SIZE_POP_START;
  pop->memory_size = SIZE_POP_START;
  for (i=0 ; i<SIZE_POP_START ; i++){
    pop->trees[i] = random_individual(g);
  }
  pop->nb_try = NB_TRY;
  pop->nb_fail = 0;
  sort_population(pop);
}


void display_info_population(Population* pop){
  int i;
  long int best = pop->trees[0]->weight;
  long int average = 0;
  
  for (i=pop->size-1 ; i>4 ; i = i-6){
    printf("%10ld %10ld %10ld %10ld %10ld %10ld\n",
	   pop->trees[i]->weight,
	   pop->trees[i-1]->weight,
	   pop->trees[i-2]->weight,
	   pop->trees[i-3]->weight,
	   pop->trees[i-4]->weight,
	   pop->trees[i-5]->weight);
    average += pop->trees[i]->weight +
      pop->trees[i-1]->weight +
      pop->trees[i-2]->weight +
      pop->trees[i-3]->weight +
      pop->trees[i-4]->weight +
      pop->trees[i-5]->weight;
  }
  for ( ; i>=0 ; i--){
    average += pop->trees[i]->weight;
    printf("%10ld ", pop->trees[i]->weight);
  }
  printf("\n");
  printf("%d trees in population\n", pop->size);
  printf("BEST : %10ld  ---  AVERAGE : %10ld\n", best, average/pop->size);
}


void sort_population(Population* pop){
  qsort(pop->trees, pop->size, sizeof(Tree*), cmp_tree);
}


void add_random_element(Population* pop, Graph_sparse* g){
  Tree** tmp;

  if (pop->size == pop->memory_size){
    pop->memory_size *= REALLOC_FACTOR;
    tmp = (Tree**)realloc(pop->trees, pop->memory_size*sizeof(Tree*));
    if (tmp == NULL){
      fprintf(stderr, "Memory allocation error\n");
      return ;
    }
    pop->trees = tmp;
  }
  pop->trees[pop->size] = random_individual(g);
  pop->size += 1;
}


void evol_population(Population* pop, Graph_sparse* g){
  Tree* new;
  int i, j, k;
  
  remove_doublon(pop, g);
  if (pop->nb_fail >= pop->nb_try){
    pop->nb_fail = 0;
    pop->nb_try = pop->nb_try*TRY_FACTOR + TRY_INCR;
    add_random_element(pop, g);
    return ;
  }
  i = rand() % pop->size;
  j = i;
  while (j == i){
    j = rand() % pop->size;
  }
  k = (i<j)?i:j;
  new = merge_tree(g, pop->trees[i], pop->trees[j]);
  if (new->weight < pop->trees[k]->weight){
    free_tree(pop->trees[k]);
    pop->trees[k] = new;
  }
  else{
    pop->nb_fail += 1;
  }
}


void remove_doublon(Population* pop, Graph_sparse* g){
  int i;
  
  sort_population(pop);
  sort_edge_in_tree(pop->trees[0]);
  for (i=0 ; i<pop->size-1 ; i++){
    sort_edge_in_tree(pop->trees[i+1]);
    if (cmp_tree(pop->trees[i], pop->trees[i+1]) == 0){
      free_tree(pop->trees[i+1]);
      pop->trees[i+1] = random_individual(g);
      remove_doublon(pop, g);
    }
  }
}
