#include <stdio.h>

#include "configuration.h"

/**
 *
 * @param configuration
 */
void configuration_print(configuration_t configuration) {
    /* general */
    fprintf(stdout, "configuration: ");
    fprintf(stdout, "#individuals=%lu, ", configuration.n_individuals);
    fprintf(stdout, "#steps=%lu, ", configuration.n_steps);

    /* union */
    fprintf(stdout, "probability union event=%f, ", configuration.configuration_union.event_probability);

    /* intersection */
    fprintf(stdout, "probability intersection event=%f, ", configuration.configuration_intersection.event_probability);

    /* intersection */
    fprintf(stdout, "probability crossing event=%f, ", configuration.configuration_crossing.event_probability);

    /* alter */
    fprintf(stdout, "probability drop out event=%f, ", configuration.configuration_alter.event_probability);
    fprintf(stdout, "probability drop out=%f, ", configuration.configuration_alter.alter_probability);

    /* renew */
    fprintf(stdout, "probability renew tick=%d, ", configuration.configuration_renew.tick);
    fprintf(stdout, "probability renew=%f\n", configuration.configuration_renew.event_probability);
    fprintf(stdout, "probability renew=%f\n", configuration.configuration_renew.probability);
}