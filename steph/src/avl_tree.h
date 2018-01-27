#ifndef __AVL_TREE__
#define __AVL_TREE__

typedef struct avl_key
{
  int key1;
  int key2;
} avl_key;

struct avl_node
{
    avl_key          key;
    struct avl_node* left;
    struct avl_node* right;
    int              height;
} avl_node;



#endif /* __AVL_TREE__ */
