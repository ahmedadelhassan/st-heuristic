#ifndef __UNION_FIND_
#define __UNION_FIND_

typedef struct union_find{
  int* father;        /* index of representative of the connected component */
  int* size_cc;       /* number of terminal nodes inside the connected component */
  int nb_nodes;       /* the numbers of nodes in the graph */
  int* terminals;     /* a copy of the address of terminals array */
  int nb_terminals;   /* number of terminals */
}Union_find;


void init_union_find(int nb_nodes, int* terminals, int nb_terminals);
void reset_union_find(void);
int father_reduce(int index);
void merge_reduce(int i1, int i2);
int is_tree_covering(void);

#endif
