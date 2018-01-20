#ifndef _LOAD_GRAPH__
#define _LOAD_GRAPH__

#include "../include/graph_common.h"

/* Update this macro if you move the code... */
#define PATH_PREFIX "../public/"
#define PATH_GRAPH_SIZE 256

int load_graph_dense(const char* path_file_output, Graph_dense* g);

#endif
