#include <assert.h>

#include "graph.h"
#include "list.h"
#include "sp_table.h"

list* sp_mk(graph* g, sp_table* spt, unsigned int u_label, unsigned int v_label)
{
  assert(g);
  assert(spt);

  /* get the shortrest path weight of a path from u_label to v_label */
  unsigned int w = sp_table_get(g, u_label, v_label);

  /* build a path from u_label to v_label of total weight w */
  return(sp_mk_w(g, w, u_label, v_label));
}
