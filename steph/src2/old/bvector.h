#ifndef ST_HEURISTIC_bvector_H
#define ST_HEURISTIC_bvector_H

#include <stdlib.h>
#include "bool.h"

typedef struct {
    bool_t *p_bools;
    size_t n_bools;
} bvector_t;

extern bvector_t* bvector_alloc(size_t n_bools);

extern void bvector_release(bvector_t *p_bvector);

extern void bvector_copy(bvector_t *p_dest_bvector, bvector_t *p_src_bvector);

extern bool_t bvector_is_false(bvector_t *p_bvector, int i);

extern bool_t bvector_is_true(bvector_t *p_bvector, int i);

extern bool_t bvector_get(bvector_t *p_bvector, int i);

extern void bvector_set(bvector_t *p_bvector, int i, bool_t b);

extern void bvector_set_false(bvector_t *p_bvector, int i);

extern void bvector_set_true(bvector_t *p_bvector, int i);

extern void bvector_set_all(bvector_t *p_bvector, bool_t b);

extern void bvector_set_all_false(bvector_t *p_bvector);

extern void bvector_set_all_true(bvector_t *p_bvector);

extern int bvector_first_false_index(bvector_t *p_bvector);

extern int bvector_first_true_index(bvector_t *p_bvector);


#endif /* ST_HEURISTIC_bvector_H */
