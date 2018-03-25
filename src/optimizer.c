#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "configuration.h"
#include "individual.h"
#include "optimizer.h"
#include "population.h"
#include "probability.h"
#include "random.h"
#include "utils.h"

typedef enum {
    STAGE1,
    STAGE2,
} stage_t;

typedef struct {
    graph_t *p_graph;
    configuration_t configuration;
    population_t *p_population;
    population_t *p_population1;
    population_t *p_population2;
    weight_t min_total_weight;
    weight_t max_total_weight;
    pool_t *p_pool;
    stage_t stage;
} optimizer_t;

/**
 *
 * @param configuration
 */
static void optimizer_check_configuration(configuration_t configuration) {
    if (configuration.n_individuals == 0) {
        fprintf(stderr, "optimizer_check_configuration. zero individuals.\n");
        exit(EXIT_FAILURE);
    }

    probability_t probability = 0;

    probability = configuration.configuration_union.event_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event union probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_intersection.event_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event intersection probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_crossing.event_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event crossing probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_alter.event_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event drop out probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_alter.alter_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad drop out probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability_t p_total = 0.0;
    p_total += configuration.configuration_union.event_probability;
    p_total += configuration.configuration_intersection.event_probability;
    p_total += configuration.configuration_crossing.event_probability;
    p_total += configuration.configuration_alter.event_probability;
    p_total += configuration.configuration_renew.event_probability;

    if (!probability_check(p_total)) {
        fprintf(stderr, "optimizer_check_configuration. bad total probability %f\n.", p_total);
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * @param configuration
 * @return
 */
static optimizer_t *optimizer_alloc(graph_t *p_graph, configuration_t configuration) {
    optimizer_check_configuration(configuration);

    optimizer_t *p_optimizer = (optimizer_t *) malloc(sizeof(optimizer_t));
    if (!p_optimizer) {
        fprintf(stderr, "optimizer_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    p_optimizer->p_graph = p_graph;
    p_optimizer->configuration = configuration;
    p_optimizer->min_total_weight = 0;
    p_optimizer->max_total_weight = 0;
    p_optimizer->p_pool = pool_alloc(p_graph->n_nodes);
    p_optimizer->p_population = NULL;
    p_optimizer->p_population1 = population_alloc(p_optimizer->p_pool, configuration.n_individuals);
    p_optimizer->p_population2 = population_alloc(p_optimizer->p_pool, configuration.n_individuals);
    p_optimizer->stage = STAGE1;

    return (p_optimizer);
}

/**
 *
 * @param p_optimizer
 */
static void optimizer_release(optimizer_t *p_optimizer) {
#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "releasing optimizer...\n");
#endif /* ST_HEURISTIC_RELEASE */

    if (p_optimizer) {
        /* release the population */
        population_release(p_optimizer->p_pool, p_optimizer->p_population);

        /* release the pool */
        pool_release(p_optimizer->p_pool);

        /* release the optimizer itself */
        memset(p_optimizer, 0x0, sizeof(optimizer_t));
        free(p_optimizer);
    }
}

#ifndef ST_HEURISTIC_RELEASE
/**
 *
 * @param flag
 */
void optimizer_print_acceptance(int flag) {
    if (flag) {
        fprintf(stdout, "accepted\n");
    } else {
        fprintf(stdout, "rejected\n");
    }
}
#endif /* ST_HEURISTIC_RELEASE */

/**
 *
 * @param p_optimizer
 * @param step
 */
static void optimizer_step_union(optimizer_t *p_optimizer, int step) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d union.\n", step);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random total weighto individuals and construct the union individual */
    pool_t *p_pool = p_optimizer->p_pool;
    graph_t *p_graph = p_optimizer->p_graph;
    individual_t individual1 = population_extract_rand_individual(p_optimizer->p_population);
    individual_t individual2 = population_extract_rand_individual(p_optimizer->p_population);
    individual_t new_individual = individual_union(p_pool, p_graph, individual1, individual2);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%lu).\n", individual1.total_weight);
    fprintf(stdout, "extract individual (total weight=%lu).\n", individual2.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* insert the 3 individuals (over-weighted individuals are drooped out) */
    int insert1 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual1);
    int insert2 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual2);
    int insert_new = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, new_individual);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%lu) ... ", individual1.total_weight);
    optimizer_print_acceptance(insert1);
    fprintf(stdout, "insert back individual (total weight=%lu) ... ", individual2.total_weight);
    optimizer_print_acceptance(insert2);
    fprintf(stdout, "insert new individual (total weight=%lu) ... ", new_individual.total_weight);
    optimizer_print_acceptance(insert_new);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param p_optimizer
 * @param step
 */
