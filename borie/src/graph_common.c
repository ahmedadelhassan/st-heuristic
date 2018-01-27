#include "../include/graph_common.h"

#include <stdio.h>
#include <stdlib.h>


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
  *first = -1;
  *last = -1;

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
  for (middle=0 ; g->edges[*first+middle].src == node; middle++) {}
  *last = *first + middle - 1;
  
  return *last - *first + 1;
}


/* An empty heap */
Path* allocate_new_path_heap(void){
  Path* new;

  new = (Path*)malloc(START_HEAP_SIZE*sizeof(Path));
  if (new == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }
  return new;
}


/* This function has linear complexity... can we do better ? */
int index_path(Path_terminal* path, int from){
  int i;

  for (i=0 ; i<path->heap_size ; i++){
    if (path->path_heap[i].from == from)
      return i;
  }
  return -1;
}


/* This function has linear complexity... can we do better ? */
int index_short_path(Path_terminal* path, int from){
  int i;

  for (i=0 ; i<path->short_size ; i++){
    if (path->path_short[i].from == from)
      return i;
  }
  return -1;
}


/* This time, the search has logarithmic complexity... */
int is_in_short_set(Path_terminal* path, int from){
  int first = 0;
  int last = path->short_size - 1;
  int middle;

  if (path->short_set[0] == from)
    return 1;
  if (path->short_size == 0)
    return 0;
    
  while(last - first > 1){
    middle = (first + last) / 2;
    if (path->short_set[middle] < from)
      first = middle;
    else
      last = middle;
  }
  return path->short_set[last] == from;
}


/* Replace properly path in heap after an insertion at the end... */
void change_father_heap(Path* heap, int index){
  int father;
  int from, next, weight;
  
  if (index > 0){
    father = (index - 1) / 2;
    if (heap[index].weight < heap[father].weight){
      from = heap[index].from;
      next = heap[index].next;
      weight = heap[index].weight;

      heap[index].from = heap[father].from;
      heap[index].next = heap[father].next;
      heap[index].weight = heap[father].weight;

      heap[father].from = from;
      heap[father].next = next;
      heap[father].weight = weight;

      change_father_heap(heap, father);
    }
  }
}


/* Insertion of a path in a path heap tree */
int add_path_in_path_heap(Path_terminal* path, int from, int next, int weight){
  int index;
  void* tmp;
  
  index = index_path(path, from);
  /* We had a path before from this node */
  if (index >= 0){
    /* Is it a shortcut ? */
    if (weight < path->path_heap[index].weight){
      path->path_heap[index].next = next;
      path->path_heap[index].weight = weight; 
    }
    /* Nothing to do if it is not a shortcut */
  }
  /* In this case, we found a new reachable node... */
  else if (!is_in_short_set(path, from)){
    /* Possible reallocation */
    if (path->heap_size == path->heap_memory_size){
      path->heap_memory_size *= REALLOC_FACTOR;
      tmp = realloc(path->path_heap, path->heap_memory_size*sizeof(Path));
      if (tmp == NULL){
	fprintf(stderr, "Memory allocation error\n");
	return 0;
      }
      else{
	path->path_heap = tmp;
      }
    }
    /* Now, insertion at the end of the head thus swapping... */
    path->path_heap[path->heap_size].from = from;
    path->path_heap[path->heap_size].next = next;
    path->path_heap[path->heap_size].weight = weight;

    change_father_heap(path->path_heap, path->heap_size);
    
    /* We did put a new element in the heap */
    path->heap_size += 1;
  }
  return 1;
}


/* Allocation and initialization of the shorstest paths to terminals */
int allocate_short_paths(Graph_sparse* g, Short_paths* shorts){
  int i;
  int k, first, last;
  
  shorts->paths = (Path_terminal*)malloc(g->nb_terminals*sizeof(Path_terminal));
  if (shorts->paths == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }
  for (i=0 ; i<g->nb_terminals ; i++){
    shorts->paths[i].terminal = g->terminals[i];
    shorts->paths[i].radius = 0;
    shorts->paths[i].heap_size = 0;
    shorts->paths[i].short_size = 0;
    shorts->paths[i].heap_memory_size = START_HEAP_SIZE;
    shorts->paths[i].short_memory_size = START_HEAP_SIZE;
    shorts->paths[i].path_heap = allocate_new_path_heap();
    shorts->paths[i].path_short = allocate_new_path_heap();
    shorts->paths[i].short_set = (int*)malloc(START_HEAP_SIZE*sizeof(int));
    if (shorts->paths[i].short_set == NULL){
      fprintf(stderr, "Memory allocation error\n");
      return 0;
    }

    edges_from_index(g, g->terminals[i], &first, &last);
    for (k = first ; k <= last ; k++){
      add_path_in_path_heap(shorts->paths+i,
			    g->edges[k].dst,
			    g->terminals[i],
			    g->edges[k].weight);
    }
  }
  fprintf(stderr, "Allocation/Initialization of topolical balls OK!\n");
  return 1;
}


