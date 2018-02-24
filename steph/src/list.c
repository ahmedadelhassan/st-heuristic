#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list_t.h"

/**
 *
 * @return
 */
list_t* list_alloc()
{
    list_t* l = (list_t*) malloc(sizeof(list_t));
    if (l == NULL)
    {
      perror("mem alloc");
      exit(EXIT_FAILURE);
    }

    l->next = NULL;
    l->data = NULL;

    return(l);
}

/**
 *
 * @param l
 */
static void list_free(list_t* l)
{
    if (l != NULL)
    {
        memset(l, 0x0, sizeof(list_t));
        free(l);
    }
}

/**
 *
 * @param l
 */
void list_release(list_t* l)
{
  if (l)
  {
      list_release(l->next);
      list_free(l);
  }
}

/**
 *
 * @param l
 * @return
 */
size_t list_size(list_t* l)
{
    size_t size = 0;
    while (l != NULL)
    {
        size++;
        l = l->next;
    }
    return(size);
}

extern list_t* list_delete(list_t* l, size_t i);
extern list_t* list_delete_first(list_t* l);

/**
 *
 * @param l
 * @return
 */
list_t* list_delete_last(list_t* l) {
    list_t* p = l;

    /* empty list */
    if (p == NULL)
    {
        return(NULL);
    }

    /* progress till the end */
    while (p->next != NULL)
    {
        p = p->next;
    }

    /* singleton list */
    if (p == l)
    {
        list_free(p);
        return(NULL);
    }

    /* non-singleton list */
    list_free(p->next);
    p->next = NULL;
    return(l);

}
