#ifndef __list__
#define __list__

typedef struct list
{
    struct list* next;
    void*        data;
} list;

extern list* list_alloc();
extern void  list_release(list* l);
extern void  list_release_all(list* l);

#endif /* __list__ */
