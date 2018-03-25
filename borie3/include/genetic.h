#ifndef __GENETIC_
#define __GENETIC_

#include "../include/individual.h"

typedef struct population{
  Tree** trees;
  int size;
  int memory_size;
  int nb_try;
  int nb_fail;
}Population;

void init_population(Population* pop, Graph_sparse* g);
void display_info_population(Population* pop);
void sort_population(Population* pop);
void add_random_element(Population* pop, Graph_sparse* g);
void evol_population(Population* pop, Graph_sparse* g);
void remove_doublon(Population* pop, Graph_sparse* g);

#endif
