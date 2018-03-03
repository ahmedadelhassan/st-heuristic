#include "heap.h"

heap_t *heap_alloc(heap_type_t type, size_t alloc_size, int (compar)(const void*, const void*)) {
    heap_t *h = (heap_t*) malloc(sizeof(heap_t));
    if (h == NULL) {
        fprintf(stderr, "heap_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    h->array = (void**) calloc(alloc_size, sizeof(void*));
    if (h->array == NULL) {
        fprintf(stderr, "heap_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    h->type = type;
    h->size = 0;
    h->alloc_size = alloc_size;
    h->compar= compar;

    return(h);
}


void heap_release(heap_t *h) {
    if (h != NULL) {
        if (h->array != NULL) {
            memset(h->array, 0x0, h->alloc_size * sizeof(void*));
            free(h->array);
        }

        memset(h, 0x0, sizeof(heap_t));
        free(h);
    }
}

static void heap_resize(const heap_t *h) {

}

void heap_insert(heap_t *h, const void *data) {
    if (h->size == h->alloc_size) {
        heap_resize(h);
    }

    h->size++;
    h->array[h->size] = data;
    heap_bubble_up(h);
}