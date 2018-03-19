#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "individual.h"
#include "population.h"
#include "statistics.h"
#include "utils.h"
#include "weight.h"

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
        /* cleanup all individuals */
        for (int i = 0; i < p_population->n_individuals; i++) {
            individual_cleanup(p_population->p_individuals[i]);
        }
        memset(p_population->p_individuals, 0x0, p_population->n_individuals * sizeof(individual_t));
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
 * @return
 */
static int population_is_leaf_at_index(population_t *p_population, int i) {
    assert(p_population);
    assert(i < p_population->n_individuals);

    return (POPULATION_LEFT(i) >= p_population->n_individuals);
}

/**
 *
 * @param p_population
 * @return
 */
static void population_update_max_total_weight(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    /* update minimum total weight (it has to be at the root) */
    p_population->max_total_weight = p_population->p_individuals[0].total_weight;
}

/**
 *
 * @param p_population
 * @return
 */
static void population_update_min_total_weight(population_t *p_population) {
    assert(p_population);
    assert(p_population->n_individuals > 0);

    /* search for minimum total weight (it has to be at a leaf) */
    int i = p_population->n_individuals - 1;
    weight_t min_total_weight = p_population->p_individuals[i].total_weight;
    while (population_is_leaf_at_index(p_population, i)) {
        if (p_population->p_individuals[i].total_weight < min_total_weight) {
            min_total_weight = p_population->p_individuals[i].total_weight;
        }
        i--;
    }

    /* update minimum total weight */
    p_population->min_total_weight = min_total_weight;
}

/**
 *
 * @param p_population
 * @param i
 */
static void population_bubble_up(population_t *p_population, int i) {
    assert(p_population);

    individual_t *p_individuals = p_population->p_individuals;

    while (i > 0) {
        int parent_i = POPULATION_PARENT(i);
        weight_t individual_total_weight = p_individuals[i].total_weight;
        weight_t parent_individual_total_weight = p_individuals[parent_i].total_weight;

        if (individual_total_weight <= parent_individual_total_weight) {
            return;
        }

        individual_t p_tmp_individual = p_individuals[i];
        p_individuals[i] = p_individuals[parent_i];
        p_individuals[parent_i] = p_tmp_individual;
        i = parent_i;
    }
}

/**
 *
 * @param p_population
 * @param individual
 * @return
 */
int population_insert_individual(population_t *p_population, individual_t individual) {
    assert(p_population);

    if (p_population->n_individuals == p_population->capacity) {
        if (p_population->max_total_weight > individual.total_weight) {
            individual_t individual_max = population_extract_max_total_weight_individual(p_population);
            individual_cleanup(individual_max);
        } else {
            /* the new individual is not inserted */
            individual_cleanup(individual);
            return (0);
        }
    }

    /* insert the new individual */
    assert(p_population->n_individuals < p_population->capacity);
    p_population->p_individuals[p_population->n_individuals] = individual;
    p_population->n_individuals++;
    population_bubble_up(p_population, p_population->n_individuals - 1);

    if (p_population->max_total_weight < individual.total_weight) {
        population_update_max_total_weight(p_population);
    }

    if (p_population->min_total_weight > individual.total_weight) {
        population_update_min_total_weight(p_population);
    }

    return (1);
}

/**
 *
 * @param p_population
 * @param i
 */
static void population_bubble_down(population_t *p_population, int i) {
    assert(p_population);
    assert(i < p_population->n_individuals);

    if (population_is_leaf_at_index(p_population, i)) {
        return;
    }

    int left_i = POPULATION_LEFT(i);
    int right_i = POPULATION_RIGHT(i);

    /* init */
    int max_i = i;

    weight_t total_weight_i = p_population->p_individuals[i].total_weight;
    weight_t total_weight_left_i = p_population->p_individuals[left_i].total_weight;

    /* test left subtree */
    if (total_weight_i < total_weight_left_i) {
        max_i = left_i;
    }

    /* test right subtree */
    if (right_i < p_population->n_individuals) {
        weight_t total_weight_right_i = p_population->p_individuals[right_i].total_weight;

        if (total_weight_i < total_weight_right_i) {
            max_i = right_i;
        }
    }

    /* swap and bubble down if needed */
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
static individual_t population_extract_individual(population_t *p_population, int i) {
    assert(p_population);
    assert(i < p_population->n_individuals);

    fprintf(stdout, "population_extract_individual at index %d\n", i);
    fprintf(stdout, "p_population->n_individuals=%lu\n", p_population->n_individuals);
    fprintf(stdout, "individual.n_edges=%zu, individual.total_weight=%u\n", p_population->p_individuals[i].n_edges, p_population->p_individuals[i].total_weight);
    fflush(stdout);

    individual_t individual = p_population->p_individuals[i];

    if (p_population->n_individuals == 1) {
        p_population->n_individuals = 0;
        p_population->min_total_weight = 0;
        p_population->max_total_weight = 0;
        return (individual);
    }

    if (p_population->n_individuals - 1 < i) {
        p_population->p_individuals[i] = p_population->p_individuals[p_population->n_individuals - 1];
        p_population->n_individuals--;
        population_bubble_down(p_population, i);
    } else {
        p_population->n_individuals--;
    }

    /* update min total weight if needed */
    if (individual.total_weight == p_population->min_total_weight) {
        population_update_min_total_weight(p_population);
    }

    /* update max total weight if needed */
    if (individual.total_weight == p_population->max_total_weight) {
        population_update_max_total_weight(p_population);
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

    int i = rand() % p_population->n_individuals;
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
 */
void population_statistics_fprint(FILE *f, const population_t *p_population) {
    assert(p_population);

    size_t n_individuals = p_population->n_individuals;

    /* allocate the statistics array */
    weight_t *p_weight = (weight_t *) calloc(n_individuals, sizeof(weight_t));
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
    statistics_fprint(f, statistics);
    //qsort(p_weight, n_individuals, sizeof(weight_t), weight_compar);
    for (int i = 0; i < n_individuals; i++) {
        if (i % 10 == 0) {
            fprintf(f, "\n");
        } else {
            fprintf(f, "\t");
        }
        fprintf(f, "%u", p_weight[i]);
    }
    fprintf(f, "\n\n");

    /* clean and release the statistics array */
    memset(p_weight, 0x0, n_individuals * sizeof(weight_t));
    free(p_weight);
}