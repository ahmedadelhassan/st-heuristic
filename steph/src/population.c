#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "individual.h"
#include "population.h"

#define POPULATION_PARENT(i) (((i) - 1) >> 1)
#define POPULATION_LEFT(i)   (((i) << 1) + 1)
#define POPULATION_RIGHT(i)  (((i) << 1) + 2)

/**
 * 
 * @param capacity
 * @return
 */
population_t *population_alloc(size_t capacity) {
    /* allocate population */
    population_t *p_population = (population_t *) malloc(sizeof(population_t));
    if (!p_population) {
        fprintf(stderr, "population_alloc. memory allocation error for capacity %u.\n", capacity);
        exit(EXIT_FAILURE);
    }

    /* allocate individuals */
    p_population->p_individuals = (individual_t **) calloc(capacity, sizeof(individual_t *));
    if (!p_population->p_individuals) {
        fprintf(stderr, "population_alloc. memory allocation error for individuals.\n");
        exit(EXIT_FAILURE);
    }

    /* initialize population */
    p_population->n_individuals = 0;
    p_population->capacity = capacity;

    return (p_population);
}

/**
 *
 * @param p_population
 */
void population_release(population_t *p_population) {
    if (p_population) {
        /* release individuals */
        for (int i = 0; i < p_population->n_individuals; i++) {
            individual_release(p_population->n_individuals[i]);
        }
        memset(p_population->p_individuals, 0x0, p_population * sizeof(individual_t*));
        free(p_population->p_individuals);

        /* release the population itself */
        memset(p_population, 0x0, sizeof(population_t));
        free(p_population);
    }
}

/**
 *
 * @param p_population
 */
static void population_resize(population_t *p_population) {
    assert(p_population);

    /* double the capacity */
    size_t new_capacity = MAX(1, 2 * p_population->capacity));
    p_population->capacity = new_capacity;

    /* new allocation */
    p_population->p_individuals = (individual_t **) calloc(new_capacity, sizeof(individual_t *));
    if (!p_population->p_individuals) {
        fprintf(stderr, "population_resize. memory allocation error for new capacity %u.\n", new_capacity);
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * @param p_population
 * @param i
 */
static void population_bubble_up(population_t *p_population, int i) {
    assert(p_population);

    int parent_index = POPULATION_PARENT(i);
    if (individual_compar_compar(p_population->p_individuals[i], p_population->p_individuals[parent_index]) < 0) {
        individual_t *p_tmp_individual = p_population->p_individuals[i];
        p_population->p_individuals[i] = p_population->p_individuals[parent_index];
        p_population->p_individuals[parent_index] = p_tmp_individual;
        population_bubble_up(p_population, parent_index);
    }
}

/**
 *
 * @param p_population
 * @param p_individual
 */
void population_insert(population_t *p_population, individual_t *p_individual) {
    assert(p_population);
    assert(p_individual);

    /* resize if needed */
    if (p_population->n_individuals == p_population->capacity) {
        population_resize(p_population);
    }

    /* insert the new individual */
    p_population->p_individuals[p_population->n_individuals++] = p_individual;
    population_bubble_up(p_population, p_population->n_individuals - 1);
}

/**
 *
 * @param p_population
 * @param i
 */
static void population_bubble_down(population_t *p_population, size_t i) {
    assert(p_population);
    assert(i < p_population->n_individuals);

    int left_idx = POPULATION_LEFT(i);
    int right_idx = POPULATION_RIGHT(i);

    if (left_idx >= p_population->n_individuals) {
        /* i is a leaf index */
        return;
    }

    int max_idx = i;

    if (individual_compar(p_population->p_individuals[i], p_population->p_individuals[left_idx]) < 0) {
        max_idx = left_idx;
    }

    if (right_idx < p_population->n_individuals) {
        if (individual_compar(p_population->p_individuals[i], p_population->p_individuals[left_idx]) < 0) {
            max_idx = right_idx;
        }
    }

    if (i != max_idx) {
        void *p_tmp_individual = p_population->p_individuals[i];
        p_population->p_individuals[i] = p_population->p_individuals[max_idx];
        p_population->p_individuals[max_idx] = p_tmp_individual;
        population_bubble_down(p_population, max_idx);
    }
}

/**
 *
 * @param p_population
 * @param i
 * @return
 */
static individual_t *population_extract(population_t *p_population, size_t i) {
    assert(p_population);
    assert(i < p_population->n_individuals);

    individual_t *p_individual = p_population->p_individuals[i];
    p_population->p_individuals[i] = p_population->p_individuals[p_population->n_individuals - 1];
    p_population->p_individuals[p_population->n_individuals - 1] = NULL;

    p_population->n_individuals--;
    population_bubble_down(p_population, i);

    /* update min total weight if needed */
    if (p_individual->total_weight == p_population->min_total_weight) {
        if (p_population->n_individuals > 0) {
            p_population->min_total_weight = population_get_min_weight(p_population);
        } else {
            p_population->min_total_weight = 0;
        }
    }

    /* update max total weight if needed */
    if (p_individual->total_weight == p_population->max_total_weight) {
        if (p_population->n_individuals > 0) {
            p_population->min_total_weight = population_get_max_weight(p_population);
        } else {
            p_population->min_total_weight = 0;
        }
    }

    return (p_individual);
}

/**
 *
 * @param p_population
 * @return
 */
individual_t *population_extract_rand(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    size_t i = rand() % p_population->n_individuals;
    return (population_extract(p_population, i));
}

/**
 *
 * @param p_population
 * @return
 */
individual_t *population_extract_max(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    return (population_extract(p_population, 0));
}

/**
 *
 * @param p_population
 * @return
 */
individual_t *population_extract_min(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    size_t min_i = p_population->n_individuals - 1;
    weight_t min_w = p_population->p_individuals[min_i]->total_weight;
    while (POPULATION_LEFT(i) > p_population->n_individuals && POPULATION_RIGHT(i) > p_population->n_individuals) {
        if (p_population->p_individuals[min_i]->total_weight < min_w) {
            min_i = i;
            min_w = p_population->p_individuals[min_i]->total_weight;
        }
        i--;
    }

    return (population_extract(p_population, min_i));
}

/**
 *
 * @param p_population
 * @return
 */
void *population_get_max_weight(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    return (p_population->p_individuals[0]->);
}

/**
 *
 * @param p_population
 * @return
 */
weight_t population_get_min_weight(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    size_t min_i = p_population->n_individuals - 1;
    weight_t min_w = p_population->p_individuals[min_i]->total_weight;
    while (POPULATION_LEFT(i) > p_population->n_individuals && POPULATION_RIGHT(i) > p_population->n_individuals) {
        if (p_population->p_individuals[min_i]->total_weight < min_w) {
            min_i = i;
            min_w = p_population->p_individuals[min_i]->total_weight;
        }
    }

    return (p_population->p_individuals[min_i]);
}