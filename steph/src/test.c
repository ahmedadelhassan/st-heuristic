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
    graph_t *p_g = graph_read(file);
    fprintf(stdout, "number of nodes = %zu\n", p_g->n_nodes);
    fprintf(stdout, "number of edges = %zu\n", p_g->n_edges);
    fprintf(stdout, "number of terminal nodes = %zu\n", p_g->n_terminal_nodes);
    fprintf(stdout, "number of non-terminal nodes = %zu\n", p_g->n_non_terminal_nodes);

    return (0);
}
