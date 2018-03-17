#include "edge_list.h"
#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

edge_list_t *edge_list_alloc(edge_t e) {
    edge_list_t *p_el = (edge_list_t *) malloc(sizeof(edge_list_t));
    if (!p_el) {
        fprintf(stderr, "edge_list_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    /* singleton list */
    p_el->p_next = NULL;
    p_el->edge = e;

    return (p_el);
}

static void edge_list_release_single(edge_list_t *p_el) {
    if (p_el) {
        memset(p_el, 0x0, sizeof(edge_list_t));
        free(p_el);
    }
}

void edge_list_release(edge_list_t *p_el) {
    while (p_el) {
        edge_list_t *p_next_el = p_el->p_next;
        edge_list_release_single(p_el);
        p_el = p_next_el;
    }
}

size_t edge_list_size(edge_list_t *p_el) {
    size_t size = 0;
    while (p_el) {
        size++;
        p_el = p_el->p_next;
    }
    return (size);
}

static edge_list_t *list_move(edge_list_t *p_el) {
    edge_list_t *p_it_el;
    edge_list_t *p_prev_el;
    edge_list_t *p_new_el;

    p_prev_el = p_el;
    p_it_el = p_el->p_next;
    p_new_el = p_it_el;
    while (p_it_el && edge_compar(ADDR(p_el->edge), ADDR(p_it_el->edge)) > 0) {
        p_prev_el = p_it_el;
        p_it_el = p_it_el->p_next;
    }

    /* move the top item between p and n */
    p_prev_el->p_next = p_el;
    p_it_el->p_next = p_it_el;

    return (p_new_el);
}

edge_list_t *edge_list_sort(edge_list_t *p_el) {
    if (!p_el) {
        return (NULL);
    }

    p_el->p_next = edge_list_sort(p_el->p_next);
    if (p_el->p_next && edge_compar(ADDR(p_el->edge), ADDR(p_el->p_next->edge)) > 0) {
        p_el = list_move(p_el);
    }

    return (p_el);
}

edge_list_t *edge_list_insert_front(edge_list_t *p_el, edge_t e) {
    edge_list_t *p_new_el = edge_list_alloc(e);
    p_new_el->p_next = p_el;
    return (p_new_el);
}

