#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl_tree.h"

#define AVL_MAX(x, y) ((x) > (y) ? (x) : (y))


/**
 *
 * @param key
 * @return
 */
avl_tree_t* avl_tree_alloc(void* key)
{
    avl_tree_t* t = (avl_tree_t*) malloc(sizeof(avl_tree_t));
    if (t == NULL)
    {
      fprintf(stderr, "avl_tree_alloc. memory allocation error\n");
      exit(EXIT_FAILURE);
    }

    t->key    = key;
    t->left   = NULL;
    t->right  = NULL;
    t->height = 1;

    return(t);
}

/**
 *
 * @param t
 */
void avl_tree_release(avl_tree_t* t)
{
    if (t != NULL)
    {
        avl_tree_release(t->left);
        avl_tree_release(t->right);
        memset(t, 0x0, sizeof(avl_tree_t));
        free(t);
    }
}

/**
 *
 * @param n
 * @return
 */
static avl_tree_t* avl_tree_right_rotate(avl_tree_t* t)
{
    avl_tree_t* x = t->left;
    avl_tree_t* y = x->right;

    /* perform rotation */
    x->right = t;
    t->left  = y;

    /* Update heights */
    t->height = 1 + AVL_MAX(avl_tree_height(t->left), avl_tree_height(t->right));
    x->height = 1 + AVL_MAX(avl_tree_height(x->left), avl_tree_height(x->right));

    /* return new root */
    return(x);
}

/**
 *
 * @param n
 * @return
 */
static avl_tree_t* avl_tree_left_rotate(avl_tree_t* t)
{
    avl_tree_t* x = t->right;
    avl_tree_t* y = x->left;

    /* perform rotation */
    x->left  = t;
    t->right = y;

    /* Update heights */
    t->height = 1 + AVL_MAX(avl_tree_height(t->left), avl_tree_height(t->right));
    x->height = 1 + AVL_MAX(avl_tree_height(x->left), avl_tree_height(x->right));

    /* return new root */
    return(x);
}

/**
 *
 * @param n
 * @return
 */
static int avl_tree_get_balance(avl_tree_t* t)
{
    if (t == NULL)
    {
      return(0);
    }
    return(avl_tree_height(t->left) - avl_tree_height(t->right));
}

/**
 *
 * @param n
 * @param key
 * @param compar
 * @return
 */
avl_tree_t* avl_tree_insert(avl_tree_t* t, void* key, int (*compar)(const void*, const void*))
{
    /* 1.  Perform the normal BST insertion */
    if (t == NULL) {
        return(avl_tree_alloc(key));
    }

    if (compar(key, t->key) < 0)
    {
        t->left = avl_tree_insert(t->left, key, compar);
    }
    else
    {
        if (compar(key, t->key) > 0)
        {
            t->right = avl_tree_insert(t->right, key, compar);
        }
        else
        {
            /* not allowed */
            return(t);
        }
    }

    /* 2. Update height of this ancestor node */
    t->height = 1 + AVL_MAX(avl_tree_height(t->left), avl_tree_height(t->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = avl_tree_get_balance(t);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if ((balance > 1) && (compar(key, t->left->key) < 0))
    {
        return(avl_tree_right_rotate(t));
    }

    // Right Right Case
    if ((balance < -1) && (compar(key, t->right->key) > 0))
    {
        return(avl_tree_left_rotate(t));

    }

    // Left Right Case
    if ((balance > 1) && (compar(key, t->left->key) > 0))
    {
        t->left =  avl_tree_left_rotate(t->left);
        return(avl_tree_right_rotate(t));
    }

    // Right Left Case
    if ((balance < -1) && (compar(key, t->right->key) < 0))
    {
        t->right = avl_tree_right_rotate(t->right);
        return(avl_tree_left_rotate(t));
    }

    /* return the (unchanged) node pointer */
    return(t);
}

/**
 *
 * @param n
 * @return
 */
size_t avl_tree_height(avl_tree_t* t)
{
    if (t == NULL)
    {
        return(0);
    }
    return(t->height);
}

/**
 *
 * @param t
 * @param key
 * @param compar
 * @return
 */
int avl_tree_lookup(avl_tree_t* t, void* key, int (*compar)(const void*, const void*))
{
    while (t != NULL)
    {
        if (compar(t->key, key) > 0)
        {
            t = t->left;
        }
        else
        {
            if (compar(t->key, key) < 0)
            {
                t = t->right;
            }
            else
            {
                return(1);
            }
        }
    }

    /* still here !? not found */
    return(0);
}

