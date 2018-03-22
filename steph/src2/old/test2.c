#include <stdlib.h>
#include <stdio.h>
#include "../bvector.h"

int main(int argc, char *argv[]) {
    bvector_t *p_bvector = bvector_alloc(24);

    bvector_print(stdout, p_bvector);
    bvector_release(p_bvector);

    bvector_t *p_bvector1 = bvector_alloc(14);
    bvector_t *p_bvector2 = bvector_alloc(14);
    bvector_t *p_dest_bvector = bvector_alloc(14);

    bvector_set(p_bvector1, 0);
    bvector_set(p_bvector1, 5);
    bvector_set(p_bvector1, 8);
    bvector_set(p_bvector1, 10);
    bvector_set(p_bvector1, 13);

    bvector_set(p_bvector2, 1);
    bvector_set(p_bvector2, 5);
    bvector_set(p_bvector2, 10);
    bvector_set(p_bvector2, 12);

    bvector_or(p_dest_bvector, p_bvector1, p_bvector2);
    bvector_print(stdout, p_bvector1);
    fprintf(stdout, "|\n");
    bvector_print(stdout, p_bvector2);
    fprintf(stdout, "=\n");
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "~\n");
    bvector_complement(p_dest_bvector, p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "unset_all\n");
    bvector_unset_all(p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "\n");

    bvector_and(p_dest_bvector, p_bvector1, p_bvector2);
    bvector_print(stdout, p_bvector1);
    fprintf(stdout, "&\n");
    bvector_print(stdout, p_bvector2);
    fprintf(stdout, "=\n");
    bvector_print(stdout, p_dest_bvector);

    fprintf(stdout, "bvector_n_trues=%zu\n", bvector_n_trues(p_dest_bvector));
    fprintf(stdout, "~\n");

    bvector_complement(p_dest_bvector, p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_n_trues=%zu\n", bvector_n_trues(p_dest_bvector));
    fprintf(stdout, "unset_all\n");
    bvector_unset_all(p_dest_bvector);
    bvector_print(stdout, p_dest_bvector);
    fprintf(stdout, "bvector_n_trues=%zu\n", bvector_n_trues(p_dest_bvector));
    fprintf(stdout, "\n");

    bvector_release(p_bvector1);
    bvector_release(p_bvector2);
    bvector_release(p_dest_bvector);

}