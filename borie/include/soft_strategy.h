#ifndef _soft__
#define _soft__

#define SIZE_START 10
#define REALLOC_FACTOR 1.5
#define BETTER_CHILD_ATTEMPT 3
#define POP_SIZE 50
#define GOOD_FATHER 20
#define NB_CHILDREN 2

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


typedef struct union_find{
  int* father;
}Union_find;


typedef struct tree{
  int* edge_indices;
  int size;
  int memory_size;
  int weight;
}Tree, *P_tree;


typedef struct pop{
  P_tree solution[POP_SIZE];
}Pop;


void sort_edges_sparse(Graph_sparse* g);
int is_terminal(Graph_sparse* g, int index);
void display_edges_sparse(Graph_sparse* g);
int edges_from_index(Graph_sparse* g, int node, int* first, int* last);
Union_find* allocate_union_find(Graph_sparse* g);
void free_union_find(Union_find* u);
int father_reduce(Union_find* u, int index);
void merge_reduce(Union_find* u, int i1, int i2);
int is_tree_covering(Union_find* u, Graph_sparse* g);
Tree* allocate_tree(void);
void free_tree(Tree* t);
int add_edge_tree(Tree* t, int edge);
Tree* generate_random_tree(Graph_sparse* g);
void update_tree_size(Tree* t, Graph_sparse* g);
void sort_edges_tree(Tree* t);
int is_edge_in_tree(Tree* t, int edge);
Tree* reduced_tree(Graph_sparse* g, Tree* old);
void init_population(Graph_sparse* g, Pop* pop);
void sort_population(Pop* pop);
void next_generation(Graph_sparse* g, Pop* pop);

#endif
