#include <stdlib.h>
#include <stdio.h>
#include "../bvector.h"

int main(int argc, char *argv[]) {
    bvector_t *p_bvector = bvector_alloc(24);

    bvector_print(stdout, p_bvector);
    for (int i = 0; i < 24; i++) {
        fprintf(stdout, "bvector_set(p_bvector, %d)\n", i);
        fprintf(stdout, "NUCHARS(%d)=%lu\n",  i, NUCHARS(i));
        fprintf(stdout, "OFFSET(%d)=%lu\n",  i, OFFSET(i));
        bvector_set(p_bvector, i);
        bvector_print(stdout, p_bvector);
        fprintf(stdout, "\n");
    }

    bvector_release(p_bvector);

    fprintf(stdout, "NUCHARS(%d)=%lu\n",  0, NUCHARS(0));
    fprintf(stdout, "OFFSET(%d)=%lu\n",  0, OFFSET(0));
    fprintf(stdout, "NUCHARS(%d)=%lu\n",  1, NUCHARS(1));
    fprintf(stdout, "OFFSET(%d)=%lu\n",  1, OFFSET(1));
    fprintf(stdout, "NUCHARS(%d)=%lu\n",  5, NUCHARS(5));
    fprintf(stdout, "OFFSET(%d)=%lu\n",  5, OFFSET(5));
    fprintf(stdout, "NUCHARS(%d)=%lu\n",  10, NUCHARS(10));
    fprintf(stdout, "OFFSET(%d)=%lu\n",  10, OFFSET(10));
    fprintf(stdout, "NUCHARS(%d)=%lu\n",  28, NUCHARS(28));
    fprintf(stdout, "OFFSET(%d)=%lu\n",  28, OFFSET(28));
    fprintf(stdout, "NUCHARS(%d)=%lu\n",  29, NUCHARS(29));
    fprintf(stdout, "OFFSET(%d)=%lu\n",  29, OFFSET(29));

    bvector_t *p_bvector1 = bvector_alloc(30);
    bvector_t *p_bvector2 = bvector_alloc(30);
    bvector_t *p_dest_bvector = bvector_alloc(30);

    bvector_set(p_bvector1, 0);
    bvector_set(p_bvector1, 5);
    bvector_set(p_bvector1, 10);
    bvector_set(p_bvector1, 29);

    bvector_set(p_bvector2, 1);
    bvector_set(p_bvector2, 5);
    bvector_set(p_bvector2, 10);
    bvector_set(p_bvector2, 28);

    fprintf(stdout, "bvector_first_false=%d\n", bvector_first_false(p_dest_bvector));
    fprintf(stdout, "bvector_first_true=%d\n", bvector_first_true(p_dest_bvector));

    bvector_or(p_dest_bvector, p_bvector1, p_bvector2);
    bvector_print(stdout, p_bvector1);
    fprintf(stdout, "|\n");
    bvector_print(stdout, p_bvector2);
    fprintf(stdout, "=\n");
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_first_false=%d\n", bvector_first_false(p_dest_bvector));
    fprintf(stdout, "bvector_first_true=%d\n", bvector_first_true(p_dest_bvector));
    fprintf(stdout, "~\n");
    bvector_complement(p_dest_bvector, p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_first_false=%d\n", bvector_first_false(p_dest_bvector));
    fprintf(stdout, "bvector_first_true=%d\n", bvector_first_true(p_dest_bvector));
    fprintf(stdout, "set_all_false\n");
    bvector_set_all_false(p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_first_false=%d\n", bvector_first_false(p_dest_bvector));
    fprintf(stdout, "bvector_first_true=%d\n", bvector_first_true(p_dest_bvector));
    fprintf(stdout, "\n");

    bvector_and(p_dest_bvector, p_bvector1, p_bvector2);
    bvector_print(stdout, p_bvector1);
    fprintf(stdout, "&\n");
    bvector_print(stdout, p_bvector2);
    fprintf(stdout, "=\n");
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_first_false=%d\n", bvector_first_false(p_dest_bvector));
    fprintf(stdout, "bvector_first_true=%d\n", bvector_first_true(p_dest_bvector));
    fprintf(stdout, "~\n");
    bvector_complement(p_dest_bvector, p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_first_false=%d\n", bvector_first_false(p_dest_bvector));
    fprintf(stdout, "bvector_first_true=%d\n", bvector_first_true(p_dest_bvector));
    fprintf(stdout, "set_all_false\n");
    bvector_set_all_false(p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_first_false=%d\n", bvector_first_false(p_dest_bvector));
    fprintf(stdout, "bvector_first_true=%d\n", bvector_first_true(p_dest_bvector));
    fprintf(stdout, "\n");
}