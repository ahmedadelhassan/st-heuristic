#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

/**
 *
 * @return
 */
list_t *list_alloc() {
    list_t *l = (list_t *) malloc(sizeof(list_t));
    if (l == NULL) {
        perror("mem alloc");
        exit(EXIT_FAILURE);
    }

    l->next = NULL;
    l->data = NULL;

    return (l);
}

/**
 *
 * @param l
 */
static void list_free(list_t *l) {
    if (l != NULL) {
        memset(l, 0x0, sizeof(list_t));
        free(l);
    }
}

/**
 *
 * @param l
 */
void list_release(list_t *l) {
    if (l != NULL) {
        list_release(l->next);
        list_free(l);
    }
}

/**
 *
 * @param l
 * @return
 */
list_t *list_copy(const list_t *l) {
    list_t *new_l = NULL;

    while (l != NULL) {
        new_l = list_insert_front(new_l, l->data);
        l = l->next;
    }

    new_l = list_reverse(new_l);

    return (new_l);
}


/**
 *
 * @param l
 * @return
 */
size_t list_size(list_t *l) {
    size_t size = 0;
    while (l != NULL) {
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
list_t *list_delete_ith(list_t *l, size_t i) {
    list_t *prev_p = NULL;
    list_t *p = l;

    while ((p != NULL) && (i > 0)) {
        prev_p = p;
        p = p->next;
        i--;
    }

    if (p == NULL) {
        return (l);
    }

    /* p != NULL */
    if (prev_p == NULL) {
        /* delete first item of l */
        list_t *next_p = p->next;
        list_free(p);
        return (next_p);
    }

    /* p != NULL && prev_p != NULL */
    prev_p->next = p->next;
    list_free(p);
    return (l);
}


/**
 *
 * @param l
 * @param data
 */
list_t *list_insert_front(list_t *l, void *data) {
    list_t *new_l = list_alloc();
    new_l->next = l;
    new_l->data = data;
    return (new_l);
}


/**
 *
 * The following function moves the top item in the linked list_t
 * to its correct position.  This is similar to insertion sort.
 * The item that was the second item in the list_t becomes the
 * top item. The list_t should contain at least 2 items and
 * from the second item on, the list_t should already be sorted.
 *
 * @param l
 * @return
 */
list_t *list_reverse(list_t *l) {
    list_t rev_l = NULL;

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
 * @param [in] l Input list
 * @param compar Comparing function
 * @return
 */
static list_t *list_move(list_t *l, int (*compar)(const void *, const void *)) {
    list_t *n;
    list_t *p;
    list_t *ret;

    p = l;
    n = l->next;
    ret = n;
    while ((n != NULL) && (compar(l->data, n->data) > 0)) {
        p = n;
        n = n->next;
    }

    /* move the top item between p and n */
    p->next = l;
    l->next = n;

    return (ret);
}

/**
 *
 * @param start
 * @param compar
 * @return
 */
list_t *list_sort(list_t *start, int (*compar)(const void *, const void *)) {
    if (start == NULL) {
        return (NULL);
    }

    start->next = list_sort(start->next, compar);
    if ((start->next != NULL) && (compar(start->data, start->next->data) > 0)) {
        start = list_move(start, compar);
    }

    return (start);
}

/**
 *
 * @param l1
 * @param l2
 * @param compar
 * @return
 */
list_t *list_union(list_t *l1, t *l2, int (*compar)(const void *, const void *)) {
    list_t *sorted_l1 = list_sort(l1, compar);
    list_t *sorted_l2 = list_sort(l2, compar);
    list_t *l = sorted_list_union(sorted_l1, sorted_l2, compar);
    return (l);
}

/**
 *
 * @param l1
 * @param l2
 * @param compar
 * @return
 */
list_t *list_intersection(list_t *l1, t *l2, int (*compar)(const void *, const void *)) {
    list_t *sorted_l1 = list_sort(l1, compar);
    list_t *sorted_l2 = list_sort(l2, compar);
    list_t *l = sorted_list_intersection(sorted_l1, sorted_l2, compar);
    return (l);
}

/**
 *
 * @param l1
 * @param l2
 * @param compar
 * @return
 */
list_t *list_difference(list_t *l1, t *l2, int (*compar)(const void *, const void *)) {
    list_t *sorted_l1 = list_sort(l1, compar);
    list_t *sorted_l2 = list_sort(l2, compar);
    list_t *l = sorted_list_difference(sorted_l1, sorted_l2, compar);
    return (l);
}