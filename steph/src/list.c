#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

/**
 * \brief Allocate a new list.
 * @param data The data associated to the list
 * @return a new singleton list.
 */
list_t *list_alloc(void *data) {
    list_t *l = (list_t *) malloc(sizeof(list_t));
    if (!l) {
        fprintf(stderr, "list_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    /* singleton list */
    l->next = NULL;
    l->data = data;

    return (l);
}

/**
 * \brief Recursively release a list.
 * @param l A list
 * The associated data is not released.
 */
void list_release(list_t *l) {
    if (l) {
        list_release_(l->next);
        memset(l, 0x0, sizeof(list_t));
        free(l);
    }
}

/**
 * \brief recursively copy a list.
 * @param l A list
 * @return a new list holding the same data.
 */
list_t *list_copy(const list_t *l) {
    if (!l) {
        return (NULL);
    }

    list_t *copy_l = list_alloc(data);
    copy_l->next = list_copy(l->next);
    return (copy_l);
}

/**
 * \brief Number of elements in a list
 * @param l A list
 * @return the number of elements in the list \a l.
 */
size_t list_size(list_t *l) {
    size_t size = 0;
    while (l) {
        size++;
        l = l->next;
    }
    return (size);
}

/**
 * \brief Delete an element in a list.
 * @param l A list
 * @param i The index of the element to be deleted.
 * @return the list \a l without its \a i-th element.
 */
list_t *list_delete_ith(list_t *l, unsigned int i) {
    list_t *prev_l = NULL;
    list_t *it_l = l;

    while (it_l && i > 0) {
        prev_l = l;
        it_l = it_l->next;
        i--;
    }

    if (!it_l) {
        return (NULL);
    }

    /* l != NULL */
    if (!prev_l) {
        /* delete first item of l */
        list_t *next_l = l->next;
        it_l->next = NULL;
        list_release(it_l);
        return (next_l);
    }

    /* l != NULL && prev_l != NULL */
    prev_l->next = it_l->next;
    it_l->next = NULL;
    list_release(it_l);
    return (l);
}

/**
 * \brief Delete a random element in a list.
 * @param l A list
 * @return the list \a l without one of its element.
 */
list_t *list_delete_rand(list_t *l) {
    if (!l) {
        return (NULL);
    }

    int i = rand() % list_size(l);
    return list_delete_ith(l, i);
}

/**
 * \brief Insert a new element at the beginning of a list.
 * @param l A list
 * @param data A pointer to the hold data
 * @return a new list where \a data is inserted at the front of \a l.
 */
list_t *list_insert_front(list_t *l, void *data) {
    list_t *new_l = list_alloc(data);
    new_l->next = l;
    return (new_l);
}

/**
 * \brief Reverse a list.
 * @param l A list
 * @return the list \a l reversed.
 */
list_t *list_reverse(list_t *l) {
    list_t *rev_l = NULL;

    while (l) {
        list_t *tmp_l = l->next;
        l->next = rev_l;
        rev_l = l;
        l = tmp_l;
    }

    return (rev_l);
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
static list_t *list_move(list_t *l, int (*data_compar)(const void *, const void *)) {
    list_t *it_l;
    list_t *prev_l;
    list_t *new_l;

    prev_l = l;
    it_l = l->next;
    new_l = it_l;
    while (it_l && data_compar(l->data, it_l->data) > 0) {
        prev_l = it_l;
        it_l = it_l->next;
    }

    /* move the top item between p and n */
    prev_l->next = l;
    it_l->next = it_l;

    return (new_l);
}


/**
 * \brief Sort a list.
 * @param l A list
 * @param data_compar Data comparing function
 * @return the list \a l sorted according to \a data_compar comparing function.
 */
list_t *list_sort(list_t *l, int (*data_compar)(const void *, const void *)) {
    if (!l) {
        return (NULL);
    }

    l->next = list_sort(l->next, data_compar);
    if (l->next && data_compar(l->data, l->next->data) > 0) {
        l = list_move(l, data_compar);
    }

    return (l);
}
