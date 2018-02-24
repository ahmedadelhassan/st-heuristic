#ifndef __UNION_FIND__
#define __UNION_FIND__

typedef struct union_find_t {
    int*   parents;
    int*   ranks;
    size_t size;
    size_t count;
} union_find_t;

extern union_find_t* union_find_alloc(size_t n);
extern void          union_find_reset(union_find_t* uf);
extern void          union_find_release(union_find_t* uf);
extern int           union_find_find_recursive_compression(union_find_t* uf, int i);
extern int           union_find_find_iterative_splitting(union_find_t* uf, int i);
extern int           union_find_find_iterative_halving(union_find_t* uf, int i);
extern int           union_find_find_iterative_comprssion(union_find_t* uf, int i);
extern void          union_find_union(union_find_t* uf, int i, int j);

#endif /* __UNION_FIND__ */
