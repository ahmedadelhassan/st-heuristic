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
    fprintf(stdout, "graph summary.\n");
    fprintf(stdout, "number of nodes = %zu\n", p_graph->n_nodes);
    fprintf(stdout, "number of edges = %zu\n", p_graph->n_edges);
    fprintf(stdout, "number of terminal nodes = %zu\n", p_graph->n_terminal_nodes);
    fprintf(stdout, "number of non-terminal nodes = %zu\n", p_graph->n_non_terminal_nodes);

    /* configuration */
    configuration_t configuration;

    /* general */
    configuration.n_individuals = 50;
    configuration.n_steps = -1;

    /* union */
    configuration.configuration_union.event_probability = 0.05;

    /* intersection */
    configuration.configuration_intersection.event_probability = 0.50;

    /* intersection */
    configuration.configuration_crossing.event_probability = 0.20;

    /* alter */
    configuration.configuration_alter.event_probability = 0.10;
    configuration.configuration_alter.alter_probability = 0.10;

    /* renew */
    configuration.configuration_renew.event_probability = 0.10;
    configuration.configuration_renew.tick = 1000;
    configuration.configuration_renew.probability = 0.25;

#ifndef ST_HEURISTIC_RELEASE
    configuration_print(configuration);
#endif /* ST_HEURISTIC_RELEASE */

    /* here we go */
    optimizer_run(p_graph, configuration);

    return (0);
}
