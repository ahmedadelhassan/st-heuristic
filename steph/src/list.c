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
    (l != NULL)
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

/**
 *
 * @param l
 * @param i
 * @return
 */
list_t* list_delete_aux(list_t* l, size_t i)
{
    if (l == NULL)
    {
        return(NULL);
    }

    if (i == 0)
    {

    }
}

/**
 *
 * @param l
 * @param i
 * @return
 */
list_t* list_delete_ith(list_t* l, size_t i)
{
    list_t* prev_p = NULL;
    list_t* p      = l;

    while ((p != NULL) && (i > 0))
    {
        prev_p = p;
        p      = p->next;
        i--;
    }

    if (p == NULL)
    {
        return(l);
    }
    else /* i == 0 */
    {
        if (prev_p == NULL)
        {
            /* delete first item of l */
            list_t* next_p = p->next;
            list_free(p);
            return(next_p);
        }
        else
        {
            prev_p->next = p->next;
            list_free(p);
            return(l);
        }
    }

}
