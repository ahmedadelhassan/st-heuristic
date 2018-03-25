#include "../include/union_find.h"

#include <stdlib.h>
#include <stdio.h>

/* WE HAVE A SINGLE THREAD, SINGLE PROCESSUS PROGRAM  */
/* WE CAN USE A SINGLE UNION-FIND STRUCTURE AT A TIME */
/* BEING A GLOBAL STATIC VARIABLE */
/* WE WILL NEED A SINGLE ALLOCATION FOR THE WHOLE EXECUTION */
static Union_find UF;


void init_union_find(int nb_nodes, int* terminals, int nb_terminals){
  UF.father = (int*)malloc((nb_nodes + 1)*sizeof(int));
  if (UF.father == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  UF.size_cc = (int*)malloc((nb_nodes + 1)*sizeof(int));
  if (UF.size_cc == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  UF.nb_nodes = nb_nodes;
  UF.terminals = terminals;
  UF.nb_terminals = nb_terminals;
}


void reset_union_find(void){
  int i;

  for (i=0 ; i<=UF.nb_nodes ; i++){
    UF.father[i] = i;
    UF.size_cc[i] = 0;
  }

  for (i=0 ; i<UF.nb_terminals ; i++){
    UF.size_cc[UF.terminals[i]] = 1;
  }
}


int father_reduce(int index){
  int father;
  
  if (UF.father[index] == index)
    return index;
  father = father_reduce(UF.father[index]);
  UF.father[index] = father;
  return father;
}


void merge_reduce(int i1, int i2){
  int f1 = father_reduce(i1);
  int f2 = father_reduce(i2);
  int f = (f1<f2)?f1:f2;

  UF.father[f1] = f;
  UF.father[f2] = f;
  UF.size_cc[f] = UF.size_cc[f1] + UF.size_cc[f2];
}


static int nb_terminal_in_cc(int i){
  return UF.size_cc[father_reduce(i)];
}


int is_tree_covering(void){
  return nb_terminal_in_cc(UF.terminals[0]) == UF.nb_terminals;
}
