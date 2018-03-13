#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int main(int argc, char *argv[]) {
    FILE *file;
    file = fopen("instance001.gr", "r");
    if (file == NULL) {
        fprintf(stderr, "cannot open %s\n", "instance001.gr");
        exit(EXIT_FAILURE);
    }
    graph_t *g = graph_read(file);
    fprintf(stdout, "number of nodes = %zu\n", g->n_nodes);
    fprintf(stdout, "number of edges = %zu\n", g->n_edges);
    fprintf(stdout, "number of terminals = %zu\n", g->n_terminals);

    return (0);
}
