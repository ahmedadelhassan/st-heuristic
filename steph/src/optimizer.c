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
 * @param p_optimizer
 */
static void optimizer_release(optimizer_t *p_optimizer) {
    if (p_optimizer) {
        if (p_optimizer->individuals) {
            for (int i = 0; i < p_optimizer->configuration.n_individuals; i++) {
                individual_release(p_optimizer->individuals[i]);
            }
            memset(p_optimizer->individuals, 0x0, opt.configuration.n_individuals * sizeof(individuals_t *));
            free(p_optimizer->individuals);
        }

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
        p_optimizer->indivduals[i] = individual_mk(p_optimizer->configuration.graph);
    }
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_union(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    fprintf(stdout, "#%05d union.\n", epoch);

    graph_t *pg = optimizer->configuration.graph;
    individual_t *p_individual1 = (individual_t *) population_extract_rand(p_optimizer->individuals);
    individual_t *p_individual2 = (individual_t *) population_extract_rand(p_optimizer->individuals);
    individual_t *p_union_individual = individual_union(p_g, p_individual1, p_individual2);

    population_insert(p_optimizer, p_individual1);
    population_insert(p_optimizer, p_individual2);
    population_insert(p_optimizer, p_union_individual);

    population_statistics_print(p_optimizer->p_population);
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_intersection(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    fprintf(stdout, "#%05d intersection.\n", epoch);

    graph_t *pg = p_optimizer->configuration.graph;
    individual_t *p_individual1 = (individual_t *) population_extract_rand(p_optimizer->individuals);
    individual_t *p_individual2 = (individual_t *) population_extract_rand(p_optimizer->individuals);
    individual_t *p_union_individual = individual_intersection(p_g, p_individual1, p_individual2);

    population_insert(optimizer, p_individual1);
    population_insert(optimizer, p_individual2);
    population_insert(optimizer, p_union_individual);

    population_statistics_print(p_optimizer->p_population);
}

/**
 *
 * @param p_optimizer
 * @param epoch
 */
static void optimizer_step_crossing(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    fprintf(stdout, "#%05d crossing.\n", epoch);

    graph_t *pg = optimizer->configuration.graph;
    individual_t *p_individual1 = (individual_t *) population_extract_rand(optimizer->individuals);
    individual_t *p_individual2 = (individual_t *) population_extract_rand(optimizer->individuals);
    probability_t p = p_optimizer->configuration.configuration_crossing.drop_out_probability;
    pair_t crossed_individuals = individual_crossing(p_g, p_individual1, p_individual2, p);

    population_insert(p_optimizer, p_individual1);
    population_insert(p_optimizer, p_individual2);
    population_insert(p_optimizer, (individual_t *) (crossed_individuals.data1));
    population_insert(p_optimizer, (individual_t *) (crossed_individuals.data2));

    optimizer_statistics_print(p_optimizer);
}

/**
 *
 * @param optimizer
 */
static void optimizer_step_drop_out(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    fprintf(stdout, "#%05d drop out.\n", epoch);

    graph_t *pg = p_optimizer->configuration.graph;
    individual_t *p_individual1 = (individual_t *) population_extract_rand(p_optimizer->individuals);
    individual_t *p_individual2 = (individual_t *) population_extract_rand(p_optimizer->individuals);
    individual_t *p_dropped_out_individual = individual_intersection(p_g, p_individual1, p_individual2);

    population_insert(optimizer, p_individual1);
    population_insert(optimizer, p_individual2);
    population_insert(optimizer, p_dropped_out_individual);

    population_statistics_print(p_optimizer->p_population);
}

/**
 *
 * @param p_optimizer
 * @param epoch
 */
static void optimizer_step_renew(optimizer_t *p_optimizer, int epoch) {
    assert(p_optimizer);

    double percentage = p_optimizer->configuration.configuration_renew.percentage;
    size_t n_renewed_individuals = (size_t) (percentage * p_optimizer->p_population->n_individuals);

    fprintf(stdout, "#%05d renew %u individuals.\n", epoch, n_renewed_individuals);

    graph_t *pg = p_optimizer->configuration.graph;
    /* remove n_renewed_individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t *p_individual = population_extract_max(p_optimizer->population);
        individual_release(p_individual);
    }

    /* insert n_renewed_individuals new individuals */
    for (int i = 0; i < n_renewed_individuals; i++) {
        individual_t *p_individual = individual_mk(p_g);
        population_insert(p_optimizer->population, p_individual);
    }

    population_statistics_print(p_optimizer->p_population);
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
        optimizer_step_union(p_optimizer, epoch);
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
        optimizer_step_crossing(p_optimizer, epoch);
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