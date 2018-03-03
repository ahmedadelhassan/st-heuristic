#ifndef ST_HEURISTIC_SORTED_LIST_H
#define ST_HEURISTIC_SORTED_LIST_H

#include "list.h"

extern list_t *sorted_list_insert(list_t *l, void *data, int (*compar)(const void *, const void *));

extern list_t *sorted_list_union(list_t *l1, sorted_t *l2, int (*compar)(const void *, const void *));

extern list_t *sorted_list_intersection(list_t *l1, sorted_t *l2, int (*compar)(const void *, const void *));

extern list_t *sorted_list_difference(list_t *l1, sorted_t *l2, int (*compar)(const void *, const void *));

#endif /* ST_HEURISTIC_SORTED_LIST_H */
