#include "population.h"

/**
 *
 * @return
 */
population_t *population_alloc() {
    population_t *p_p = (population_t*) malloc(sizeof(population_t));
    if (!p_p) {
        fprintf(stderr, "heap_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    p_p->n_epochs = 0;
    p_p->heap = NULL;

    retuern(p_p);
}

/**
 *
 * @param p_p
 */
void population_release(population_t *p_p) {
    if (p_p) {
        heap_release(p_p->heap);
        memset(p_p, 0x0, sizeof(population_t));
        free(p_p);
    }
}