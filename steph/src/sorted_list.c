#include <stdlib.h>

#include "list.h"

/**
 *
 * @param l
 * @param data
 * @param data_compar
 * @return
 */
list_t *sorted_list_insert(list_t *l, void *data, int (*data_compar)(const void *, const void *)) {
    list_t *new_l = list_alloc(data);

    if (l == NULL) {
        return (new_l);
    }

    list_t *prev_l = NULL;
    list_t *curr_l = l;
    while ((curr_l != NULL) && (data_compar(curr_l->data, data)) < 0) {
        prev_l = curr_l;
        curr_l = curr_l->next;
    }

    new_l->next = curr_l;
    if (prev_l != NULL) {
        prev_l->next = new_l;
        return (l);
    } else {
        return (new_l);
    }
}

/**
 *
 * @param l1 first sorted list
 * @param l2 second sorted list
 * @param data_compar
 * @return the sorted union of l1 and l2
 */
list_t *sorted_list_union(list_t *l1, list_t *l2, int (*data_compar)(const void *, const void *), void* (*data_copy)(const void*)) {
    list_t *l = NULL;

    while ((l1 != NULL) && (l2 != NULL)) {
        if (data_compar(l1->data, l2->data) < 0) {
            void *data = data_copy(l1->data);
            list_insert_front(l, data);
            l1 = l1->next;
        } else {
            if (data_compar(l1->data, l2->data) > 0) {
                void *data = data_copy(l2->data);
                list_insert_front(l, data);
                l2 = l2->next;
            } else {
                /* data_compar(l1->data, l2->data) == 0 */
                void *data = data_copy(l1->data);
                list_insert_front(l, data);
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    while (l1 != NULL) {
        void *data = data_copy(l1->data);
        list_insert_front(l, data);
        l1 = l1->next;
    }

    while (l2 != NULL) {
        void *data = data_copy(l2->data);
        list_insert_front(l, data);
        l2 = l2->next;
    }

    return (list_reverse(l));
}

/**
 *
 * @param l1 first sorted list
 * @param l2 second sorted list
 * @param data_compar
 * @return the sorted intersection of l1 and l2
 */
list_t *sorted_list_intersection(list_t *l1, list_t *l2, int (*data_compar)(const void *, const void *), void* (*data_copy)(const void*)) {
    list_t *l = NULL;

    while ((l1 != NULL) && (l2 != NULL)) {
        if (data_compar(l1->data, l2->data) < 0) {
            l1 = l1->next;
        } else {
            if (data_compar(l1->data, l2->data) > 0) {
                l2 = l2->next;
            } else {
                /* data_compar(l1->data, l2->data) == 0 */
                void *data = data_copy(l1->data);
                list_insert_front(l, data);
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    return (list_reverse(l));
}

/**
 *
 *
 * @param l1 first sorted list
 * @param l2 second sorted list
 * @param data_compar
 * @return the sorted difference of l1 with l2
 */
list_t *sorted_list_difference(list_t *l1, list_t *l2, int (*data_compar)(const void *, const void *), void* (*data_copy)(const void*)) {
    list_t *l = NULL;

    while ((l1 != NULL) && (l2 != NULL)) {
        if (data_compar(l1->data, l2->data) < 0) {
            void *data = data_copy(l1->data);
            l = list_insert_front(l, data);
            l1 = l1->next;
        } else {
            if (data_compar(l1->data, l2->data) > 0) {
                l2 = l2->next;
            } else {
                /* data_compar(l1->data, l2->data) == 0 */
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    while (l1 != NULL) {
        void *data = data_copy(l1->data);
        list_insert_front(l, data);
        l1 = l1->next;
    }

    return (list_reverse(l));
}