static void optimizer_step_intersection(optimizer_t *p_optimizer, int step) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d intersection.\n", step);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random total weighto individuals and construct the intersection individual */
    pool_t *p_pool = p_optimizer->p_pool;
    graph_t *p_graph = p_optimizer->p_graph;

    individual_t individual1 = population_extract_rand_individual(p_optimizer->p_population);
    individual_t individual2 = population_extract_rand_individual(p_optimizer->p_population);
    individual_t new_individual = individual_intersection(p_pool, p_graph, individual1, individual2);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%lu).\n", individual1.total_weight);
    fprintf(stdout, "extract individual (total weight=%lu).\n", individual2.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* insert the 3 individuals (over-weighted individuals are drooped out) */
    int insert1 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual1);
    int insert2 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual2);
    int insert_new = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, new_individual);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%lu) ... ", individual1.total_weight);
    optimizer_print_acceptance(insert1);
    fprintf(stdout, "insert back individual (total weight=%lu) ... ", individual2.total_weight);
    optimizer_print_acceptance(insert2);
    fprintf(stdout, "insert new individual (total weight=%lu) ... ", new_individual.total_weight);
    optimizer_print_acceptance(insert_new);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param p_optimizer
 * @param step
 */
static void optimizer_step_crossing(optimizer_t *p_optimizer, int step) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d crossing.\n", step);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random total weight individuals and construct the total weight crossing individuals */
    individual_t individual1 = population_extract_rand_individual(p_optimizer->p_population);
    individual_t individual2 = population_extract_rand_individual(p_optimizer->p_population);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%lu).\n", individual1.total_weight);
    fprintf(stdout, "extract individual (total weight=%lu).\n", individual2.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    pool_t *p_pool = p_optimizer->p_pool;
    graph_t *p_graph = p_optimizer->p_graph;
    individuals_t new_individuals = individual_crossing(p_pool, p_graph, individual1, individual2);

    /* insert the 4 individuals (over-weighted individuals are drooped out) */
    int insert1 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual1);
    int insert2 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual2);
    int insert_new1 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, new_individuals.individual1);
    int insert_new2 = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, new_individuals.individual2);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%lu) ... ", individual1.total_weight);
    optimizer_print_acceptance(insert1);
    fprintf(stdout, "insert back individual (total weight=%lu) ... ", individual2.total_weight);
    optimizer_print_acceptance(insert2);
    fprintf(stdout, "insert new individual (total weight=%lu) ... ", new_individuals.individual1.total_weight);
    optimizer_print_acceptance(insert_new1);
    fprintf(stdout, "insert new individual (total weight=%lu) ... ", new_individuals.individual2.total_weight);
    optimizer_print_acceptance(insert_new2);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param p_optimizer
 * @param step
 */
static void optimizer_step_alter(optimizer_t *p_optimizer, int step) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d drop out.\n", step);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random one individual and construct the total weight dropped out individuals */
    individual_t individual = population_extract_rand_individual(p_optimizer->p_population);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%lu).\n", individual.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    pool_t *p_pool = p_optimizer->p_pool;
    graph_t *p_graph = p_optimizer->p_graph;
    probability_t probability = p_optimizer->configuration.configuration_alter.alter_probability;

    individual_t new_individual = individual_alter(p_pool, p_graph, individual, probability);

    /* insert the 2 individuals (over-weighted individuals are drooped out) */
    int insert = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual);
    int insert_new = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, new_individual);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%lu) ... ", individual.total_weight);
    optimizer_print_acceptance(insert);
    fprintf(stdout, "insert new individual (total weight=%lu) ... ", new_individual.total_weight);
    optimizer_print_acceptance(insert_new);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param p_optimizer
 * @param step
 */
