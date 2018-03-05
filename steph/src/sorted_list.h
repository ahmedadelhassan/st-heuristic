#ifndef ST_HEURISTIC_SORTED_LIST_H
#define ST_HEURISTIC_SORTED_LIST_H

#include "list.h"

extern list_t *sorted_list_insert(list_t *l, void *data, int (*data_data_compar)(const void *, const void *));

extern list_t *sorted_list_union(list_t *l1, list_t *l2, int (*data_compar)(const void *, const void *), void* (*data_copy)(const void*));

extern list_t *sorted_list_intersection(list_t *l1, list_t *l2, int (*data_compar)(const void *, const void *), void* (*data_copy)(const void*));

extern list_t *sorted_list_difference(list_t *l1, list_t *l2, int (*data_compar)(const void *, const void *), void* (*data_copy)(const void*));

#endif /* ST_HEURISTIC_SORTED_LIST_H */
