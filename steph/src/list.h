#ifndef ST_HEURISTIC_LIST_H
#define ST_HEURISTIC_LIST_H

typedef struct list_t {
    struct list_t *next;
    void *data;
} list_t;

extern list_t *list_alloc(void *data);

extern void list_release(list_t *l);

extern list_t *list_copy(const list_t *l);

extern size_t list_size(list_t *l);

extern list_t *list_delete_ith(list_t *l, unsigned int i);

extern list_t *list_delete_ith_with_data_release(list_t *l, unsigned int i, void(*data_release)(void*));

extern list_t *list_delete_rand(list_t *l);

extern list_t *list_delete_rand_with_data_release(list_t *l, void(*data_release)(void*));

extern list_t *list_insert_front(list_t *l, void *data);

extern list_t *list_reverse(list_t *l);

extern list_t *list_sort(list_t *l, int (*data_compar)(const void*, const void*));

extern list_t *list_union(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*));

extern list_t *list_union_with_data_alloc(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*), void* (*data_alloc)(void *));

extern list_t *list_intersection(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*));

extern list_t *list_intersection_with_data_alloc(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*), void* (*data_alloc)(void *));

extern list_t *list_difference(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*));

extern list_t *list_difference_with_data_alloc(list_t *l1, list_t *l2, int (*data_compar)(const void*, const void*), void* (*data_alloc)(void *));

#endif /* ST_HEURISTIC_LIST_H */

