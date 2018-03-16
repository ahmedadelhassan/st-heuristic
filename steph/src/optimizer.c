#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuration.h"
#include "individual.h"
#include "optimizer.h"
#include "population.h"
#include "probability.h"
#include "random.h"
#include "utils.h"

/**
 *
 * @param configuration
 */
static void optimizer_check_configuration(configuration_t configuration) {
    if (!configuration.graph) {
        fprintf(stderr, "optimizer_check_configuration. NULL graph.\n");
        exit(EXIT_FAILURE);
    }

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

    probability = configuration.configuration_crossing.crossing_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad crossing probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_drop_out.event_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event drop out probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_drop_out.drop_out_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad drop out probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_renew.event_probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event renew probability %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability = configuration.configuration_renew.percentage;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad renew percentage %f\n.", probability);
        exit(EXIT_FAILURE);
    }

    probability_t p_total = 0.0;
    p_total += configuration.configuration_union.event_probability;
    p_total += configuration.configuration_intersection.event_probability;
    p_total += configuration.configuration_crossing.event_probability;
    p_total += configuration.configuration_drop_out.event_probability;
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
static optimizer_t *optimizer_alloc(configuration_t configuration) {
    optimizer_check_configuration(configuration);

    optimizer_t *p_optimizer = (optimizer_t *) malloc(sizeof(optimizer_t));
    if (!p_optimizer) {
        fprintf(stderr, "optimizer_init. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    p_optimizer->configuration = configuration;
    p_optimizer->min_total_weight = 0;
    p_optimizer->max_total_weight = 0;
    p_optimizer->p_population = population_alloc(configuration.n_individuals);

    return (p_optimizer);
}

/**
 *
 * @param p_optimizer
 */
static void optimizer_release(optimizer_t *p_optimizer) {
    if (p_optimizer) {
        /* release the population */
        population_release(p_optimizer->p_population);

        /* release the optimizer itsef */
        memset(p_optimizer, 0x0, sizeof(optimizer_t));
        free(p_optimizer);
    }
}

/**
 *
 * @param p_optimizer
 * @return
 */
static void optimizer_init(optimizer_t *p_optimizer) {
    assert(p_optimizer);

    for (int i = 0; i < p_optimizer->configuration.n_individuals; i++) {
        individual_t *p_individual = individual_mk(p_optimizer->configuration.graph);
        population_insert_individual(p_optimizer->p_population, p_individual);
    }
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_union(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d union.\n", epoch);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random two individuals and construct the union individual */
    graph_t *p_g = p_optimizer->configuration.graph;
    individual_t *p_individual1 = (individual_t *) population_extract_rand_individual(p_optimizer->p_population);
    individual_t *p_individual2 = (individual_t *) population_extract_rand_individual(p_optimizer->p_population);
    individual_t *p_union_individual = individual_union(p_g, p_individual1, p_individual2);

    /* insert the 3 individuals (over-weighted individuals are drooped out) */
    population_insert_individual(p_optimizer->p_population, p_individual1);
    population_insert_individual(p_optimizer->p_population, p_individual2);
    population_insert_individual(p_optimizer->p_population, p_union_individual);

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_print(p_optimizer->p_population);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_intersection(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d intersection.\n", epoch);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random two individuals and construct the intersection individual */
    graph_t *p_g = p_optimizer->configuration.graph;
    individual_t *p_individual1 = (individual_t *) population_extract_rand_individual(p_optimizer->p_population);
    individual_t *p_individual2 = (individual_t *) population_extract_rand_individual(p_optimizer->p_population);
    individual_t *p_intersection_individual = individual_intersection(p_g, p_individual1, p_individual2);

    /* insert the 3 individuals (over-weighted individuals are drooped out) */
    population_insert_individual(p_optimizer->p_population, p_individual1);
    population_insert_individual(p_optimizer->p_population, p_individual2);
    population_insert_individual(p_optimizer->p_population, p_intersection_individual);

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_print(p_optimizer->p_population);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param p_optimizer
 * @param epoch
 */
static void optimizer_step_crossing(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d crossing.\n", epoch);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random two individuals and construct the two crossing individuals */
    individual_t *p_individual1 = (individual_t *) population_extract_rand_individual(p_optimizer->p_population);
    individual_t *p_individual2 = (individual_t *) population_extract_rand_individual(p_optimizer->p_population);

    graph_t *p_g = p_optimizer->configuration.graph;
    probability_t probability = p_optimizer->configuration.configuration_crossing.crossing_probability;
    pair_t crossed_individuals = individual_crossing(p_g, p_individual1, p_individual2, probability);

    /* insert the 4 individuals (over-weighted individuals are drooped out) */
    population_insert_individual(p_optimizer->p_population, p_individual1);
    population_insert_individual(p_optimizer->p_population, p_individual2);
    population_insert_individual(p_optimizer->p_population, (individual_t *) (crossed_individuals.data1));
    population_insert_individual(p_optimizer->p_population, (individual_t *) (crossed_individuals.data2));

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_print(p_optimizer->p_population);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_drop_out(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d drop out.\n", epoch);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random one individual and construct the two dropped out individuals */
    individual_t *p_individual = (individual_t *) population_extract_rand_individual(p_optimizer->p_population);

    graph_t *p_g = p_optimizer->configuration.graph;
    probability_t probability = p_optimizer->configuration.configuration_crossing.crossing_probability;
    individual_t *p_dropped_out_individual = individual_drop_out(p_g, p_individual, probability);

    /* insert the 2 individuals (over-weighted individuals are drooped out) */
    population_insert_individual(p_optimizer->p_population, p_individual);
    population_insert_individual(p_optimizer->p_population, p_dropped_out_individual);

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_print(p_optimizer->p_population);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param p_optimizer
 * @param epoch
 */
static void optimizer_step_renew(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    double percentage = p_optimizer->configuration.configuration_renew.percentage;
    size_t n_renewed_individuals = (size_t)(percentage * p_optimizer->p_population->n_individuals);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d renew %lu individuals.\n", epoch, n_renewed_individuals);
#endif /* ST_HEURISTIC_RELEASE */

    graph_t *p_g = p_optimizer->configuration.graph;
    /* remove n_renewed_individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t *p_individual = population_extract_max_total_weight_individual(p_optimizer->p_population);
        individual_release(p_individual);
    }

    /* insert n_renewed_individuals new individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t *p_individual = individual_mk(p_g);
        population_insert_individual(p_optimizer->p_population, p_individual);
    }


#ifndef ST_HEURISTIC_RELEASE
    population_statistics_print(p_optimizer->p_population);
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param optimizer
 */
static void optimizer_step(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    configuration_t configuration = p_optimizer->configuration;
    probability_t p = probability_rand();

    /* union event */
    probability_t threshold = configuration.configuration_union.event_probability;
    if (p <= threshold) {
        optimizer_step_union(p_optimizer, epoch);
        return;
    }

    /* intersection event */
    threshold += configuration.configuration_intersection.event_probability;
    if (p <= threshold) {
        optimizer_step_intersection(p_optimizer, epoch);
        return;
    }

    /* crossing event */
    threshold += configuration.configuration_crossing.event_probability;
    if (p <= threshold) {
        optimizer_step_crossing(p_optimizer, epoch);
        return;
    }

    /* drop out event */
    threshold += configuration.configuration_drop_out.event_probability;
    if (p <= threshold) {
        optimizer_step_drop_out(p_optimizer, epoch);
        return;
    }

    /* renew event */
    threshold += configuration.configuration_renew.event_probability;
    if (p <= threshold) {
        optimizer_step_renew(p_optimizer, epoch);
        return;
    }

    /* still here !? no operation is going to be applied. */
    fprintf(stdout, "no operation applied (total probability does not sum to 1.0).\n");
    fprintf(stdout, "unexpected event, this is probably a bug.\n");
}

/**
 *
 * @param configuration
 */
void optimizer_run(configuration_t configuration) {

    /* init */
    optimizer_t *p_optimizer = optimizer_alloc(configuration);
    optimizer_init(p_optimizer);

    if (p_optimizer->configuration.n_epochs > 0) {
        for (int i = 0; i < p_optimizer->configuration.n_epochs; i++) {
            optimizer_step(p_optimizer, i);
        }
    } else {
        int i = 1;
        while (1) {
            optimizer_step(p_optimizer, i);
            i++;
        }
    }

    optimizer_release(p_optimizer);
}