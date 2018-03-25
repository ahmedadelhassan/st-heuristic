#ifndef __TREE_
#define __TREE_

typedef struct tree{
  int* edge_indices;  /* array of selected edges in the tree */
  int size;           /* the current number of edges in the tree */
  int memory_size;    /* the memory size max of array edge_indices */
  long int weight;    /* total weight of the tree */
}Tree, *P_tree;


Tree* allocate_tree(void);
void free_tree(Tree* t);
int add_edge_tree(Tree* t, int edge);
int cmp_tree(const void* a1, const void* a2);
void sort_edge_in_tree(Tree* t);

#endif
