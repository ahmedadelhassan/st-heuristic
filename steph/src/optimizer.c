#include "individual.h"
#include "optimizer->h"
#include "probability.h"
#include "random.h"
#include "utils.h

/**
 *
 * @param configuration
 */
static void optimizer_check_configuration(configurarion_t configuration) {
    if (!configuration.graph) {
        fprintf(stderr, "optimizer_check_configuration. NULL graph.\n");
        exit(EXIT_FAILURE);
    }

    if (configuration.n_individuals == 0) {
        fprintf(stderr, "optimizer_check_configuration. zero individuals.\n");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_union.event_probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event union probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_intersection.event_probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event intersection probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_crossing.event_probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event crossing probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_crossing.crossing_probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad crossing probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_drop_out.event_probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event drop out probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_crossing.drop_out_probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad drop out probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_renew.event_probability)) {
        fprintf(stderr, "optimizer_check_configuration. bad event renew probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if (!probability_check(configuration.configuration_renew.percentage)) {
        fprintf(stderr, "optimizer_check_configuration. bad renew percentage %f\n.");
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
static optimizer_t *optimizer_alloc(configurarion_t configuration) {
    optimizer_check_configuration(configuration);

    optimizer_t *optimizer = (optimizer_t *) malloc(sizeof(optimizer_t));
    if (!optimizer) {
        fprintf(stderr, "optimizer_init. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    optimizer->configurartion = configuration;
    optimizer->best_total_weight = 0;
    optimizer->worst_total_weight = 0;
    optimizer->individuals = heap_alloc(configuration.n_individuals);

    return (optimizer);
}

/**
 *
 * @param optimizer
 */
static void optimizer_release(optimizer_t *optimizer) {
    if (optimizer) {
        if (optimizer->individuals) {
            for (int i = 0; i < optimizer->configuration.n_individuals; i++) {
                individual_release(optimizer->individuals[i]);
            }
            memset(optimizer->individuals, 0x0, opt.configuration.n_individuals * sizeof(individuals_t * ));
            free(optimizer->individuals);
        }

        /* release the optimizer itsef */
        memset(optimizer, 0x0, sizeof(optimizer_t));
        free(optimizer);
    }
}

/**
 *
 * @param optimizer
 * @return
 */
static void optimizer_init(optimizer_t *optimizer) {
    assert(optimizer);

    for (int i = 0; i < optimizer->configuration.n_individuals; i++) {
        optimizer->indivduals[i] = individual_mk(optimizer->configuration.graph);
    }
}

/**
 *
 * @param optimizer
 */
static void heap_extract_rand(optimizer_t *optimizer) {
    assert(optimizer);

    individual_t *individual = heap_extract_rand(optimizer->individuals);

    if (heap_size(optimizer->individuals) > 0) {
        individual_t *max_individual = heap_max(optimizer->individuals);
        optimizer->max_total_weight = max_individual->total_weight;
    } else {
        optimizer->min_total_weight = 0;
        optimizer->max_total_weight = 0;
    }

    return (individual);
}

/**
 *
 * @param optimizer
 * @param individuals
 */
static void optimizer_rank_3(optimizer_t *optimizer, individual_t *individuals[3]) {
    assert(optimizer);

    /* first individual */
    individual_t individual0 = individuals[0];
    weight_t tw0 = individual[0]->total_weight;

    /* second individual */
    individual_t individual1 = individuals[1];
    weight_t tw1 = individual[1]->total_weight;

    /* third individual */
    individual_t individual2 = individuals[2];
    weight_t tw2 = individual[2]->total_weight;

    /* rank individuals */
    if (tw0 < MIN(tw1, tw2)) {
        if (tw1 <= tw2) {
            individuals[0] = individuals0;
            individuals[1] = individuals1;
            individuals[2] = individuals2;
        } else {
            individuals[0] = individuals0;
            individuals[1] = individuals2;
            individuals[2] = individuals1;
        }
    } else {
        if (tw1 < MIN(tw0, tw2)) {
            if (tw0 <= tw2) {
                individuals[0] = individuals1;
                individuals[1] = individuals0;
                individuals[2] = individuals2;
            } else {
                individuals[0] = individuals1;
                individuals[1] = individuals2;
                individuals[2] = individuals0;
            }
        } else {
            if (tw0 <= tw1) {
                individuals[0] = individuals2;
                individuals[1] = individuals0;
                individuals[2] = individuals1;
            } else {
                individuals[0] = individuals2;
                individuals[1] = individuals1;
                individuals[2] = individuals0;
            }
        }
    }
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_union(optimizer_t *optimizer) {
    assert(optimizer);

    individual_t *individual1 = (individual_t *) optimizer_heap_extract_rand(optimizer->individuals);
    individual_t *individual2 = (individual_t *) optimizer_heap_extract_rand(optimizer->individuals);
    individual_t *union_individual = individual_union(optimizer->configuration.graph, individual1, individual2);

    individual_t *individuals[3] = {individual1, individual2, union_individual};
    optimizer_rank_3(optimizer, individuals);

    population_insert(optimizer, individuals[0]);
    population_insert(optimizer, individuals[1]);
    population_insert(optimizer, individuals[2]);
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_intersection(optimizer_t *optimizer) {
    assert(optimizer);

    individual_t *individual1 = (individual_t *) optimizer_heap_extract_rand(optimizer->individuals);
    individual_t *individual2 = (individual_t *) optimizer_heap_extract_rand(optimizer->individuals);
    individual_t *intersection_individual = individual_intersection(optimizer->configuration.graph, individual1,
                                                                    individual2);

    individual_t *individuals[3] = {individual1, individual2, intersection_individual};
    optimizer_rank_3(optimizer, individuals);

    optimizer_heap_insert(optimizer, individuals[0]);
    optimizer_heap_insert(optimizer, individuals[1]);
    optimizer_heap_insert(optimizer, individuals[2]);
}

/**
 *
 * @param optimizer
 */
static void optimizer_step(optimizer_t *optimizer) {
    assert(optimizer);

    configuration_t configuration = optimizer->configuration;
    probability_t p = probability_rand();

    /* union event */
    probability_t threshold = configuration.configuration_union.event_probability;
    if (p <= threshold) {
        optimizer_step_union(optimizer);
        return;
    }

    /* intersection event */
    threshold += configuration.configuration_intersection.event_probability;
    if (p <= threshold) {
        optimizer_step_union(optimizer);
        return;
    }

    /* crossing event */
    threshold += configuration.configuration_crossing.event_probability;
    if (p <= threshold) {
        optimizer_step_crossing(optimizer);
        return;
    }

    /* drop out event */
    threshold += configuration.configuration_drop_out.event_probability;
    if (p <= threshold) {
        optimizer_step_crossing(optimizer);
        return;
    }

    /* renew event */
    threshold += configuration.configuration_renew.event_probability;
    if (p <= threshold) {
        optimizer_step_crossing(optimizer);
        return
    }

    /* still here !? no operation is going to be applied. */
    fprintf(stdout, "no operation applied (total probability does not sum to 1.0).\n");
}

/**
 *
 * @param configuration
 */
void optimizer_run(configuration_t configuration) {

    /* init */
    optimizer_t *optimizer = optimizer_alloc(configuration);
    optimizer_init(optimizer);

    if (optimizer->configuration.n_epochs > 0) {
        for (int i = 0; i < optimizer->configuration.n_epochs; i++) {
            optimizer_step(optimizer);
        }
    } else {
        while (1) {
            optimizer_step(optimizer);
        }
    }


}