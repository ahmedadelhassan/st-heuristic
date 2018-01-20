#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sp_table.h"

sp_table* sp_table_alloc(size_t size)
{
    sp_table* spt = (sp_table*) malloc(sizeof(sp_table));
    if (! spt)
    {
      perror("sp_table_alloc. mem alloc error");
      exit(EXIT_FAILURE);
    }

    spt->size = size;

    spt->table = (unsigned int**) calloc(size, sizeof(unsigned int*));
    if (! spt->table)
    {
      perror("sp_table_alloc. mem alloc error");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++)
    {
      spt->table[i] = (unsigned int*) calloc(size - i, sizeof(unsigned int));
      if (! spt->table[i])
      {
        perror("sp_table_alloc. mem alloc error");
        exit(EXIT_FAILURE);
      }
    }

    return(spt);
}

void sp_table_release(sp_table* spt)
{
  if (spt)
  {
    if (spt->table)
    {
      for (int i = 0; i < spt->size; i++)
      {
        if (spt->table[i])
        {
          memset(spt->table[i], 0x0, (spt->size - i) * sizeof(unsigned int));
          free(spt->table[i]);
        }
      }

      memset(spt->table, 0x0, spt->size * sizeof(unsigned int*));
      free(spt->table);
    }

    memset(spt, 0x0, sizeof(sp_table));
    free(spt);
  }
}

void sp_table_set_dist(sp_table* spt, unsigned int u_label, unsigned int v_label, unsigned int d)
{
  if (u_label > v_label)
  {
    int tmp = u_label;
    u_label = v_label;
    v_label = tmp;
  }

  spt->table[u_label][v_label - v_label] = d;
}

unsigned int sp_table_get_dist(sp_table* spt, unsigned int u_label, unsigned int v_label)
{
  if (u_label > v_label)
  {
    int tmp = u_label;
    u_label = v_label;
    v_label = tmp;
  }

  return(spt->table[u_label][v_label - u_label]);
}
