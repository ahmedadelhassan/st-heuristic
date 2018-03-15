#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

/**
 * \brief Allocate a new list.
 * @param data The data associated to the list
 * @return a new singleton list.
 */
list_t *p_list_alloc(void *data) {
    list_t *p_l = (list_t *) malloc(sizeof(list_t));
    if (!p_l) {
        fprintf(stderr, "list_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    /* singleton list */
    p_l->p_next = NULL;
    p_l->data = data;

    return (p_l);
}

/**
 * \brief Recursively release a list.
 * @param p_l A list
 * The associated data is not released.
 */
void list_release(list_t *p_l) {
    if (p_l) {
        list_release_(p_l->p_next);
        memset(p_l, 0x0, sizeof(list_t));
        free(p_l);
    }
}

/**
 * \brief recursively copy a list.
 * @param p_l A list
 * @return a new list holding the same data.
 */
list_t *p_list_copy(const list_t *p_l) {
    if (!p_l) {
        return (NULL);
    }

    list_t *p_copy_l = list_alloc(p_l->data);
    p_copy_l->p_next = list_copy(p_l->p_next);
    return (p_copy_l);
}

/**
 * \brief Number of elements in a list
 * @param l A list
 * @return the number of elements in the list \a l.
 */
size_t list_size(list_t *p_l) {
    size_t size = 0;
    while (p_l) {
        size++;
        p_l = p_l->p_next;
    }
    return (size);
}

/**
 * \brief Delete an element in a list.
 * @param p_l A list
 * @param i The index of the element to be deleted.
 * @return the list \a l without its \a i-th element.
 */
list_t *p_list_delete_ith(list_t *p_l, int i) {
    list_t *p_prev_l = NULL;
    list_t *p_it_l = l;

    while (p_it_l && i > 0) {
        p_prev_l = l;
        p_it_l = p_it_l->p_next;
        i--;
    }

    if (!p_it_l) {
        return (NULL);
    }

    /* p_l != NULL */
    if (!p_prev_l) {
        /* delete first item of l */
        list_t *p_next_l = p_l->p_next;
        p_it_l->p_next = NULL;
        list_release(p_it_l);
        return (p_next_l);
    }

    /* l != NULL && p_prev_l != NULL */
    prev_p_l->p_next = p_it_l->p_next;
    p_it_l->p_next = NULL;
    list_release(p_it_l);
    return (p_l);
}

/**
 * \brief Delete a random element in a list.
 * @param p_l A list
 * @return the list \a l without one of its element.
 */
list_t *p_list_delete_rand(list_t *p_l) {
    if (!p_l) {
        return (NULL);
    }

    int i = rand() % list_size(p_l);
    return list_delete_ith(p_l, i);
}

/**
 * \brief Insert a new element at the beginning of a list.
 * @param l A list
 * @param data A pointer to the hold data
 * @return a new list where \a data is inserted at the front of \a l.
 */
list_t *p_list_insert_front(list_t *p_l, void *data) {
    list_t *p_new_l = list_alloc(data);
    new_p_l->p_next = l;
    return (p_new_l);
}

/**
 * \brief Reverse a list.
 * @param p_l A list
 * @return the list \a l reversed.
 */
list_t *p_list_reverse(list_t *p_l) {
    list_t *p_rev_l = NULL;

    while (p_l) {
        list_t *p_tmp_l = p_l->p_next;
        p_l->p_next = p_rev_l;
        p_rev_l = l;
        p_l = p_tmp_l;
    }

    return (p_rev_l);
}

/**
 *
 * The following function moves the top item in the linked list_t
 * to its correct position.  This is similar to insertion sort.
 * The item that was the second item in the list_t becomes the
 * top item. The list_t should contain at least 2 items and
 * from the second item on, the list_t should already be sorted.
 */

/**
 *
 * @param l
 * @param data_compar
 * @return
 */
static list_t *p_list_move(list_t *p_l, int (*data_compar)(const void *, const void *)) {
    list_t *p_it_l;
    list_t *p_prev_l;
    list_t *p_new_l;

    p_prev_l = l;
    p_it_l = p_l->p_next;
    p_new_l = p_it_l;
    while (p_it_l && data_compar(p_l->data, p_it_l->data) > 0) {
        p_prev_l = p_it_l;
        p_it_l = p_it_l->p_next;
    }

    /* move the top item between p and n */
    prev_p_l->p_next = l;
    p_it_l->p_next = p_it_l;

    return (p_new_l);
}


/**
 * \brief Sort a list.
 * @param l A list
 * @param data_compar Data comparing function
 * @return the list \a l sorted according to \a data_compar comparing function.
 */
list_t *p_list_sort(list_t *p_l, int (*data_compar)(const void *, const void *)) {
    if (!p_l) {
        return (NULL);
    }

    p_l->p_next = list_sort(p_l->p_next, data_compar);
    if (p_l->p_next && data_compar(p_l->data, p_l->p_next->data) > 0) {
        p_l = list_move(p_l, data_compar);
    }

    return (p_l);
}
