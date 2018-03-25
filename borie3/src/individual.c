#include "../include/individual.h"


void update_tree_size(Tree* t, Graph_sparse* g){
  int i;

  t->weight = 0;
  for (i=0 ; i<t->size ; i++){
    t->weight += g->edges[t->edge_indices[i]].weight;
  }
}


Tree* random_individual(Graph_sparse* g){
  Tree* new = allocate_tree();
  int index_edge;
  int src, dst;

  reset_union_find();
  reset_perm_n();
  reset_touch_reduce();
  
  while(!is_tree_covering()){
    index_edge = random_perm_n();
    src = g->edges[index_edge].src;
    dst = g->edges[index_edge].dst;

    if (father_reduce(src) != father_reduce(dst)){
      merge_reduce(src, dst);
      add_edge_tree(new, index_edge);
      add_edge_touch_reduce(src, dst);
    }
  }

  reduce_tree(g, new);
  update_tree_size(new, g);
  sort_edge_in_tree(new);
  return new;
}


Tree* merge_tree(Graph_sparse* g, Tree* t1, Tree* t2){
  Tree* new = allocate_tree();
  int index_edge;
  int src, dst;

  reset_union_find();
  reset_perm_set();
  reset_touch_reduce();

  add_set_perm_set(t1->edge_indices, t1->size);
  add_set_perm_set(t2->edge_indices, t2->size);
  
  while(!is_tree_covering()){
    index_edge = random_perm_set();
    src = g->edges[index_edge].src;
    dst = g->edges[index_edge].dst;

    if (father_reduce(src) != father_reduce(dst)){
      merge_reduce(src, dst);
      add_edge_tree(new, index_edge);
      add_edge_touch_reduce(src, dst);
    }
  }

  reduce_tree(g, new);
  update_tree_size(new, g);
  sort_edge_in_tree(new);
  return new;  
}
