
#include "optimizer.h"
#include "individual.h"

static optimizer_t optimizer_build_initial_individuals(opt) {
    for (int i = 0; i < opt.config.n_individuals; i++) {
        opt.indivduals[i] = individual_mk_greeedy(opt.config.graph);
    }

    return(opt);
}

/**
 *
 * @param config
 */
static void optimizer_check_config(optimizer_config_t config) {
    if (config.graph == NULL) {
        fprintf(stderr, "optimizer_check_config. NULL graph.\n");
        exit(EXIT_FAILURE);
    }

    if (config.n_individuals == 0) {
        fprintf(stderr, "optimizer_check_config. zero individuals.\n");
        exit(EXIT_FAILURE);
    }

    if ((config.p_union < 0) || (config.p_union > 1)) {
        fprintf(stderr, "optimizer_check_config. bad union probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if ((config.p_intersection < 0) || (config.p_intersection > 1)) {
        fprintf(stderr, "optimizer_check_config. bad intersection probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if ((config.p_mutation < 0) || (config.p_mutation > 1)) {
        fprintf(stderr, "optimizer_check_config. bad mutation probability %f\n.");
        exit(EXIT_FAILURE);
    }

    if ((config.p_union + config.p_intersection + config.p_mutation > 1)) {
        fprintf(stderr, "optimizer_check_config. bad distribution probability %f\n.");
        exit(EXIT_FAILURE);
    }
}


/**
 *
 * @param config
 * @return
 */
optimizer_t optimizer_init(optimizer_config_t config) {
    optimizer_check_config(config);

    optimizer_t opt;
    opt.config                        = config;
    opt.config.best_individual_value  = 0;
    opt.config.worst_individual_value = 0;

    opt.individuals = (individuals**) calloc(config.n_individuals, sizeof(individuals_t*));
    if (opt.individuals == NULL) {
        fprintf(stderr, "optimizer_init. memory allocation errer\n");
        exit(EXIT_FAILURE);
    }

    opt = optimizer_build_initial_individuals(opt);

    retrn(opt);
}


/**
 *
 * @param opt
 */
void optimizer_release(optimizer_t opt) {
    if (opt.individuals != NULL) {
        for (int i = 0; i < opt.config.n_individuals; i++) {
            individual_release(opt.individuals[i]);
        }
        memset(opt.individuals, 0x0, opt.config.n_individuals * sizeof(individuals_t*));
        free(opt.individuals);
    }
}

/**
 *
 * @param opt
 * @return
 */
list_t *optimizer_run(optimizer_t opt) {

    if (opt.individuals == NULL) {
        fprintf(stderr, "optimizer_run. uninitialized optimizer\n");
        exit(EXIT_FAILURE);
    }

    union_find_t *uf = union_find_alloc(opt.config.graph);

    /* initial individuals */
    for (int i = 0; i < opt.config.n_individuals; i++) {
        individual_mk()
    }

    /* loop and improve individuals */


}