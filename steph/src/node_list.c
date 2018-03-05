#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "node_list.h"

/**
 *
 * @param e
 * @return
 */
node_list_t *node_list_alloc(node_t n) {
    node_list_t *nl = (node_list_t *) malloc(sizeof(node_list_t));
    if (nl == NULL) {
        fprintf(stderr, "node_list_alloc. memory allocation error for node (%zu, %zu, %zu).\n", e.src, e.dest,
                e.weight);
        exit(EXIT_FAILURE);
    }

    nl->next = NULL;
    nl->node = n;

    return (nl);
}

/**
 *
 * @param nl
 */
void node_list_release(node_list_t *nl) {
    if (nl != NULL) {
        node_list_release(nl->next);
        memset(nl, 0x0, sizeof(node_list_t));
        free(nl);
    }
}

/**
 *
 * @param nl
 * @return
 */
node_list_t *node_list_copy(const node_list_t *nl) {
    if (nl == NULL) {
        return (NULL);
    } else {
        node_list_t *new_nl = node_list_alloc(nl.node);
        new_nl->next = node_list_copy(nl->next);
        return (new_nl);
    }
}

/**
 *
 * @param nl
 * @return
 */
size_t node_list_size(node_list_t *nl) {
    size_t size = 0;
    while (nl != NULL) {
        size++;
        nl = nl->next;
    }
    return (size);
}

/**
 *
 * @param nl
 * @param i
 * @return
 */
node_list_t *node_list_delete_ith(node_list_t *nl, unsigned int i) {
    node_list_t *prev_nl = NULL;
    node_list_t *it_nl = nl;

    while ((it_nl != NULL) && (i > 0)) {
        prev_nl = nl;
        it_nl = it_nl->next;
        i--;
    }

    if (it_nl == NULL) {
        return (NULL);
    }

    /* nl != NULL */
    if (prev_nl == NULL) {
        /* delete first item of nl */
        list_t *next_nl = nl->next;
        it_nl->next == NULL;
        node_list_release(it_nl);
        return (next_nl);
    }

    /* nl != NULL && prev_nl != NULL */
    prev_nl->next = it_nl->next;
    it_nl->next = NULL;
    list_release(it_nl);
    return (nl);
}

/**
 *
 * @param nl
 * @param n
 * @return
 */
node_list_t *node_list_insert_front(node_list_t *nl, node_t n) {
    node_list_t *new_nl = node_list_alloc(n);
    new_nl->next = nl;
    return (new_nl);
}

/**
 *
 * @param l
 * @return
 */
node_list_t *node_list_reverse(node_list_t *nl) {
    node_list_t *rev_nl = NULL;

    while (nl != NULL) {
        node_list_t *tmp_nl = nl->next;
        nl->next = rev_nl;
        rev_nl = nl;
        nl = tmp_nl;
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
 *
 * @param [in] l Input list
 * @param compar Comparing function
 * @return
 */
static node_list_t *node_list_move(node_list_t *nl) {
    node_list_t *it_nl;
    node_list_t *prev_nl;
    node_list_t *new_nl;

    prev_nl = nl;
    it_nl = l->next;
    new_nl = it_nl;
    while ((it_nl != NULL) && (nl->node > it_nl->node)) {
        prev_nl = it_nl;
        it_nl = it_nl->next;
    }

    /* move the top item between p and n */
    prev_nl->next = nl;
    it_nl->next = it_nl;

    return (new_nl);
}

/**
 *
 * @param nl
 * @return
 */
node_list_t *node_list_sort(node_list_t *nl) {
    if (nl == NULL) {
        return (NULL);
    }

    nl->next = node_list_sort(nl->next);
    if ((nl->next != NULL) && (nl->node, nl->next->node)) {
        nl = node_list_move(nl);
    }

    return (nl);
}

/**
 *
 * @param nl1 sorted node list
 * @param nl2 sorted node list
 * @return
 */
node_list_t *node_list_union(node_list_t *nl1, node_list_t *nl2) {
    list_t *nl = NULL;

    while ((nl1 != NULL) && (nl2 != NULL)) {
        if (nl1->node < nl2->node) {
            nl = node_list_insert_front(nl, nl1->node);
            nl1 = nl1->next;
        } else {
            if (nl1->node > nl2->node) {
                nl = node_list_insert_front(nl, nl2->noded);
                nl2 = nl2->next;
            } else {
                /* nl1->node == nl2->node */
                nl = node_list_insert_front(nl, nl1->node);
                nl1 = nl1->next;
                nl2 = nl2->next;
            }
        }
    }

    while (nl1 != NULL) {
        nl = node_list_insert_front(nl, nl1->node);
        nl1 = nl1->next;
    }

    while (nl2 != NULL) {
        nl = node_list_insert_front(nl, nl2->node);
        nl2 = nl2->next;
    }

    return (node_list_reverse(nl));
}

/**
 *
 * @param nl1 sorted node list
 * @param nl2 sorted node list
 * @return
 */
node_list_t *node_list_intersection(node_list_t *nl1, node_list_t *nl2) {
    node_list_t *nl = NULL;

    while ((nl1 != NULL) && (nl2 != NULL)) {
        if (nl1->node < nl2->node) {
            nl1 = nl1->next;
        } else {
            if (nl1->node > nl2->node) {
                nl2 = nl2->next;
            } else {
                /* nl1->node == nl2->node */
                nl = node_list_insert_front(nl, nl1->node);
                nl1 = nl1->next;
                nl2 = nl2->next;
            }
        }
    }

    return (node_list_reverse(nl));
}

/**
 *
 * @param nl1
 * @param nl2
 * @return
 */
node_list_t *node_list_difference(node_list_t *nl1, node_list_t *nl2) {
    node_list_t *nl = NULL;

    while ((nl1 != NULL) && (nl2 != NULL)) {
        if (nl1->node < nl2->node) {
            nl = node_list_insert_front(nl, nl1->node);
            nl1 = nl1->next;
        } else {
            if (nl1->node > nl1->node) {
                nl2 = nl2->next;
            } else {
                /* nl1->node == nl2->node */
                nl1 = nl1->next;
                nl2 = nl2->next;
            }
        }
    }

    while (nl1 != NULL) {
        nl = node_list_insert_front(nl, nl1->node);
        nl1 = nl1->next;
    }

    return (list_reverse(nl));
}