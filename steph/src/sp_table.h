#ifndef __SP_TABLE__
#define __SP_TABLE__

typedef struct sp_table
{
    size_t         size;
    unsigned int** table;
} sp_table;


extern sp_table*    sp_table_alloc(size_t size);
extern void         sp_table_release(sp_table* spt);
extern void         sp_table_set_dist(sp_table* spt, unsigned int u_label, unsigned int v_label, unsigned int d);
extern unsigned int sp_table_get_dist(sp_table* spt, unsigned int u_label, unsigned int v_label);

#endif /* __SP_TABLE__ */
