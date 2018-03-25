#include "weight.h"

/**
 *
 * @param p1
 * @param p2
 * @return
 */
int weight_compar(const void *p1, const void *p2) {
    weight_t *p_w1 = (weight_t*) p1;
    weight_t *p_w2 = (weight_t*) p2;

    if (*p_w1 < *p_w2) {
        return (-1);
    } else {
        if (*p_w1 > *p_w2) {
            return (1);
        } else {
            return (0);
        }
    }
}