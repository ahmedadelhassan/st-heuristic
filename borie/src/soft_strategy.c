#include "../include/soft_strategy.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* A lexicographic compare function for the edges... */
static int cmp_edge(const void* e1, const void* e2){
  int c;
  const Edge* a = e1;
  const Edge* b = e2;
  
  if ((c = (a->src - b->src)))
    return c;
  return a->dst - b->dst; 
}


/* compare two integer */
static int cmp_int(const void* e1, const void* e2){
  const int* a = e1;
  const int* b = e2;

  return *a - *b; 
}


/* Sort the edges in lexicographic order... */
/* Sort also the terminals indices just in case... */
void sort_edges_sparse(Graph_sparse* g){
  qsort(g->edges, 2*g->nb_edges, sizeof(Edge), cmp_edge);
  qsort(g->terminals, g->nb_terminals, sizeof(int), cmp_int);
}


/* Return 1 if index is a terminal, return 0 otherwise... */
int is_terminal(Graph_sparse* g, int index){
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


/* This is a test function to visualize the list of edges */
void display_edges_sparse(Graph_sparse* g){
  int i;

  for (i=0 ; i<2*g->nb_edges ; i++){
    fprintf(stderr,
	    "SRC: %5d DST: %5d WEIGTH: %3d\n",
	    g->edges[i].src,
	    g->edges[i].dst,
	    g->edges[i].weight);
  }
}


/* Return the number of edges linked to node index */
/* The two pointer first and last are updated */
/* *first : index of the first edge linked to node */
/* *last : index of the last edge linked to node */
int edges_from_index(Graph_sparse* g, int node, int* first, int* last){
  int start = 0;
  int end = 2*g->nb_edges - 1;
  int middle;

  /* In case the node is isolated... */
  *first = 0;
  *last = 0;

  if (g->edges[0].src == node){
    *first = 0;
  }
  else{
    while (end - start > 1){
      middle = (start + end) / 2;
      if (g->edges[middle].src < node)
	start = middle;
      else
	end = middle;
    }
    *first = start+1;
  }
  for (middle=0 ; g->edges[*first + middle].src == node; middle++) {}
  *last = *first + middle - 1;
  
  return *last - *first + 1;
}


/* Allocate and Initialize new union find instance */
Union_find* allocate_union_find(Graph_sparse* g){
  int i;
  Union_find* new;

  new = (Union_find*)malloc(sizeof(Union_find));
  if (new == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  new->father = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (new->father == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  
  for (i=0 ; i<=g->nb_nodes ; i++){
    new->father[i] = i;
  }
  return new;
}


/* Free union find instance */
void free_union_find(Union_find* u){
  free(u->father);
  free(u);
}


/* Return the representative of the connex component in which index lives. */
int father_reduce(Union_find* u, int index){
  int father;
  
  if (u->father[index] == index)
    return index;
  father = father_reduce(u, u->father[index]);
  u->father[index] = father;
  return father;
}


/* merge the two connex components in which i1 and i1 live. */
void merge_reduce(Union_find* u, int i1, int i2){
  int f1 = father_reduce(u, i1);
  int f2 = father_reduce(u, i2);
  int f = (f1<f2)?f1:f2;

  u->father[f1] = f;
  u->father[f2] = f;
}


/* Return 1 if the union_find is covering the terminals, return 0 otherwise. */
int is_tree_covering(Union_find* u, Graph_sparse* g){
  int godfather = father_reduce(u, g->terminals[0]);
  int i;

  for (i=1 ; i<g->nb_terminals ; i++){
    if (father_reduce(u, g->terminals[i]) != godfather)
      return 0;
  }
  return 1;
}


/* Allocate an empty tree */
Tree* allocate_tree(void){
  Tree* new;

  new = (Tree*)malloc(sizeof(Tree));
  if (new == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  new->edge_indices = (int*)malloc(SIZE_START*sizeof(int));
  if (new->edge_indices == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  new->size = 0;
  new->memory_size = SIZE_START;
  return new;
}


/* Free a tree... */
void free_tree(Tree* t){
  free(t->edge_indices);
  free(t);
}


/* Add edges in tree */
int add_edge_tree(Tree* t, int edge){
  int* tmp;
  
  /* Reallocation case */
  if (t->size == t->memory_size){
    t->memory_size *= REALLOC_FACTOR;
    tmp = realloc(t->edge_indices, t->memory_size*sizeof(int));
    if (tmp == NULL){
      fprintf(stderr, "Reallocation memory error\n");
      return 0;
    }
    t->edge_indices = tmp;
  }
  t->edge_indices[t->size] = edge;
  t->size++;
  return 1;
}


/* Return a random tree */
Tree* generate_random_tree(Graph_sparse* g){
  Tree* t = allocate_tree();
  Union_find* u = allocate_union_find(g);
  int index_alea;
  int src, dst;

  while(!is_tree_covering(u, g)){
    index_alea = rand() % (2*g->nb_edges);

    while(1){
      src = g->edges[index_alea].src;
      dst = g->edges[index_alea].dst;
      if (father_reduce(u, src) != father_reduce(u, dst))
	break;
      index_alea++;
      index_alea = index_alea % (2*g->nb_edges);
    }

    merge_reduce(u, src, dst);
    add_edge_tree(t, index_alea);
  }
  free_union_find(u);
  return t;
}


/* Returns the size of the tree t */
void update_tree_size(Tree* t, Graph_sparse* g){
  int i;
  int weight=0;

  for (i=0 ; i<t->size ; i++){
    weight += g->edges[t->edge_indices[i]].weight;
  }
  t->weight = weight;
}


/* Sort edges in tree */
void sort_edges_tree(Tree* t){
  qsort(t->edge_indices, t->size, sizeof(int), cmp_int);
}


/* Return 1 if the edge is in the tree... */
int is_edge_in_tree(Tree* t, int edge){
  int start = 0;
  int end = t->size - 1;
  int middle;

  if (t->edge_indices[0] == edge){
    return 1;
  }
  else{
    while (end - start > 1){
      middle = (start + end) / 2;
      if (t->edge_indices[middle] < edge)
	start = middle;
      else
	end = middle;
    }
  }
  return t->edge_indices[end] == edge;
}


/* return True if edge or its reverse is in the tree t */
int is_edge_or_reverse_in_tree(Graph_sparse* g, Tree* t, int edge){
  int src, dst, i;
  int min, max;
  
  if (is_edge_in_tree(t, edge))
    return 1;
  src = g->edges[edge].dst;
  dst = g->edges[edge].src;
  edges_from_index(g, src, &min, &max);  
  for (i=min ; i<max ; i++)
    if (g->edges[i].dst == dst)
      break;
  return is_edge_in_tree(t, i);
}


/* Return the index of the reverse edge of index edge */
int reverve_index_of_edge(Graph_sparse* g, int edge){
  int min, max;
  int src, dst;
  int i = -1;
  
  src = g->edges[edge].dst;   /* source of the reverse */
  dst = g->edges[edge].src;   /* destination of the reverse */
  edges_from_index(g, src, &min, &max);
  for (i=min ; i<max ; i++)
    if (g->edges[i].dst == dst)
      break;
  assert(i != -1);
  return i;
}


/* This function search all node in the tree, edge by edge... */
/* return 1 if the comming edge reach a terminal node or if */
/* any child reach a terminal node. Return 0 otherwise */
/* Add the edge in the tree if the edge is needed */
static int reduce_tree_rec(Graph_sparse* g, Tree* old, int current, int father, int edge, Tree* new){
  int min, max;
  int index_rev;
  int need_it = 0;
  int i;
  
  if (edges_from_index(g, current, &min, &max)){
    /* For each edge in the graph linked to current */
    for (i=min ; i<=max ; i++){
      /* if the edge is in the tree */
      if (is_edge_in_tree(old, i) && g->edges[i].dst != father){
        need_it |= reduce_tree_rec(g, old, g->edges[i].dst, current, i, new);
      }
      else{
        index_rev = reverve_index_of_edge(g, i);
        if (is_edge_in_tree(old, index_rev) && g->edges[index_rev].src != father){
          need_it |= reduce_tree_rec(g, old, g->edges[i].dst, current, i, new);
        }
      }
    }
  }
  /* we add the edge is the reduced tree if it is needed */
  if (need_it || is_terminal(g, current)){
    add_edge_tree(new, edge);
    return 1;
  }
  return 0;
}


/* Reduce branch to terminal leafs */
Tree* reduced_tree(Graph_sparse* g, Tree* old){
  Tree* new = allocate_tree();
  int min, max;
  int index_rev;
  int i;
  int begin;

  sort_edges_tree(old);
  
  /* Let us a choice a random terminal as root... */
  begin = g->terminals[rand() % g->nb_terminals];

  edges_from_index(g, begin, &min, &max);
  for (i=min ; i<=max ; i++){
    if (is_edge_in_tree(old, i)){
      reduce_tree_rec(g, old, g->edges[i].dst, begin, i, new);
    }
    else{
      index_rev = reverve_index_of_edge(g, i);
      if (is_edge_in_tree(old, index_rev)){
	reduce_tree_rec(g, old, g->edges[i].dst, begin, i, new);
      }
    }
  }
  free_tree(old);
  sort_edges_tree(new);
  return new;
}


/* Child function */
Tree* merge_tree(Graph_sparse* g, Tree* f1, Tree* f2){
  Tree* child = allocate_tree();
  Union_find* u = allocate_union_find(g);
  Tree* choice = NULL;
  int index_alea;
  int edge_alea;
  int src, dst;

  while(!is_tree_covering(u, g)){
    choice = (rand()%2)?f1:f2;
    index_alea = rand() % (choice->size);
    edge_alea = choice->edge_indices[index_alea];
    
    while(1){
      src = g->edges[edge_alea].src;
      dst = g->edges[edge_alea].dst;
      if (father_reduce(u, src) != father_reduce(u, dst))
	break;
      index_alea++;
      index_alea = index_alea % (choice->size);
      edge_alea = choice->edge_indices[index_alea];      
    }

    merge_reduce(u, src, dst);
    add_edge_tree(child, edge_alea);
  }
  free_union_find(u);
  return reduced_tree(g, child);
}


/* Compute child */
Tree* make_good_child(Graph_sparse* g, Tree* f1, Tree* f2){
  Tree* good = NULL;
  int i = 0;

  while(i < BETTER_CHILD_ATTEMPT){
    good = merge_tree(g, f1, f2);
    update_tree_size(good, g);
    if ((good->weight < f1->weight) && (good->weight < f2->weight)){
      return good;
    }
    free_tree(good);
    i++;
  }
  return NULL;
}


/* Initialiatise population */
void init_population(Graph_sparse* g, Pop* pop){
  int i;
  Tree* new;

  for (i=0 ; i<POP_SIZE ; i++){
    new = generate_random_tree(g);
    pop->solution[i] = reduced_tree(g, new);
    update_tree_size(pop->solution[i], g);
    fprintf(stderr, "new --> %d\n", pop->solution[i]->weight);
  }
  sort_population(pop);
}


/* to compare two tree */
/* static int cmp_tree(const void* e1, const void* e2){ */
/*   P_tree a = (P_tree)e1; */
/*   P_tree b = (P_tree)e2; */

/*   return a->weight - b->weight;  */
/* } */


/* To sort a population */
void sort_population(Pop* pop){
  Tree* tmp;
  int i;
  int j;

  for (i=0; i<POP_SIZE-1 ; i++){
    for (j=i; j<POP_SIZE-1 ; j++){
      if ((pop->solution[j])->weight > (pop->solution[j+1])->weight){
	tmp = pop->solution[j];
	pop->solution[j] = pop->solution[j+1];
	pop->solution[j+1] = tmp; 
      }	
    }
  }
  /* qsort(pop->solution, POP_SIZE, sizeof(P_tree), cmp_tree); */
}


/*  */
void next_generation(Graph_sparse* g, Pop* pop){
  int i, j;
  int k = 0;
  Pop next;
  Tree* new;
  int other;
  int copy;

  for (i=0 ; i<GOOD_FATHER ; i++){
    copy = 0;
    for (j=0 ; j<NB_CHILDREN ; j++){
      other = (rand() % (POP_SIZE - (i+1))) + i + 1;
      new = make_good_child(g, pop->solution[i], pop->solution[other]);
      if (new != NULL){
	next.solution[k] = new;
	k++;
      }
      else if (copy == 0){
	next.solution[k] = pop->solution[i];
	copy = 1;
	k++;
      }
      if ((copy == 0) && (j == NB_CHILDREN-1))
	free_tree(pop->solution[i]);
    }
  }
  for ( ; k<POP_SIZE ; k++){
    new = generate_random_tree(g);
    next.solution[k] = reduced_tree(g, new);
    update_tree_size(next.solution[k], g);    
  }
  for (i=GOOD_FATHER ; i<POP_SIZE ; i++)
    free_tree(pop->solution[i]);
  for (i=0 ; i<POP_SIZE ; i++)
    pop->solution[i] = next.solution[i];
 
  sort_population(pop);
}
