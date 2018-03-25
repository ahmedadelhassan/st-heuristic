#ifndef _LOAD_GRAPH__
#define _LOAD_GRAPH__

#include "../include/graph.h"

/* Need to be here for FILE* type */
#include <stdio.h>

int load_graph_sparse(FILE* stream, Graph_sparse* g);

#endif
