#include "../include/soft_strategy.h"

#include "../include/union_find.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int* Perm=NULL;

typedef struct union_indice{
  int* indices;
  int size;
}Union_indice;

static Union_indice Union;

typedef struct touch_node{
  int* index1;
  int* index2;
  int* touch;
  int* edges_to_remove;
  int nb_edges;
}Touch_node;

static Touch_node Touch;

static int copy = 0;



/* Function comparing two interger */
static int cmp_int(const void* a1, const void* a2){
  const int* i1 = a1;
  const int* i2 = a2;
  return *i1 - *i2;
}


/* Allocate and initialize the global static permutation to the identity */
static void init_perm(Graph_sparse* g){
  int i;
  
  Perm = (int*)malloc(g->nb_edges*sizeof(int));
  if (Perm == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  for (i=0 ; i<g->nb_edges ; i++)
    Perm[i] = i;
}


/* Allocate and initialize the global static permutation to the identity */
static void init_union(Graph_sparse* g){
  Union.indices = (int*)malloc(2*g->nb_edges*sizeof(int));
  if (Union.indices == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  Union.size = 0;
}


/* Generation of a random covering tree */
static Tree* random_tree(Graph_sparse* g){
  Tree* new = allocate_tree();
  int i = 0;
  int tmp, src, dst;
  int rand_mix = -1;

  reset_union_find(g);
  reset_touch(g);

  while(!is_tree_covering(g)){
    rand_mix = (rand()%(g->nb_edges - i)) + i;
    tmp = Perm[i];
    Perm[i] = Perm[rand_mix];
    Perm[rand_mix] = tmp;

    src = g->edges[Perm[i]].src;
    dst = g->edges[Perm[i]].dst;
    if (father_reduce(src) != father_reduce(dst)){
      merge_reduce(src, dst);
      add_edge_tree(new, Perm[i]);
      add_edge_touch(Perm[i], src, dst);
    }
    i++;
  }

  collect_edge_to_remove(g, new);
  reduce_tree(new);
  update_tree_size(new, g);
  return new;
}


/* Mix tree */
static Tree* mix_tree(Graph_sparse* g, Tree* t1, Tree* t2){
  Tree* new = allocate_tree();
  int i;
  int tmp, src, dst;
  int rand_mix = -1;

  reset_union_find(g);
  reset_touch(g);
  
  for (i=0 ; i<t1->size ; i++){
    Union.indices[i] = t1->edge_indices[i];
  }
  for (i=0 ; i<t2->size ; i++){
    Union.indices[t1->size+i] = t2->edge_indices[i];    
  }
  Union.size = t1->size + t2->size;

  i=0;
  while(!is_tree_covering(g)){
    if (i == Union.size){
      free_tree(new);
      return NULL;
    }
    rand_mix = (rand()%(Union.size - i)) + i;
    tmp = Union.indices[i];
    Union.indices[i] = Union.indices[rand_mix];
    Union.indices[rand_mix] = tmp;

    src = g->edges[Union.indices[i]].src;
    dst = g->edges[Union.indices[i]].dst;
    if (father_reduce(src) != father_reduce(dst)){
      merge_reduce(src, dst);
      add_edge_tree(new, Union.indices[i]);
      add_edge_touch(Union.indices[i], src, dst);
    }
    i++;
  }

  collect_edge_to_remove(g, new);
  reduce_tree(new);
  update_tree_size(new, g);
  return new;
}


/* initialize a population */
Pop* init_population(Graph_sparse* g){
  Pop* pop;
  int i;

  pop = (Pop*)malloc(sizeof(Pop));
  if (pop == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  pop->indiv = (P_tree*)malloc(POP_SIZE*sizeof(P_tree));
  if (pop->indiv == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  pop->size = POP_SIZE;
  pop->memory_size = POP_SIZE;
  pop->nb_try = 10;
  pop->nb_fail = 0;
  for (i=0 ; i<POP_SIZE ; i++){
    pop->indiv[i] = random_tree(g);
  }
  
  return pop;
}


/* Some evolution */
static int merge_in_pop(Graph_sparse* g, Pop* pop){
  int i1, i2;
  Tree* new;

  i1 = rand() % pop->size;
  i2 = rand() % pop->size;
  new = mix_tree(g, pop->indiv[i1], pop->indiv[i2]);
  if (new == NULL)
    return 1;
  if (new->weight < pop->indiv[i1]->weight){
    free_tree(pop->indiv[i1]);
    pop->indiv[i1] = new;
    return 0;
  }
  free_tree(new);
  return 1;
}


/* Some information in terminal about a population */
void print_info_pop(Pop* pop){
  int i;
  unsigned long int best=pop->indiv[0]->weight;
  unsigned long int total=best;

  for (i=1 ; i<pop->size ; i++){
    total += pop->indiv[i]->weight;
    if (best > pop->indiv[i]->weight){
      best = pop->indiv[i]->weight;
    }
  }
  

  for (i=0 ; i<pop->size; i++){
    if (!(i%5)){
      printf("\n");
    }
    printf("%10lu ", (pop->indiv[i])->weight);
  }
  printf("\n");
  printf("%d trees\n", pop->size);
  printf("average : %lu ", total/pop->size);
  printf("- best : %lu ", best);
  printf("- (%d fail / %d tries)\n", pop->nb_fail, pop->nb_try);
  printf("%d copy destroyed\n", copy);
}


/* Add edges in tree */
static int add_new_indiv(Graph_sparse* g, Pop* pop){
  P_tree* tmp;
  
  /* Reallocation case */
  if (pop->size == pop->memory_size){
    pop->memory_size *= REALLOC_FACTOR;
    tmp = (P_tree*)realloc(pop->indiv, pop->memory_size*sizeof(P_tree));
    if (tmp == NULL){
      fprintf(stderr, "Reallocation memory error\n");
      return 0;
    }
    pop->indiv = tmp;
  }
  pop->indiv[pop->size] = random_tree(g);
  pop->size++;
  return 1;
}


/* Evolve one time the population */
void evol_population(Graph_sparse* g, Pop* pop){
  sort_pop(pop);
  reduce_pop(pop, g);
  if (pop->nb_fail < pop->nb_try){
    pop->nb_fail += merge_in_pop(g, pop);
  }
  else{
    pop->nb_fail = 0;
    pop->nb_try *= 1.2;
    add_new_indiv(g, pop);
  }
}


/* Sort array of integers */
static void sort_int_array(int* array, int size){
  qsort(array, size, sizeof(int), cmp_int);
}


/* Sort all terminals node */
static void sort_terminals(Graph_sparse* g){
  sort_int_array(g->terminals, g->nb_terminals);
}


/* Return 1 if index is a terminal, return 0 otherwise... */
static int is_terminal(Graph_sparse* g, int index){
  int start = 0;
  int end = g->nb_terminals - 1;
  int middle;

  if (g->terminals[0] == index){
    return 1;
  }
  else{
    while (end - start > 1){
      middle = (start + end) / 2;
      if (g->terminals[middle] < index)
	start = middle;
      else
	end = middle;
    }
  }
  return g->terminals[end] == index;
}


/* Allocate and initialize the global static permutation to the identity */
static void init_touch(Graph_sparse* g){
  Touch.index1 = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (Touch.index1 == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  Touch.index2 = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (Touch.index2 == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }  
  Touch.touch = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (Touch.touch == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
  Touch.edges_to_remove = (int*)malloc((g->nb_edges)*sizeof(int));
    if (Touch.edges_to_remove == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return ;
  }
}


/* reset the touched node in the global static structure */
static void reset_touch(Graph_sparse* g){
  int i;

  for (i=0 ; i<=g->nb_nodes ; i++){
    Touch.touch[i] = 0;
    Touch.index1[i] = -1;
    Touch.index2[i] = -1;    
  }
  Touch.nb_edges = 0;
}


/* Add an edge in the touch records */
static void add_edge_touch(int edge, int src, int dst){
  Touch.touch[src] += 1;
  Touch.touch[dst] += 1;
  
  if (Touch.index1[src] == -1)
    Touch.index1[src] = edge;
  else if (Touch.index2[src] == -1)
    Touch.index2[src] = edge;

  if (Touch.index1[dst] == -1)
    Touch.index1[dst] = edge;
  else if (Touch.index2[dst] == -1)
    Touch.index2[dst] = edge;  
}


/* Remove an edge from the Touch static records and collect it in the
   set of edge to remove */
static void remove_edge_touch(Graph_sparse* g, int edge){
  int src = g->edges[edge].src;
  int dst = g->edges[edge].dst;

  Touch.touch[src] -= 1;
  Touch.touch[dst] -= 1;

  if (Touch.index1[src] == edge){
    Touch.index1[src] = Touch.index2[src];
    Touch.index2[src] = -1;
  }
  else if (Touch.index2[src] == edge){
    Touch.index2[src] = -1;
  }
  
  if (Touch.index1[dst] == edge){
    Touch.index1[dst] = Touch.index2[dst];
    Touch.index2[dst] = -1;
  }
  else if (Touch.index2[dst] == edge){
    Touch.index2[dst] = -1;
  }

  Touch.edges_to_remove[Touch.nb_edges] = edge;
  Touch.nb_edges += 1;
}


/* collect in a set the edges that need to be removed */
static void collect_edge_to_remove(Graph_sparse* g, Tree* t){
  int i;
  int edge, src, dst;

  for (i=0 ; i<t->size ; i++){
    edge = t->edge_indices[i];
    src = g->edges[edge].src;
    dst = g->edges[edge].dst;
    if (((Touch.touch[src] == 1) && (!is_terminal(g, src))) || ((Touch.touch[dst] == 1) && (!is_terminal(g, dst)))){
      remove_edge_touch(g, edge);
    }
  }
}


/* Reduce some of the edges appearing in t non contributing for covering */
static void reduce_tree(Tree* t){
  int i = 0;
  int j = 0;
  int end = t->size - Touch.nb_edges;
  
  sort_int_array(Touch.edges_to_remove, Touch.nb_edges);
  sort_int_array(t->edge_indices, t->size);

  while(i < end){
    if (i+j >= t->size)
      break;
    /* printf("%d/%d -- %d/%d\n", i, end, j, Touch.nb_edges); */
    if (t->edge_indices[i+j] == Touch.edges_to_remove[j]){
      j++;
    }
    else{
      t->edge_indices[i] = t->edge_indices[i+j];
      i++;
    }
  }
  t->size = end;
}


/* Initialise all */
void init_all(Graph_sparse* g){
  init_perm(g);
  init_union(g);
  init_touch(g);
  sort_terminals(g);
  init_union_find(g);
}


/* Compare deeply two tree... */
static int cmp_tree(const void* t1, const void* t2){
  const Tree* a = t1;
  const Tree* b = t2;  
  int c;
  int i;

  if ((c = (a->weight - b->weight)))
    return c;
  if ((c = (a->size - b->size)))
    return c;
  for (i=0 ; i<a->size ; i++){
    if ((c = (a->edge_indices[i] - b->edge_indices[i])))
      return c;    
  }
  return 0;
}


static void sort_pop(Pop* pop){
  qsort(pop->indiv, pop->size, sizeof(P_tree), cmp_tree);
}


static void reduce_pop(Pop* pop, Graph_sparse* g){
  int i;

  sort_int_array(pop->indiv[0]->edge_indices, pop->indiv[0]->size);
  for (i=0 ; i<pop->size-1 ; i++){
    sort_int_array(pop->indiv[i+1]->edge_indices, pop->indiv[i+1]->size);
    if (cmp_tree(pop->indiv[i], pop->indiv[i+1]) == 0){
      copy++;
      free_tree(pop->indiv[i+1]);
      pop->indiv[i+1] = random_tree(g);
    }  
  }
}
