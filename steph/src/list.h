#ifndef ST_HEURISTIC_LIST_H
#define ST_HEURISTIC_LIST_H

typedef struct list_t
{
    struct list_t* next;
    void*          data;
} list_t;

extern list_t* list_alloc();
extern void    list_release(list_t* l);
extern size_t  list_size(list_t* l);
extern list_t* list_delete_ith(list_t* l, size_t i);

#endif /* ST_HEURISTIC_LIST_H */
