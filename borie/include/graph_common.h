#ifndef _GRAPH_COMMON__
#define _GRAPH_COMMON__

#define START_HEAP_SIZE 10 /* intial size of heaps and arrays */
#define REALLOC_FACTOR 1.3 /* reallocation factor for heaps and arrays */


/* Dense graph : use probably too much memory !!!! */
typedef struct graph_dense{
  int nb_nodes;
  int nb_edges;
  int** edges;
  int** dist;
  int** next;
  int* terminals;
}Graph_dense;


/* An edges is defined with a source index and a destination index */
/* We store edges in both way in order to enable logarithmic search */
typedef struct edge{
  int src;       /* index of the source */
  int dst;       /* index of the destination */
  int weight;    /* weight of this edge */
}Edge;


/* A sparse graph is defined with a number of nodes, a number of edges */
/* number of terminal nodes, a list of edges (stored in both way) */
/* the list of the indices of terminals */
typedef struct graph_sparse{
  int nb_nodes;       /* number of nodes in the graph */
  int nb_edges;       /* number of edges in the graph */
  int nb_terminals;   /* number of terminals nodes in the graph */
  Edge* edges;        /* pointer to array of all edges in both ways */
  int* terminals;     /* pointer to the array of indices of terminals */
}Graph_sparse;


/* A stortest path is defined from an origin : from */
/* To go the target, its thus go to next */
/* The total weight of the path from `from` to the target is weight */
/* The target is a terminal nodes with can be refind by */
/* going along the path : next -> next -> next -> ... -> terminal */
/* Also each terminal node has a variable of type Path associated to it */
typedef struct path{
  int from;      /* Origin of the path */
  int next;      /* next node of the path */
  int weight;    /* total weight of the path */
}Path;


/* For each terminal node, we will have a variable of type Path_terminal */
typedef struct path_to_terminal{
  int terminal;           /* index of the terminal */
  int radius;             /* radius of the topologicial ball of shortest paths */
  int heap_size;          /* the size of the heap of paths in investigation */
  int short_size;         /* the size of the set of shortest path around */
  int heap_memory_size;   /* the max size of memory for the heap */
  int short_memory_size;  /* the list of shortest path going to the terminal */
  Path* path_heap;        /* a pointer to the heap */
  Path* path_short;       /* a pointer to the set of shortest path */
  int* short_set;         /* the ordered set of indices having a shortest path */
}Path_terminal;


/* The array of all topological ball of shortest paths around terminals node */
typedef struct short_paths{
  Path_terminal* paths;   /* array of size nb_terminals */   
}Short_paths;


/* At least, all topological balls must intersect such that they form */
/* a single connex component. */
/* Idealy, intersection are big enought to randomize a growing tree */
/* reaching the terminals */
/* each array have for size the number of nodes */
/* father is the first terminal index having a shortest path going to */
/* father_reduce is used to merge connex component */
/* father_X give other terminal indices reachable from the node */
/* The more we have big intersection (father_X), more we will have */
/* choice during random generation */
typedef struct union_find{
  int* father;         /* the index of the parent node in which the node lives */
  int* father_reduce;  /* the reduced index of the father --> union-find data */
  int* father_2;       /* an alternative terminal reachable from the node */
  int* father_3;       /* another alternative terminal reachable from the node */
  int* father_4;       /* another alternative terminal reachable from the node */
  int* father_5;       /* another alternative terminal reachable from the node */
}Union_find;


void sort_edges_sparse(Graph_sparse* g);
void display_edges_sparse(Graph_sparse* g);
int edges_from_index(Graph_sparse* g, int node, int* first, int* last);
Path* allocate_new_path_heap(void);
int allocate_short_paths(Graph_sparse* g, Short_paths* shorts);
int add_path(Short_paths* shorts, int goal, int from, int next, int weight);
int index_path(Path_terminal* path, int from);
int index_short_path(Path_terminal* path, int from);
void change_father_heap(Path* heap, int index);
int add_path_in_path_heap(Path_terminal* path, int from, int next, int weight);
int add_new_short_path(Path_terminal* paths, int from, int next, int weight);
int allocate_union_find(Graph_sparse* g, Union_find* components);
int find_reduce(Union_find* components, int index);
void union_reduce(Graph_sparse* g, Union_find* components, int src, int dst);
int connected_union_find(Graph_sparse* g, Union_find* components);


#endif
