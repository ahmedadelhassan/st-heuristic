#include <stdlib.h>

#include "probability.h"

/**
 *
 * @return
 */
probability_t probability_rand() {
    return (rand() / (double) RAND_MAX);
}

/**
 *
 * @param p
 * @return
 */
int probability_check(double p) {
    return (p >= 0.0 && p <= 1.0);
}
