#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

/**
 *
 * @param e
 * @return
 */
list_t *list_alloc(void *data) {
    list_t *l = (list_t *) malloc(sizeof(list_t));
    if (! l) {
        fprintf(stderr, "list_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    l->next = NULL;
    l->data = data;

    return (l);
}

/**
 *
 * @param l
 */
void list_release(list_t *l) {
    list_release_with_data_release(l, NULL);
}

/**
 *
 * @param l
 * @param data_release
 */
void list_release_with_data_release(list_t *l, void (*data_release)(void *)) {
    if (l != NULL) {
        list_release_with_data_release(l->next, data_release);
        if (data_release) {
            data_release(l->data);
        }
        memset(l, 0x0, sizeof(list_t));
        free(l);
    }
}

/**
 *
 * @param l
 * @return
 */
list_t *list_copy(const list_t *l) {
    return (list_copy_with_data_copy(l, NULL));
}

/**
 *
 * @param l
 * @param data_copy
 * @return
 */
list_t *list_copy_with_data_copy(const list_t *l, void *(*data_copy)(const void *)) {
    if (! l) {
        return (NULL);
    } else {
        void *data = data_copy ? data_copy(l->data) : l->data;
        list_t *copy_l = list_alloc(data);
        copy_l->next = list_copy(l->next);
        return (copy_l);
    }
}

/**
 *
 * @param l
 * @return
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
 *
 * @param l
 * @param i
 * @return
 */
list_t *list_delete_ith(list_t *l, unsigned int i) {
    return(list_delete_ith_with_data_release(l, i, NULL));
}

/**
 *
 * @param l
 * @param i
 * @param data_release
 * @return
 */
list_t *list_delete_ith_with_data_release(list_t *l, unsigned int i, void (*data_release)(void *)) {
    list_t *prev_l = NULL;
    list_t *it_l = l;

    while (it_l && i > 0) {
        prev_l = l;
        it_l = it_l->next;
        i--;
    }

    if (! it_l) {
        return (NULL);
    }

    /* l != NULL */
    if (! prev_l) {
        /* delete first item of l */
        list_t *next_l = l->next;
        it_l->next = NULL;
        list_release_with_data_release(it_l, data_release);
        return (next_l);
    }

    /* l != NULL && prev_l != NULL */
    prev_l->next = it_l->next;
    it_l->next = NULL;
    list_release_with_data_release(it_l, data_release);
    return (l);
}

/**
 *
 * @param l
 * @return
 */
list_t *list_delete_rand(list_t *l) {
    return (list_delete_rand_with_data_release(l, NULL));
}

/**
 *
 * @param l
 * @return
 */
list_t *list_delete_rand_with_data_release(list_t *l, void (*data_release)(void *)) {
    if (!l) {
        return (NULL);
    }

    int i = rand() % list_size(l);
    return list_delete_ith_with_data_release(l, i, data_release);
}

/**
 *
 * @param l
 * @param n
 * @return
 */
list_t *list_insert_front(list_t *l, void *data) {
    list_t *new_l = list_alloc(data);
    new_l->next = l;
    return (new_l);
}

/**
 *
 * @param l
 * @return
 */
list_t *list_reverse(list_t *l) {
    list_t *rev_l = NULL;

    while (l != NULL) {
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
 *
 * @param l
 * @return
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

/**
 *
 * @param l1 sorted node list
 * @param l2 sorted node list
 * @return
 */
list_t *list_union(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*)) {
    return list_union_with_data_alloc(l1, l2, data_compar, NULL);
}

/**
 *
 * @param l1
 * @param l2
 * @param data_alloc
 * @return
 */
list_t *list_union_with_data_alloc(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*), void *(*data_alloc)(void *)) {
    list_t *l = NULL;

    while (l1 && l2) {
        if (data_compar(l1->data, l2->data) < 0) {
            void *data = data_alloc ? data_alloc(l1->data) : l1->data;
            l = list_insert_front(l, data);
            l1 = l1->next;
        } else {
            if (data_compar(l1->data, l2->data) > 0) {
                void *data = data_alloc ? data_alloc(l2->data) : l2->data;
                l = list_insert_front(l, data);
                l2 = l2->next;
            } else {
                /* data_compar(l1->data, l2->data) == 0 */
                void *data = data_alloc ? data_alloc(l1->data) : l1->data;
                l = list_insert_front(l, data);
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    while (l1) {
        void *data = data_alloc ? data_alloc(l1->data) : l1->data;
        l = list_insert_front(l, data);
        l1 = l1->next;
    }

    while (l2) {
        void *data = data_alloc ? data_alloc(l2->data) : l2->data;
        l = list_insert_front(l, data);
        l2 = l2->next;
    }

    return (list_reverse(l));
}

/**
 *
 * @param l1 sorted node list
 * @param l2 sorted node list
 * @return
 */
list_t *list_intersection(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*)) {
    return (list_intersection_with_data_alloc(l1, l2, data_compar, NULL));
}

/**
 *
 * @param l1
 * @param l2
 * @param data_alloc
 * @return
 */
list_t *list_intersection_with_data_alloc(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*), void *(*data_alloc)(void *)) {

    list_t *l = NULL;

    while (l1 && l2) {
        if (data_compar(l1->data, l2->data) < 0) {
            l1 = l1->next;
        } else {
            if (data_compar(l1->data, l2->data) > 0) {
                l2 = l2->next;
            } else {
                /* data_compar(l1->data, l2->data) == 0 */
                void *data = data_alloc ? data_alloc(l1->data) : l1->data;
                l = list_insert_front(l, data);
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    return (list_reverse(l));
}

/**
 *
 * @param l1
 * @param l2
 * @return
 */
list_t *list_difference(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*)) {
    return (list_difference_with_data_alloc(l1, l2, data_compar, NULL));
}

/**
 * 
 * @param l1
 * @param l2
 * @param data_alloc
 * @return
 */
list_t *list_difference_with_data_alloc(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*), void *(*data_alloc)(void *)) {
    list_t *l = NULL;

    while (l1 && l2) {
        if (data_compar(l1->data, l2->data) < 0) {
            l = list_insert_front(l, l1->data);
            l1 = l1->next;
        } else {
            if (l1->data > l1->data) {
                l2 = l2->next;
            } else {
                /* data_compar(l1->data, l2->data) == 0 */
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    while (l1) {
        l = list_insert_front(l, l1->data);
        l1 = l1->next;
    }

    return (list_reverse(l));
}