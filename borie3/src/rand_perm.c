#include "../include/rand_perm.h"

#include <stdlib.h>
#include <stdio.h>

/* WE HAVE A SINGLE THREAD, SINGLE PROCESSUS PROGRAM  */
/* WE CAN USE A SINGLE PERMUTATION STRUCTURE AT A TIME */
/* BEING A GLOBAL STATIC VARIABLE */
/* WE WILL NEED A SINGLE ALLOCATION FOR THE WHOLE EXECUTION */
static Perm_n Sigma_n;

static Perm_set Sigma_set;


void init_perm_n(int nb_edges){
  int i;
  
  Sigma_n.values = (int*)malloc(nb_edges*sizeof(int));
  if (Sigma_n.values == NULL){
    fprintf(stderr, "Memory Alocation Error\n");
    return ;
  }
  Sigma_n.size = nb_edges;
  Sigma_n.current = 0;
  for (i=0 ; i<nb_edges ; i++){
    Sigma_n.values[i] = i;
  }
}


void reset_perm_n(void){
  Sigma_n.current = 0;
}


int random_perm_n(void){
  int tmp, rand_index;

  rand_index = rand() % (Sigma_n.size - Sigma_n.current) + Sigma_n.current;
  tmp = Sigma_n.values[rand_index];
  Sigma_n.values[rand_index] = Sigma_n.values[Sigma_n.current];
  Sigma_n.values[Sigma_n.current] = tmp;
  Sigma_n.current += 1;
  return tmp;
}


void init_perm_set(int nb_edges){
  Sigma_set.values = (int*)malloc(2*nb_edges*sizeof(int));
  if (Sigma_set.values == NULL){
    fprintf(stderr, "Memory Alocation Error\n");
    return ;
  }
  Sigma_set.size = 0;
  Sigma_set.current = 0;
}


void reset_perm_set(void){
  Sigma_set.current = 0;
  Sigma_set.size = 0;  
}


void add_set_perm_set(int* values, int size){
  int i;

  for (i=0 ; i<size ; i++){
    Sigma_set.values[Sigma_set.size+i] = values[i];
  }
  Sigma_set.size += size;
}


int random_perm_set(void){
  int tmp, rand_index;

  rand_index = rand() % (Sigma_set.size - Sigma_set.current) + Sigma_set.current;
  tmp = Sigma_set.values[rand_index];
  Sigma_set.values[rand_index] = Sigma_set.values[Sigma_set.current];
  Sigma_set.values[Sigma_set.current] = tmp;
  Sigma_set.current += 1;
  return tmp;
}
