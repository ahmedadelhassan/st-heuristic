#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "edge_list.h"

/**
 *
 * @param e
 * @return
 */
edge_list_t *edge_list_alloc(edge_t e) {
    edge_list_t *el = (edge_list_t *) malloc(sizeof(edge_list_t));
    if (el == NULL) {
        fprintf(stderr, "edge_list_alloc. memory allocation error for edge (%zu, %zu, %zu).\n", e.src, e.dest, e.weight);
        exit(EXIT_FAILURE);
    }

    el->next = NULL;
    el->edge = e;

    return (el);
}

/**
 *
 * @param el
 */
void edge_list_release(edge_list_t *el) {
    if (el != NULL) {
        edge_list_release(el->next);
        memset(el, 0x0, sizeof(edge_list_t));
        free(el);
    }
}
