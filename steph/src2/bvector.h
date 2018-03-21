#ifndef ST_HEURISTIC_BVECTOR_H
#define ST_HEURISTIC_BVECTOR_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char *p_uchars;
    size_t n_uchars;
    size_t n_bits;
} bvector_t;

extern bvector_t *bvector_alloc(size_t n);

extern void bvector_release(bvector_t *p_bvector);

extern void bvector_copy(bvector_t *p_dest_bvector, const bvector_t *p_src_bvector);

extern void bvector_set(bvector_t *p_bvector, int i);

extern void bvector_set_all(bvector_t *p_bvector);

extern void bvector_unset(bvector_t *p_bvector, int i);

extern void bvector_unset_all(bvector_t *p_bvector);

extern int bvector_get(const bvector_t *p_bvector, int i);

extern void bvector_or(bvector_t *p_dest_bvector, const bvector_t *p_bvector1, const bvector_t *p_bvector2);

extern void bvector_and(bvector_t *p_dest_bvector, const bvector_t *p_bvector1, const bvector_t *p_bvector2);

extern void bvector_complement(bvector_t *p_dest_bvector, const bvector_t *p_src_bvector);

extern int bvector_first_false(bvector_t *p_bvector);

extern int bvector_first_true(bvector_t *p_bvector);

extern void bvector_print(FILE *f, const bvector_t *p_bvector);

#endif /* ST_HEURISTIC_BVECTOR_H */
