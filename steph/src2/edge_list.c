#include "edge_list.h"
#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 *
 * @param e
 * @return
 */
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

/**
 *
 * @param p_el
 */
static void edge_list_release_single(edge_list_t *p_el) {
    if (p_el) {
        memset(p_el, 0x0, sizeof(edge_list_t));
        free(p_el);
    }
}

/**
 *
 * @param p_el
 */
void edge_list_release(edge_list_t *p_el) {
    while (p_el) {
        edge_list_t *p_next_el = p_el->p_next;
        edge_list_release_single(p_el);
        p_el = p_next_el;
    }
}

/**
 *
 * @param p_el
 * @return
 */
edge_list_t *edge_list_copy(edge_list_t *p_el) {
    if (!p_el) {
        return (NULL);
    }

    edge_list_t *p_copy_el = edge_list_alloc(p_el->edge);
    edge_list_t *p_it_el = p_el->p_next;
    edge_list_t *p_it_copy_el = p_copy_el;
    while (p_it_el) {
        p_it_copy_el->p_next = edge_list_alloc(p_it_el->edge);
        p_it_el = p_it_el->p_next;
        p_it_copy_el = p_it_copy_el->p_next;
    }

    return (p_copy_el);
}

/**
 *
 * @param p_el
 * @return
 */
edge_list_t *edge_list_reverse(edge_list_t *p_el) {
    edge_list_t *p_rev_el = NULL;
    while (p_el) {
        edge_t e = p_el->edge;
        edge_list_insert_front(p_rev_el, e);
        p_el = p_el->p_next;
    }
    return (p_rev_el);
}

/**
 *
 * @param p_el
 * @return
 */
size_t edge_list_size(const edge_list_t *p_el) {
    size_t size = 0;
    while (p_el) {
        size++;
        p_el = p_el->p_next;
    }
    return (size);
}

weight_t edge_list_total_weight(const edge_list_t *p_el) {
    weight_t total_weight = 0;
    while (p_el) {
        total_weight += p_el->edge.weight;
        p_el = p_el->p_next;
    }
    return (total_weight);
}

/**
 *
 * @param p_el
 * @return
 */
weight_t edge_list_total_weight(edge_list_t *p_el) {
    weight_t weight = 0;
    while (p_el) {
        weight += p_el->edge.weight;
        p_el = p_el->p_next;
    }
    return (weight;)
}

/**
 *
 * @param p_el
 * @param compar
 * @return
 */
static void edge_list_insert(edge_list_t **p_p_el, edge_list_t *p_el, int (compar)(const void *, const void *)) {
    if (!(*p_p_el) || compar(ADDR(p_el->edge), ADDR((*p_p_el)->edge)) <= 0) {
        p_el->p_next = *p_p_el;
        *p_p_el = p_el;
    } else {
        edge_list_t *p_it_el = *p_p_el;
        while (p_it_el->p_next && compar(ADDR(p_it_el->p_next->edge), ADDR(p_el->edge)) < 0) {
            p_it_el = p_it_el->p_next;
        }
        p_el->p_next = p_it_el->p_next;
        p_it_el->p_next = p_el;
    }
}

/**
 *
 * @param p_el
 * @return
 */
edge_list_t *edge_list_sort_by_endpoints(edge_list_t *p_el) {
    if (!p_el) {
        return (NULL);
    }

    if (!p_el->p_next) {
        return (p_el);
    }

    /* sort tail */
    edge_list_t *p_sorted_el = edge_list_sort_by_endpoints(p_el->p_next);
    p_el->p_next = NULL;

    /* insert head in sorted tail */
    edge_list_insert(ADDR(p_sorted_el), p_el, edge_compar_by_endpoints);

    return (p_sorted_el);
}

/**
 *
 * @param p_el
 * @return
 */
edge_list_t *edge_list_sort_by_weight(edge_list_t *p_el) {
    if (!p_el) {
        return (NULL);
    }

    if (!p_el->p_next) {
        return (p_el);
    }

    /* sort tail */
    edge_list_t *p_sorted_el = edge_list_sort_by_endpoints(p_el->p_next);
    p_el->p_next = NULL;

    /* insert head in sorted tail */
    edge_list_insert(ADDR(p_sorted_el), p_el, edge_compar_by_weight);

    return (p_sorted_el);
}

/**
 *
 * @param p_el
 * @param e
 * @return
 */
edge_list_t *edge_list_insert_front(edge_list_t *p_el, edge_t e) {
    edge_list_t *p_new_el = edge_list_alloc(e);
    p_new_el->p_next = p_el;
    return (p_new_el);
}

/**
 *
 * @param f
 * @param p_el
 */
void edge_list_fprint(FILE *f, const edge_list_t *p_el) {
    while (p_el) {
        edge_t e = p_el->edge;
        edge_fprint(f, e);
        p_el = p_el->p_next;
        if (p_el) {
            fprintf(f, ",");
        }
    }
    fprintf(f, "\n");
}