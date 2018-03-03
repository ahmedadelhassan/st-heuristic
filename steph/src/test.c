#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

int compar_int(const void* px, const void* py)
{
    int x = *((int*) px);
    int y = *((int*) py);
    return(x - y);
}

avl_tree_t* insert(avl_tree_t* t, int x)
{
    int* px = (int*) malloc(sizeof(int));
    *px = x;
    return avl_tree_insert(t, px, compar_int);
}

int main (int argc, char *argv[])
{
    avl_tree_t* root = NULL;
    printf("empty\n");
    avl_tree_preorder(root);

    root = insert(root, 10);
    printf("insert 10\n");
    avl_tree_preorder(root);
    printf("\n");

    root = insert(root, 20);
    printf("insert 20\n");
    avl_tree_preorder(root);
    printf("\n");

    root = insert(root, 30);
    printf("insert 30\n");
    avl_tree_preorder(root);
    printf("\n");

    root = insert(root, 40);
    printf("insert 40\n");
    avl_tree_preorder(root);
    printf("\n");

    root = insert(root, 50);
    printf("insert 50\n");
    avl_tree_preorder(root);
    printf("\n");

    root = insert(root, 25);
    printf("insert 25\n");
    avl_tree_preorder(root);
    printf("\n");

    return(0);
}
