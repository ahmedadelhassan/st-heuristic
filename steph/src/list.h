#ifndef ST_HEURISTIC_LIST_H
#define ST_HEURISTIC_LIST_H

typedef struct list_t {
    struct list_t *next;
    void *data;
} list_t;

extern list_t *list_alloc();

extern void list_release(list_t *l);

extern list_t *list_copy(const list_t *l);

extern size_t list_size(list_t *l);

extern list_t *list_delete_ith(list_t *l, size_t i);

extern list_t *list_insert_front(list_t *l, void *data);

extern list_t *list_reverse(list_t *l);

extern list_t *list_sort(list_t *start, int (*compar)(const void *, const void *));

extern list_t *list_union(list_t *l1, list_t *l2, int (*compar)(const void *, const void *));

extern list_t *list_intersection(list_t *l1, list_t *l2, int (*compar)(const void *, const void *));

extern list_t *list_difference(list_t *l1, list_t *l2, int (*compar)(const void *, const void *));

#endif /* ST_HEURISTIC_LIST_H */
