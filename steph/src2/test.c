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
    graph_t *p_graph = graph_read(stdin);
    fprintf(stdout, "number of nodes = %zu\n", p_graph->n_nodes);
    fprintf(stdout, "number of edges = %zu\n", p_graph->n_edges);
    fprintf(stdout, "number of terminal nodes = %zu\n", p_graph->n_terminal_nodes);
    fprintf(stdout, "number of non-terminal nodes = %zu\n", p_graph->n_non_terminal_nodes);

    /* configuration */
    configuration_t configuration;

    /* general */
    configuration.n_individuals = 10;
    configuration.n_epochs = 3;

    /* union */
    configuration.configuration_union.event_probability = 0.3;

    /* intersection */
    configuration.configuration_intersection.event_probability = 0.3;

    /* intersection */
    configuration.configuration_crossing.event_probability = 0.1;

    /* drop out */
    configuration.configuration_drop_out.event_probability = 0.1;
    configuration.configuration_drop_out.drop_out_probability = 0.1;

    /* insert */
    configuration.configuration_insert.event_probability = 0.1;
    configuration.configuration_insert.insert_probability = 0.1;

    /* renew */
    configuration.configuration_renew.event_probability = 0.1;
    configuration.configuration_renew.probability = 0.1;

#ifndef ST_HEURISTIC_RELEASE
    configuration_print(configuration);
#endif /* ST_HEURISTIC_RELEASE */

    /* run */
    optimizer_run(p_graph, configuration);

    return (0);
}
