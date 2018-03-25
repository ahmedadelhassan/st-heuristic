#ifndef __INDIVIDUAL_
#define __INDIVIDUAL_

#include "../include/graph.h"
#include "../include/tree.h"
#include "../include/rand_perm.h"
#include "../include/union_find.h"
#include "../include/touch_reduce.h"

void update_tree_size(Tree* t, Graph_sparse* g);
Tree* random_individual(Graph_sparse* g);
Tree* merge_tree(Graph_sparse* g, Tree* t1, Tree* t2);

#endif
