#include<stdio.h>
#include<stdlib.h>
#include "avc_tree.h"

#define AVL_MAX(x, y) ((x) > (y) ? (x) : (y))

// A utility function to get height of the tree
int avl_height(avl_node* n)
{
    if (n == NULL)
    {
      return(0);
    }
    return(n->height);
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
avl_node* avl_node_mk(avl_key key)
{
    avl_node* n = (avl_node*) malloc(sizeof(avl_node));
    if (avl_node == NULL)
    {
      perror("avl_node_mk");
      exit(EXIT_FAILURE);
    }

    n->key    = key;
    n->left   = NULL;
    n->right  = NULL;
    n->height = 1;

    return(n);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
avl_node* avl_tree_right_rotate(avl_node* n)
{
    avl_node* x = n->left;
    avl_node* y = x->right;

    // Perform rotation
    x->right = n;
    n->left  = y;

    // Update heights
    n->height = AVL_MAX(height(n->left), height(n->right)) + 1;
    x->height = AVL_MAX(height(x->left), height(x->right)) + 1;

    // Return new root
    return(x);
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
avl_node* leftRotate(avl_node* n)
{
    avl_node* x = n->right;
    avl_node* y = x->left;

    // Perform rotation
    x->left  = n;
    n->right = y;

    //  Update heights
    n->height = max(height(n->left), height(n->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return new root
    return(x);
}

// Get Balance factor of node N
int avl_tree_balance(avl_node* n)
{
    if (n == NULL)
    {
      return(0);
    }
    return(height(n->left) - height(n->right));
}

// Recursive function to insert key in subtree rooted
// with node and returns new root of subtree.
struct Node* insert(struct Node* node, int key)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left  = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Equal keys are not allowed in BST
        return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                           height(node->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void avl_tree_preorder(avl_node* n)
{
    if(root != NULL)
    {
        printf("(%d, %d) ", root->key.key1, n->key.key2);
        preOrder(n->left);
        preOrder(n->right);
    }
}

/* Drier program to test above function*/
int main()
{
  struct Node *root = NULL;

  /* Constructing tree given in the above figure */
  root = insert(root, 10);
  root = insert(root, 20);
  root = insert(root, 30);
  root = insert(root, 40);
  root = insert(root, 50);
  root = insert(root, 25);

  /* The constructed AVL Tree would be
            30
           /  \
         20   40
        /  \     \
       10  25    50
  */

  printf("Preorder traversal of the constructed AVL"
         " tree is \n");
  preOrder(root);

  return 0;
}
