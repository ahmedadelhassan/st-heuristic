#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "individual.h"
#include "population.h"
#include "statistics.h"

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
        fprintf(stderr, "population_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    /* allocate individuals */
    p_population->p_individuals = (individual_t *) calloc(capacity, sizeof(individual_t));
    if (!p_population->p_individuals) {
        fprintf(stderr, "population_alloc. memory allocation error %lu individuals.\n", capacity);
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
        /* release all individuals */
        for (int i = 0; i < p_population->n_individuals; i++) {
            individual_release(ADDR(p_population->p_individuals[i]));
        }
        memset(p_population->p_individuals, 0x0, p_population->n_individuals * sizeof(individual_t *));
        free(p_population->p_individuals);

        /* release the population itself */
        memset(p_population, 0x0, sizeof(population_t));
        free(p_population);
    }
}

/**
 *
 * @param p_population
 * @param i
 */
static void population_bubble_up(population_t *p_population, int i) {
    assert(p_population);

    int parent_i = POPULATION_PARENT(i);
    if (individual_compar(p_population->p_individuals[i], p_population->p_individuals[parent_i]) < 0) {
        individual_t p_tmp_individual = p_population->p_individuals[i];
        p_population->p_individuals[i] = p_population->p_individuals[parent_i];
        p_population->p_individuals[parent_i] = p_tmp_individual;
        population_bubble_up(p_population, parent_i);
    }
}

/**
 *
 * @param p_population
 * @param p_individual
 */
int population_insert_individual(population_t *p_population, individual_t individual) {
    assert(p_population);

    if (p_population->n_individuals == p_population->capacity) {
        individual_t max_weight_individual = population_get_max_total_weight_individual(p_population);
        weight_t max_weight = max_weight_individual.total_weight;
        if (max_weight > individual.total_weight) {
            /* the new individual is inserted and a max weight individual is released to make room */
            individual_t individual_max = population_extract_max_total_weight_individual(p_population);
            individual_release(ADDR(individual_max));
        } else {
            /* the new individual is not inserted */
            individual_release(ADDR(individual));
            return (0);
        }
    }

    /* insert the new individual */
    p_population->p_individuals[p_population->n_individuals++] = individual;
    population_bubble_up(p_population, p_population->n_individuals - 1);

    return (1);
}

/**
 *
 * @param p_population
 * @param i
 */
static void population_bubble_down(population_t *p_population, size_t i) {
    assert(p_population);
    assert(i < p_population->n_individuals);

    int left_i = POPULATION_LEFT(i);
    int right_i = POPULATION_RIGHT(i);

    if (left_i >= p_population->n_individuals) {
        /* i is a leaf index */
        return;
    }

    int max_i = i;

    if (individual_compar(p_population->p_individuals[i], p_population->p_individuals[left_i]) < 0) {
        max_i = left_i;
    }

    if (right_i < p_population->n_individuals) {
        if (individual_compar(p_population->p_individuals[i], p_population->p_individuals[left_i]) < 0) {
            max_i = right_i;
        }
    }

    if (i != max_i) {
        individual_t tmp_individual = p_population->p_individuals[i];
        p_population->p_individuals[i] = p_population->p_individuals[max_i];
        p_population->p_individuals[max_i] = tmp_individual;
        population_bubble_down(p_population, max_i);
    }
}

/**
 *
 * @param p_population
 * @param i
 * @return
 */
static individual_t population_extract_individual(population_t *p_population, size_t i) {
    assert(p_population);
    assert(i < p_population->n_individuals);

    individual_t individual = p_population->p_individuals[i];
    p_population->p_individuals[i] = p_population->p_individuals[p_population->n_individuals - 1];

    p_population->n_individuals--;
    population_bubble_down(p_population, i);

    /* update min total weight if needed */
    if (individual.total_weight == p_population->min_total_weight) {
        if (p_population->n_individuals > 0) {
            individual_t min_total_weight_individual = population_get_min_total_weight_individual(p_population);
            p_population->min_total_weight = min_total_weight_individual.total_weight;
        } else {
            p_population->min_total_weight = 0;
        }
    }

    /* update max total weight if needed */
    if (p_population->n_individuals > 0) {
        individual_t max_total_weight_individual = population_get_max_total_weight_individual(p_population);
        p_population->max_total_weight = max_total_weight_individual.total_weight;
    } else {
        p_population->max_total_weight = 0;
    }

    return (individual);
}

/**
 *
 * @param p_population
 * @return
 */
individual_t population_extract_rand_individual(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    size_t i = rand() % p_population->n_individuals;
    return (population_extract_individual(p_population, i));
}

/**
 *
 * @param p_population
 * @return
 */
individual_t population_extract_max_total_weight_individual(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    return (population_extract_individual(p_population, 0));
}

/**
 *
 * @param p_population
 * @return
 */
individual_t population_extract_min_total_weight_individual(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    int min_i = p_population->n_individuals - 1;
    int i = min_i;
    weight_t min_weight = p_population->p_individuals[min_i].total_weight;
    while (POPULATION_LEFT(i) > p_population->n_individuals && POPULATION_RIGHT(i) > p_population->n_individuals) {
        if (p_population->p_individuals[min_i].total_weight < min_weight) {
            min_i = i;
            min_weight = p_population->p_individuals[min_i].total_weight;
        }
        i--;
    }

    return (population_extract_individual(p_population, min_i));
}

/**
 *
 * @param p_population
 * @return
 */
individual_t population_get_max_total_weight_individual(const population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    return (p_population->p_individuals[0]);
}

/**
 *
 * @param p_population
 * @return
 */
individual_t population_get_min_total_weight_individual(const population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    int i = p_population->n_individuals - 1;
    individual_t min_weight_individual = p_population->p_individuals[i];
    weight_t min_weight = min_weight_individual.total_weight;
    while (POPULATION_LEFT(i) > p_population->n_individuals && POPULATION_RIGHT(i) > p_population->n_individuals) {
        individual_t individual = p_population->p_individuals[i];
        if (individual.total_weight < min_weight) {
            min_weight_individual = individual;
            min_weight = min_weight_individual.total_weight;
        }
        i--;
    }

    return (min_weight_individual);
}

/**
 *
 * @param p_population
 */
void population_statistics_print(const population_t *p_population) {
    assert(p_population);

    size_t n_individuals = p_population->n_individuals;

    /* allocate the statistics array */
    weight_t *p_weight = (weight_t*) calloc(n_individuals, sizeof(weight_t));
    if (!p_weight) {
        fprintf(stderr, "population_statistics_print. memory alloc for %lu individuals.\n", n_individuals);
        exit(EXIT_FAILURE);
    }

    /* fill the statistics array */
    for (int i = 0; i < n_individuals; i++) {
        p_weight[i] = p_population->p_individuals[i].total_weight;
    }

    /* reporting */
    statistics_t statistics = statistics_mk(p_weight, n_individuals);
    statistics_print(statistics);
    for (int i = 0; i < n_individuals; i++) {
        if (i % 5 == 0) {
            fprintf(stdout, "\n");
        } else {
            fprintf(stdout, "\t");
        }
        fprintf(stdout, "%u", p_weight[i]);
    }
    fprintf(stdout, "\n");

    /* clean and release the statistics array */
    memset(p_weight, 0x0, n_individuals * sizeof(weight_t));
    free(p_weight);
}