/* add a shortest path in the list of shortest path */
int add_new_short_path(Path_terminal* paths, int from, int next, int weight){
  void* tmp;
  int i;

  /* Possible reallocation for the list of shortest paths */
  if (paths->short_size == paths->short_memory_size){
    paths->short_memory_size *= REALLOC_FACTOR;
    tmp = realloc(paths->path_short, paths->short_memory_size*sizeof(Path));
    if (tmp == NULL){
      fprintf(stderr, "Memory allocation error\n");
      return 0;
    }
    else{
      paths->path_short = tmp;
    }
    tmp = realloc(paths->short_set, paths->short_memory_size*sizeof(int));
    if (tmp == NULL){
      fprintf(stderr, "Memory allocation error\n");
      return 0;
    }
    else{
      paths->short_set = tmp;
    }
  }
  paths->path_short[paths->short_size].from = from;
  paths->path_short[paths->short_size].next = next;
  paths->path_short[paths->short_size].weight = weight;

  /* We also had the index the set of short path... */
  /* This insertion must be ordered... */
  for (i=paths->short_size ; (i >= 1) && paths->short_set[i-1] > from ; i--){
    paths->short_set[i] = paths->short_set[i-1];
  }
  paths->short_set[i] = from;
  
  paths->short_size += 1;
  return 1;
}


int allocate_union_find(Graph_sparse* g, Union_find* components){
  int i;

  components->father = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (components->father == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }
  components->father_2 = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (components->father_2 == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }
  components->father_3 = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (components->father_3 == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }
  components->father_4 = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (components->father_4 == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }
  components->father_5 = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (components->father_5 == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }
  components->father_reduce = (int*)malloc((g->nb_nodes+1)*sizeof(int));
  if (components->father_reduce == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return 0;
  }

  for (i=0 ; i<=g->nb_nodes ; i++){
    components->father[i] = i;
    components->father_2[i] = 0;
    components->father_3[i] = 0;
    components->father_4[i] = 0;
    components->father_5[i] = 0;
    components->father_reduce[i] = i;
  }

  fprintf(stderr, "Allocation/Initialization of union-find OK!\n");
  return 1;
}


/* Return the representative of the set in which index lives. */
/* Reduce futher searching path during backtrack */
int find_reduce(Union_find* components, int index){
  int father;
  
  if (components->father_reduce[index] == index)
    return index;
  father = find_reduce(components, components->father_reduce[index]);
  components->father_reduce[index] = father;
  return father;
}


/* Merge the two set containing dst and src */
/* Reduce serch paths and harmonize representatives to smaller */
/* Use it when you register a new shortest path form src to dst */
/* src is any node, dst should be a terminal index */
/* Father_X champs are used to register intersection of balls */
/* Up to 5 shortest path to terminals can be registred.. */
/* This allow nice randomization after... */
void union_reduce(Graph_sparse* g, Union_find* components, int src, int dst){
  int father1 = find_reduce(components, src);
  int father2 = find_reduce(components, dst);
  int father = (father1 < father2)?father1:father2;

  components->father_reduce[src] = father;
  components->father_reduce[dst] = father;

  if (components->father[src] == src && (!is_terminal(g, src))){
    components->father[src] = dst;
    return ;
  }
  if (components->father_2[src] == 0){
    components->father_2[src] = dst;
    return ;
  }
  if (components->father_3[src] == 0){
    components->father_3[src] = dst;
    return ;
  }
  if (components->father_4[src] == 0){
    components->father_4[src] = dst;
    return ;
  }
  if (components->father_5[src] == 0){
    components->father_5[src] = dst;
    return ;
  }
}


/* Return 0 if all terminals live in connected balls */
/* Return the smallest index of a non connected terminals otherwise */
int connected_union_find(Graph_sparse* g, Union_find* components){
  int i;
  int godfather = find_reduce(components, g->terminals[0]);

  for (i=1 ; i<g->nb_terminals ; i++){
    if (find_reduce(components, g->terminals[i]) != godfather)
      return g->terminals[i];
  }
  return 0;
}


