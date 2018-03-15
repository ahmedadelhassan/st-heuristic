#ifndef ST_HEURISTIC_LIST_H
#define ST_HEURISTIC_LIST_H

typedef struct list_t {
    struct list_t *p_next;
    void *data;
} list_t;

extern list_t *p_list_alloc(void *data);

extern void list_release(list_t *p_l);

extern list_t *p_list_copy(const list_t *p_l);

extern size_t list_size(list_t *p_l);

extern list_t *p_list_delete_ith(list_t *p_l, int i);

extern list_t *p_list_delete_rand(list_t *p_l);

extern list_t *p_list_insert_front(list_t *p_l, void *data);

extern list_t *p_list_reverse(list_t *p_l);

extern list_t *p_list_sort(list_t *p_l, int (*data_compar)(const void*, const void*));

extern list_t *p_list_union(list_t *p_l1, list_t *p_l2, int (*data_compar)(const void*, const void*));

extern list_t *p_list_intersection(list_t *p_l1, list_t *p_l2, int (*data_compar)(const void*, const void*));

#endif /* ST_HEURISTIC_LIST_H */

