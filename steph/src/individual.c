
#include "individual.h"

/**
 *
 * @return
 */
individual_t* individual_alloc(list_t* nodes)
{
    individual_t *ind = (individual_t*) malloc(sizeof(individual_t));
    if (ind == NULL)
    {
        fprintf(stderr, "individual_alloc. memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    st->nodes                    = nodes;
    st->n_nodes                  = list_size(nodes);
    st->min_weight_spanning_tree = 0;


    return(st);
}


/**
 *
 * @param st
 */
void individual_release(individual_t* st)
{
    if (st != NULL)
    {
        list_release(st->nodes);
        memset(st, 0x0, sizeof(individual_t));
        free(st);
    }
}

/**
 *
 * @param st
 * @param e
 */
void individual_add_edge(individual_t* st, edge_t* e)
{
    if ((st != NULL) && (e != NULL))
    {
        st->sedges = list_insert_front(st->edges, (void*) e);
        st->total_weight += e->weight;
    }
}

