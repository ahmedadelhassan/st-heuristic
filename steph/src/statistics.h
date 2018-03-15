#ifndef ST_HEURISTIC_STATISTICS_H
#define ST_HEURISTIC_STATISTICS_H

#include <stdlib.h>
#include "weight.h"

typedef struct {
    size_t n_samples;
    double mean;
    double variance;
    double std_deviation;
    weight_t sum;
} statistics_t;

extern statistics_t statistics_mk(weight_t *array, size_t n);

extern void statistics_print(statistics_t statistics);

#endif /* ST_HEURISTIC_STATISTICS_H */
