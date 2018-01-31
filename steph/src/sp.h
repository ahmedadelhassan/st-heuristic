#ifndef __SP__
#define __SP__

#include "list.h"

typedef struct sp
{
  unsigned int w;
  list*        p;
} sp;

typedef struct path
{
  unsigned int v_label;
} path;

#endif /* __SP__ */
