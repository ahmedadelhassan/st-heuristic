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

    probability = configuration.configuration_renew.probability;
    if (!probability_check(probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad renew probability %f\n.", probability);
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
#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "releasing optimizer...\n");
#endif /* ST_HEURISTIC_RELEASE */

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

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "optimizer init: creating %lu individuals ...", p_optimizer->configuration.n_individuals);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    for (int i = 0; i < p_optimizer->configuration.n_individuals; i++) {
        individual_t individual = individual_mk(p_optimizer->configuration.graph, NULL);
        population_insert_individual(p_optimizer->p_population, individual);
    }

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, " done\n\n");
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */
}

#ifndef ST_HEURISTIC_RELEASE
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
 * @param optimizer
 */
static void optimizer_step_union(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d union.\n", epoch);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random total weighto individuals and construct the union individual */
    graph_t *p_g = p_optimizer->configuration.graph;
    individual_t individual1 = population_extract_min_total_weight_individual(p_optimizer->p_population);
    individual_t individual2 = population_extract_rand_individual(p_optimizer->p_population);
    individual_t union_individual = individual_union(p_g, individual1, individual2);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%u).\n", individual1.total_weight);
    fprintf(stdout, "extract individual (total weight=%u).\n", individual2.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* insert the 3 individuals (over-weighted individuals are drooped out) */
    int insert1 = population_insert_individual(p_optimizer->p_population, individual1);
    int insert2 = population_insert_individual(p_optimizer->p_population, individual2);
    int insert_union = population_insert_individual(p_optimizer->p_population, union_individual);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%u) ... ", individual1.total_weight);
    optimizer_print_acceptance(insert1);
    fprintf(stdout, "insert back individual (total weight=%u) ... ", individual2.total_weight);
    optimizer_print_acceptance(insert2);
    fprintf(stdout, "insert new individual (total weight=%u) ... ", union_individual.total_weight);
    optimizer_print_acceptance(insert_union);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
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
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random total weighto individuals and construct the intersection individual */
    graph_t *p_g = p_optimizer->configuration.graph;
    individual_t individual1 = population_extract_min_total_weight_individual(p_optimizer->p_population);
    individual_t individual2 = population_extract_rand_individual(p_optimizer->p_population);
    individual_t intersection_individual = individual_intersection(p_g, individual1, individual2);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%u).\n", individual1.total_weight);
    fprintf(stdout, "extract individual (total weight=%u).\n", individual2.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* insert the 3 individuals (over-weighted individuals are drooped out) */
    int insert1 = population_insert_individual(p_optimizer->p_population, individual1);
    int insert2 = population_insert_individual(p_optimizer->p_population, individual2);
    int insert_intersection = population_insert_individual(p_optimizer->p_population, intersection_individual);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%u) ... ", individual1.total_weight);
    optimizer_print_acceptance(insert1);
    fprintf(stdout, "insert back individual (total weight=%u) ... ", individual2.total_weight);
    optimizer_print_acceptance(insert2);
    fprintf(stdout, "insert new individual (total weight=%u) ... ", intersection_individual.total_weight);
    optimizer_print_acceptance(insert_intersection);
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
 * @param epoch
 */
static void optimizer_step_crossing(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d crossing.\n", epoch);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random total weighto individuals and construct the total weighto crossing individuals */
    individual_t individual1 = population_extract_min_total_weight_individual(p_optimizer->p_population);
    individual_t individual2 = population_extract_rand_individual(p_optimizer->p_population);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%u).\n", individual1.total_weight);
    fprintf(stdout, "extract individual (total weight=%u).\n", individual2.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    graph_t *p_g = p_optimizer->configuration.graph;
    probability_t probability = p_optimizer->configuration.configuration_crossing.crossing_probability;
    individuals2_t crossed_individuals = individual_crossing(p_g, individual1, individual2, probability);

    /* insert the 4 individuals (over-weighted individuals are drooped out) */
    int insert1 = population_insert_individual(p_optimizer->p_population, individual1);
    int insert2 = population_insert_individual(p_optimizer->p_population, individual2);
    int cross1 = population_insert_individual(p_optimizer->p_population, crossed_individuals.individual1);
    int cross2 = population_insert_individual(p_optimizer->p_population, crossed_individuals.individual2);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%u) ... ", individual1.total_weight);
    optimizer_print_acceptance(insert1);
    fprintf(stdout, "insert back individual (total weight=%u) ... ", individual2.total_weight);
    optimizer_print_acceptance(insert2);
    fprintf(stdout, "insert new individual (total weight=%u) ... ", crossed_individuals.individual1.total_weight);
    optimizer_print_acceptance(cross1);
    fprintf(stdout, "insert new individual (total weight=%u) ... ", crossed_individuals.individual2.total_weight);
    optimizer_print_acceptance(cross2);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
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
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    /* extract at random one individual and construct the total weight dropped out individuals */
    individual_t individual = population_extract_rand_individual(p_optimizer->p_population);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "extract individual (total weight=%u).\n", individual.total_weight);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    graph_t *p_g = p_optimizer->configuration.graph;
    probability_t probability = p_optimizer->configuration.configuration_crossing.crossing_probability;
    individual_t dropped_out_individual = individual_drop_out(p_g, individual, probability);

    /* insert the 2 individuals (over-weighted individuals are drooped out) */
    int insert = population_insert_individual(p_optimizer->p_population, individual);
    int insert_dropped_out = population_insert_individual(p_optimizer->p_population, dropped_out_individual);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "insert back individual (total weight=%u) ... ", individual.total_weight);
    optimizer_print_acceptance(insert);
    fprintf(stdout, "insert new individual (total weight=%u) ... ", dropped_out_individual.total_weight);
    optimizer_print_acceptance(insert_dropped_out);
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
 * @param epoch
 */
static void optimizer_step_renew(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    double probability = p_optimizer->configuration.configuration_renew.probability;
    size_t n_renewed_individuals = (size_t)(probability * p_optimizer->p_population->n_individuals);

#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "#%05d renew %lu individuals.\n", epoch, n_renewed_individuals);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    graph_t *p_g = p_optimizer->configuration.graph;
    /* remove n_renewed_individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t individual = population_extract_max_total_weight_individual(p_optimizer->p_population);

#ifndef ST_HEURISTIC_RELEASE
        fprintf(stdout, "extract individual (total weight=%u).\n", individual.total_weight);
        fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

        individual_cleanup(individual);
    }

    /* insert n_renewed_individuals new individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t individual = individual_mk(p_g, NULL);
        int insert = population_insert_individual(p_optimizer->p_population, individual);

#ifndef ST_HEURISTIC_RELEASE
        fprintf(stdout, "insert new individual (total weight=%u) ... ", individual.total_weight);
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
#ifndef ST_HEURISTIC_RELEASE
    fprintf(stdout, "no operation applied (total probability does not sum to 1.0).\n");
    fprintf(stdout, "unexpected event, this is probably a bug.\n");
#endif /* ST_HEURISTIC_RELEASE */
}

/**
 *
 * @param configuration
 */
void optimizer_run(configuration_t configuration) {

    /* init */
    optimizer_t *p_optimizer = optimizer_alloc(configuration);
    optimizer_init(p_optimizer);

#ifndef ST_HEURISTIC_RELEASE
    population_statistics_fprint(stdout, p_optimizer->p_population);
    fflush(stdout);
#endif /* ST_HEURISTIC_RELEASE */

    if (p_optimizer->configuration.n_epochs > 0) {
        for (int i = 1; i <= p_optimizer->configuration.n_epochs; i++) {
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