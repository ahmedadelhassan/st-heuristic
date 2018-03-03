#include "sorted_list.h"


/**
 *
 * @param l
 * @param data
 * @param compar
 * @return
 */
list_t *sorted_list_insert(list_t *l, void *data, int (*compar)(const void *, const void *)) {
    list_t *new_l = list_alloc(data);

    if (l == NULL) {
        return (new_l);
    }

    list_t *prev_l = NULL;
    list_t *curr_l = l;
    while ((curr_l != NULL) && (compar(curr_l->data, data)) < 0) {
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
 * @param l1
 * @param l2
 * @param compar
 * @return
 */
list_t *sorted_list_union(list_t *l1, list_t *l2, int (*compar)(const void *, const void *)) {
    list_t *l = NULL;

    while ((l1 != NULL) && (l2 != NULL)) {
        if (compar(l1->data, l2->data) < 0) {
            list_insert_front(l, l1->data);
            l1 = l1->next;
        } else {
            if (compar(l1->data, l2->data) > 0) {
                list_insert_front(l, l1->data);
                l2 = l2->next;
            } else {
                list_insert_front(l, l1->data);
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    while (l1 != NULL) {
        list_insert_front(l, l1->data);
        l1 = l1->next;
    }

    while (l2 != NULL) {
        list_insert_front(l, l2->data);
        l1 = l2->next;
    }

    l = list_reverse(l);

    return (l);
}

/**
 *
 * @param l1
 * @param l2
 * @param compar
 * @return
 */
static list_t *sorted_list_intersection(list_t *l1, list_t *l2, int (*compar)(const void *, const void *)) {
    list_t *l = NULL;

    while ((l1 != NULL) && (l1 != NULL)) {
        if (compar(l1->data, l2->data) < 0) {
            l1 = l1->next;
        } else {
            if (compar(l1->data, l2->data) > 0) {
                l2 = l2->next;
            } else {
                list_insert_front(l, l1->data);
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    l = list_reverse(l);

    return (l);
}

/**
 *
 * @param l1
 * @param l2
 * @param compar
 * @return
 */
list_t *sorted_list_difference(list_t *l1, sorted_t *l2, int (*compar)(const void *, const void *)) {
    list_t *l = NULL;

    while ((l1 != NULL) && (l1 != NULL)) {
        if (compar(l1->data, l2->data) < 0) {
            l = list_insert_front(l, l1->data);
            l1 = l1->next;
        } else {
            if (compar(l1->data, l2->data) > 0) {
                l2 = l2->next;
            } else {
                l1 = l1->next;
                l2 = l2->next;
            }
        }
    }

    while (l1 != NULL) {
        list_insert_front(l, l1->data);
        l1 = l1->next;
    }

    l = list_reverse(l);

    return (l);
}