static void optimizer_step_renew(optimizer_t *p_optimizer, int step) {
    assert(p_optimizer);

    double probability = p_optimizer->configuration.configuration_renew.probability;
    size_t n_renewed_individuals = (size_t)(probability * p_optimizer->p_population->n_individuals);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d renew %lu individuals.\n", step, n_renewed_individuals);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    pool_t *p_pool = p_optimizer->p_pool;
    graph_t *p_graph = p_optimizer->p_graph;

    /* remove n_renewed_individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t individual = population_extract_max_total_weight_individual(p_optimizer->p_population);

#ifndef ST_HEURISTIC_RELEASE
        fprintf(stdout, "extract individual (total weight=%lu).\n", individual.total_weight);
        fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

        individual_cleanup(p_pool, individual);
    }

    /* insert n_renewed_individuals new individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t individual = individual_mk(p_pool, p_graph);
        int insert = population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population, individual);

#ifndef ST_HEURISTIC_RELEASE
        fprintf(stdout, "insert new individual (total weight=%lu) ... ", individual.total_weight);
        optimizer_print_acceptance(insert);
        fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */
    }


#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param optimizer
 */
static void optimizer_step(optimizer_t *p_optimizer, int step) {
    assert(p_optimizer);

    configuration_t configuration = p_optimizer->configuration;

    if (step == 1 || (step % p_optimizer->configuration.configuration_renew.tick) == 0) {
#ifndef ST_HEURISTIC_RELEASE
        fprintf(stdout, "#%05d stage=%d.\n", step, p_optimizer->stage);
        fflush(stdout);
        fprintf(stdout, "population 1\n");
        population_statistics_fprint(stdout, p_optimizer->p_population1);
        fprintf(stdout, "population 2\n");
        population_statistics_fprint(stdout, p_optimizer->p_population2);
        fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

        if (step == 1) {
            for (int i = 0; i < p_optimizer->configuration.n_individuals; i++) {
                individual_t individual = individual_mk(p_optimizer->p_pool, p_optimizer->p_graph);
                population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population2, individual);
            }
        }

        if (p_optimizer->stage == STAGE1) {
            /* transfer best individuals from population 1 to population 2 */
            while (p_optimizer->p_population1->n_individuals) {
                individual_t individual = population_extract_max_total_weight_individual(p_optimizer->p_population1);
                population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population2, individual);
            }

            /* renew population 1 */
            for (int i = 0; i < p_optimizer->configuration.n_individuals; i++) {
                individual_t individual = individual_mk(p_optimizer->p_pool, p_optimizer->p_graph);
                population_insert_individual(p_optimizer->p_pool, p_optimizer->p_population1, individual);
            }

            /* now work on population 1 */
            p_optimizer->p_population = p_optimizer->p_population1;

            /* next stage is STAGE2 */
            p_optimizer->stage = STAGE2;
        } else {
            /* now work on population 2 */
            p_optimizer->p_population = p_optimizer->p_population2;

            /* next stage is STAGE1 */
            p_optimizer->stage = STAGE1;
        }
    }

    probability_t p = probability_rand();

    /* union event */
    probability_t threshold = configuration.configuration_union.event_probability;
    if (p <= threshold) {
        optimizer_step_union(p_optimizer, step);
        return;
    }

    /* intersection event */
    threshold += configuration.configuration_intersection.event_probability;
    if (p <= threshold) {
        optimizer_step_intersection(p_optimizer, step);
        return;
    }

    /* crossing event */
    threshold += configuration.configuration_crossing.event_probability;
    if (p <= threshold) {
        optimizer_step_crossing(p_optimizer, step);
        return;
    }

    /* drop out event */
    threshold += configuration.configuration_alter.event_probability;
    if (p <= threshold) {
        optimizer_step_alter(p_optimizer, step);
        return;
    }

    /* renew event */
    threshold += configuration.configuration_renew.event_probability;
    if (p <= threshold) {
        optimizer_step_renew(p_optimizer, step);
        return;
    }

    /* still here !? no operation is going to be applied. */
#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "no operation applied (total probability does not sum to 1.0).\n");
    fprintf(stdout, "unexpected event, this is probably a bug.\n");
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param configuration
 */
void optimizer_run(graph_t *p_graph, configuration_t configuration) {

    /* init */
    optimizer_t *p_optimizer = optimizer_alloc(p_graph, configuration);

    if (p_optimizer->configuration.n_steps > 0) {
        for (int i = 1; i <= p_optimizer->configuration.n_steps; i++) {
            optimizer_step(p_optimizer, i);
        }
    } else {
        int step = 1;
        while (1) {
            optimizer_step(p_optimizer, step);
            step++;
        }
    }

    optimizer_release(p_optimizer);
}