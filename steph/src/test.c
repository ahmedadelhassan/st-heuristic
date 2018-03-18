#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "configuration.h"
#include "graph.h"
#include "optimizer.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    /* load graph */
    graph_t *p_g = graph_read(stdin);
    fprintf(stdout, "number of nodes = %zu\n", p_g->n_nodes);
    fprintf(stdout, "number of edges = %zu\n", p_g->n_edges);
    fprintf(stdout, "number of terminal nodes = %zu\n", p_g->n_terminal_nodes);
    fprintf(stdout, "number of non-terminal nodes = %zu\n", p_g->n_non_terminal_nodes);

    /* configuration */
    configuration_t configuration;

    /* general */
    configuration.graph = p_g;
    configuration.n_individuals = 20;
    configuration.n_epochs = 100;

    /* union */
    configuration.configuration_union.event_probability = 0.5;

    /* intersection */
    configuration.configuration_intersection.event_probability = 0.5;

    /* intersection */
    configuration.configuration_crossing.event_probability = 0.0;
    configuration.configuration_crossing.crossing_probability = 0.0;

    /* drop out */
    configuration.configuration_drop_out.event_probability = 0.0;
    configuration.configuration_drop_out.drop_out_probability = 0.0;

    /* renew */
    configuration.configuration_renew.event_probability = 0.0;
    configuration.configuration_renew.probability = 0.0;

#ifndef ST_HEURISTIC_RELEASE
    configuration_print(configuration);
#endif /* ST_HEURISTIC_RELEASE */

    /* run */
    optimizer_run(configuration);

    return (0);
}
