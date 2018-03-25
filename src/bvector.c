#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bvector.h"

#define BYTE (8 * sizeof(unsigned char))

#define FULL (255)

#define NUCHARS(i) ((int) i / BYTE)

#define OFFSET(i)  (BYTE - 1 - (i % BYTE))

/**
 *
 * @param size
 * @return
 */
bvector_t *bvector_alloc(size_t size) {
    bvector_t *p_bvector = (bvector_t*) malloc(sizeof(bvector_t));
    if (!p_bvector) {
        fprintf(stderr, "bvector_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    p_bvector->n_bits = size;
    p_bvector->n_uchars = 1 + NUCHARS(size);
    p_bvector->p_uchars = (unsigned char*) calloc(p_bvector->n_uchars, sizeof(unsigned char));
    if (!p_bvector->p_uchars) {
        fprintf(stderr, "bvector_alloc. memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    return (p_bvector);
}

/**
 *
 * @param p_bvector
 */
void bvector_release(bvector_t *p_bvector) {
    if (p_bvector) {
        if (p_bvector->p_uchars) {
            memset(p_bvector->p_uchars, 0x0, p_bvector->n_uchars * sizeof(unsigned char));
            free(p_bvector->p_uchars);
        }
        memset(p_bvector, 0x0, sizeof(bvector_t));
    }
}

/**
 *
 * @param p_dest_bvector
 * @param p_src_bvector
 */
void bvector_copy(bvector_t *p_dest_bvector, const bvector_t *p_src_bvector) {
    assert(p_src_bvector);
    assert(p_dest_bvector);
    assert(p_dest_bvector->n_bits == p_src_bvector->n_bits);

    for (int i = 0; i < p_dest_bvector->n_uchars; i++) {
        p_dest_bvector->p_uchars[i] = p_src_bvector->p_uchars[i];
    }
}

/**
 *
 * @param p_bvector
 * @param i
 */
void bvector_set(bvector_t *p_bvector, int i) {
    if (!p_bvector) {
        return;
    }
    p_bvector->p_uchars[NUCHARS(i)] |= (1u << OFFSET(i));
}

/**
 *
 * @param p_bvector
 */
void bvector_set_all(bvector_t *p_bvector) {
    if (!p_bvector) {
        return;
    }
    for (int i = 0; i < p_bvector->n_uchars; i++) {
        p_bvector->p_uchars[NUCHARS(i)] = FULL;
    }
}

/**
 *
 * @param p_bvector
 * @param i
 */
void bvector_unset(bvector_t *p_bvector, int i) {
    if (!p_bvector) {
        return;
    }
    p_bvector->p_uchars[NUCHARS(i)] &= ~(1u << OFFSET(i));
}

/**
 *
 * @param p_bvector
 */
void bvector_unset_all(bvector_t *p_bvector) {
    if (!p_bvector) {
       return;
    }
    for (int i = 0; i < p_bvector->n_uchars; i++) {
        p_bvector->p_uchars[i] = 0u;
    }
}

/**
 *
 * @param p_bvector
 * @param i
 * @return
 */
int bvector_get(const bvector_t *p_bvector, int i) {
    assert(p_bvector);

    return (p_bvector->p_uchars[NUCHARS(i)] & (1u << OFFSET(i)));
}

/**
 *
 * @param p_bvector
 * @param i
 * @return
 */
int bvector_is_false(const bvector_t *p_bvector, int i) {
    return (! bvector_get(p_bvector, i));
}

/**
 *
 * @param p_bvector
 * @param i
 * @return
 */
int bvector_is_true(const bvector_t *p_bvector, int i) {
    return (! bvector_get(p_bvector, i));
}

/**
 *
 * @param p_dest_bvector
 * @param p_src_bvector1
 * @param p_src_bvector2
 */
void bvector_or(bvector_t *p_dest_bvector, const bvector_t *p_src_bvector1, const bvector_t *p_src_bvector2) {
    assert(p_src_bvector1);
    assert(p_src_bvector2);
    assert(p_dest_bvector);
    assert(p_src_bvector1->n_bits == p_src_bvector2->n_bits);
    assert(p_dest_bvector->n_bits == p_src_bvector1->n_bits);

    for (int i = 0; i < p_dest_bvector->n_uchars; i++) {
        p_dest_bvector->p_uchars[i] = p_src_bvector1->p_uchars[i] | p_src_bvector2->p_uchars[i];
    }
}

/**
 *
 * @param p_dest_bvector
 * @param p_src_bvector1
 * @param p_src_bvector2
 */
void bvector_and(bvector_t *p_dest_bvector, const bvector_t *p_src_bvector1, const bvector_t *p_src_bvector2) {
    assert(p_src_bvector1);
    assert(p_src_bvector2);
    assert(p_dest_bvector);
    assert(p_src_bvector1->n_bits == p_src_bvector2->n_bits);
    assert(p_dest_bvector->n_bits == p_src_bvector1->n_bits);

    for (int i = 0; i < p_dest_bvector->n_uchars; i++) {
        p_dest_bvector->p_uchars[i] = p_src_bvector1->p_uchars[i] & p_src_bvector2->p_uchars[i];
    }
}

/**
 *
 * @param p_dest_bvector
 * @param p_src_bvector
 */
void bvector_complement(bvector_t *p_dest_bvector, const bvector_t *p_src_bvector) {
    assert(p_src_bvector);
    assert(p_dest_bvector);
    assert(p_dest_bvector->n_bits == p_src_bvector->n_bits);

    for (int i = 0; i < p_dest_bvector->n_uchars; i++) {
        p_dest_bvector->p_uchars[i] = ~(p_src_bvector->p_uchars[i]);
    }
}

/**
 *
 * @param p_bvector
 * @return
 */
size_t bvector_n_falses(const bvector_t *p_bvector) {
    assert(p_bvector);

    return (p_bvector->n_bits - bvector_n_trues(p_bvector));
}

/**
 *
 * @param p_bvector
 * @return
 */
size_t bvector_n_trues(const bvector_t *p_bvector) {
    assert(p_bvector);

    size_t count = 0;
    for (int i = 0; i < p_bvector->n_uchars; i++) {
        if (i < p_bvector->n_uchars - 1) {
            if (p_bvector->p_uchars[i]) {
                for (int j = 8 * sizeof(unsigned char) - 1; j >= 0; j--) {
                    if (p_bvector->p_uchars[i] & (1u << j)) {
                        count++;
                    }
                }
            }
        }
        else {
            for (int j = 8 - 1; j > OFFSET(p_bvector->n_bits); j--) {
                if (p_bvector->p_uchars[i] & (1u << j)) {
                    count++;
                }
            }
        }
    }
    return(count);
}

/**
 *
 * @param f
 * @param x
 */
static void bvector_print_uchar(FILE *f, unsigned char x)
{
    for (int i = 8 - 1; i >= 0; i--) {
        if (x & (1u << i)) {
            fputc((int) '1', f);
        } else {
            fputc((int) '0', f);
        }
    }
}

/**
 *
 * @param f
 * @param p_bvector
 */
void bvector_print(FILE *f, const bvector_t *p_bvector) {
    if (!p_bvector) {
        return;
    }

    fprintf(f, "n_uchars=%lu n_bits=%lu p_uchars=", p_bvector->n_uchars, p_bvector->n_bits);
    for (int i = 0; i < p_bvector->n_uchars; i++) {
        if (i < p_bvector->n_uchars - 1) {
            bvector_print_uchar(f, p_bvector->p_uchars[i]);
            fputc(' ', f);
        } else {
            for (int j = 8 - 1; j > OFFSET(p_bvector->n_bits); j--) {
                if (p_bvector->p_uchars[i] & (1u << j)) {
                    fputc((int) '1', f);
                } else {
                    fputc((int) '0', f);
                }
            }
        }
    }
    fputc('\n', f);
}