/* Replace node index in the heap according its children */
/* This is to use after a root suppression to rebalance the heap */
void change_children_heap(Path* heap, int index, int size_heap){
  int from, next, weight;
  int small_child;

  /* No children */
  if (2*index+1 > size_heap-1){
    return ;
  }

  /* Single child */
  if (2*index+1 == size_heap-1){
    if (heap[index].weight > heap[2*index+1].weight){
      from = heap[index].from;
      next = heap[index].next;
      weight = heap[index].weight;

      heap[index].from = heap[2*index+1].from;
      heap[index].next = heap[2*index+1].next;
      heap[index].weight = heap[2*index+1].weight;

      heap[2*index+1].from = from;
      heap[2*index+1].next = next;
      heap[2*index+1].weight = weight;
    }
    return ;
  }

  /* Two children */
  if (heap[2*index+2].weight > heap[2*index+1].weight)
    small_child = 2*index+1;
  else
    small_child = 2*index+2;

  if (heap[index].weight > heap[small_child].weight){
    from = heap[index].from;
    next = heap[index].next;
    weight = heap[index].weight;

    heap[index].from = heap[small_child].from;
    heap[index].next = heap[small_child].next;
    heap[index].weight = heap[small_child].weight;

    heap[small_child].from = from;
    heap[small_child].next = next;
    heap[small_child].weight = weight;

    change_children_heap(heap, small_child, size_heap);    
  }
}


/* Remove the shortest path in a heap tree */
/* This path is thus a shortest one for sure (no more shortcut can exist) */
/* The three pointer from, next, weight are updated properly. */
/* Return 1 if an element if removed, return 0 otherwise. */
int remove_root_in_path_heap(Path_terminal* path,
			     int* from,
			     int* next,
			     int* weight){
  
  if (path->heap_size == 0)
    return 0;
  
  *from = path->path_heap[0].from;
  *next = path->path_heap[0].next;
  *weight = path->path_heap[0].weight;

  path->heap_size--;

  if (path->heap_size > 0){
    path->path_heap[0].from = path->path_heap[path->heap_size].from;
    path->path_heap[0].next = path->path_heap[path->heap_size].next;
    path->path_heap[0].weight = path->path_heap[path->heap_size].weight;

    change_children_heap(path->path_heap, 0, path->heap_size);
    
  }
  return 1;
}


/* First master function */
/* This one try to increasse the topological radius of the ball around */
/* the terminal whose index is `index`. */
/* 1 --> Take the shortest path in the heap associated to terminal index */
/* 2 --> Register this path as a shortest path around the terminal */
/* 3 --> Update the union-find components acording this new shortest path */
/* 4 --> Look at neighboors of the origin of the new shortest path */
/* 5 --> Insert in the heap path new reachable origins if relevant */
/* Moraly, this function send stolons to colonize more nodes around a terminal */
void send_stolon(Graph_sparse* g,
		 Short_paths* shorts,
		 Union_find* components,
		 int index){

  int from, next, weight;
  int terminal = g->terminals[index];
  int first, last, k;
  int new_from, new_weight;

  /* 1 --> Take the shortest path in the heap associated to terminal index */
  if (!remove_root_in_path_heap(shorts->paths+index, &from, &next, &weight))
    return ;

  /* 2 --> Register this path as a shortest path around the terminal */
  add_new_short_path(shorts->paths+index, from, next, weight);

  /* 3 --> Update the union-find components acording this new shortest path */
  union_reduce(g, components, from, terminal);

  /* 4 --> Look at neighboors of the origin of the new shortest path */
  if (edges_from_index(g, from, &first, &last)){
    for (k=first ; k<=last ; k++){
      new_from = g->edges[k].dst;
      new_weight = weight + g->edges[k].weight;
      /* 5 --> Insert in the heap path new reachable origins if relevant */
      add_path_in_path_heap(shorts->paths+index, new_from, from, new_weight);
    }
  }
}


/* Make balls to becomme connexe balanced */
/* TEST FUNCTION TESTING A STRATEGY */
/* The strategy is : Each ball around terminals grown at the same speed */
/* We stop when the union of all balls form something connected */
void merge_all_balanced(Graph_sparse* g, Short_paths* s, Union_find* c){
  int turn = 0;
  int i;

  while (connected_union_find(g, c) != 0){
    for (i=0 ; i<g->nb_terminals ; i++){
      send_stolon(g, s, c, i);      
    }
    turn++;
  }
  fprintf(stderr, "%d turn(s)\n", turn);
}


/* Currently, some realloc provide segfault due to instructions affecting */
/* contextual information of malloc [-1] and [-2] of allocated area... */
/* For debugging, here is a print function */
void print_heap(Path* heap, int size_heap){
  int i;

  for (i=0 ; i<size_heap ; i++){
    fprintf(stderr, "|%d->%d : %d|", heap[i].from, heap[i].next, heap[i].weight);
  }
  fprintf(stderr, "\n");
}
