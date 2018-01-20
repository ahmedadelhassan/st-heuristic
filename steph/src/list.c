#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

list* list_alloc()
{
    list* l = (list*) malloc(sizeof(list));
    if (! l)
    {
      perror("mem alloc");
      exit(EXIT_FAILURE);
    }

    l->next = NULL;
    l->data = NULL;

    return(l);
}

void list_release(list* l)
{
  if (l)
  {
    memset(l, 0x0, sizeof(list));
    free(l);
  }
}

void list_release_all(list* l)
{
  if (l)
  {
    list_release_all(l->next);
    list_release(l);
  }
}
