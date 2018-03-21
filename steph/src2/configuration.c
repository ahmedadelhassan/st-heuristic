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
    fprintf(stdout, "#epochs=%lu, ", configuration.n_epochs);

    /* union */
    fprintf(stdout, "probability union event=%f, ", configuration.configuration_union.event_probability);

    /* intersection */
    fprintf(stdout, "probability intersection event=%f, ", configuration.configuration_intersection.event_probability);

    /* intersection */
    fprintf(stdout, "probability crossing event=%f, ", configuration.configuration_crossing.event_probability);

    /* drop out */
    fprintf(stdout, "probability drop out event=%f, ", configuration.configuration_drop_out.event_probability);
    fprintf(stdout, "probability drop out=%f, ", configuration.configuration_drop_out.drop_out_probability);

    /* drop out */
    fprintf(stdout, "probability insert event=%f, ", configuration.configuration_insert.event_probability);
    fprintf(stdout, "probability insert=%f, ", configuration.configuration_insert.insert_probability);

    /* renew */
    fprintf(stdout, "probability renew event=%f, ", configuration.configuration_renew.event_probability);
    fprintf(stdout, "probability renew=%f\n", configuration.configuration_renew.probability);
}