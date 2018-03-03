#ifndef __AVL_TREE__
#define __AVL_TREE__


typedef struct avl_tree_t
{
    void*              key;
    struct avl_tree_t* left;
    struct avl_tree_t* right;
    size_t             height;
} avl_tree_t;


extern avl_tree_t* avl_tree_alloc(void* key);
extern void        avl_tree_release(avl_tree_t* t);
extern avl_tree_t* avl_tree_insert(avl_tree_t* t, void* key, int (*compar)(const void*, const void*));
extern size_t      avl_tree_height(avl_tree_t* t);
extern int         avl_tree_lookup(avl_tree_t* t, void* key, int (*compar)(const void*, const void*));

#endif /* __AVL_TREE__ */
