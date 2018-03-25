#ifndef ST_HEURISTIC_STATISTICS_H
#define ST_HEURISTIC_STATISTICS_H

#include <stdio.h>
#include <stdlib.h>

#include "weight.h"

typedef struct {
    size_t n_samples;
    double mean;
    double variance;
    double std_deviation;
    weight_t sum;
    weight_t min;
    weight_t max;
} statistics_t;

extern statistics_t statistics_mk(weight_t *array, size_t n);

extern void statistics_fprint(FILE *f, statistics_t statistics);

#endif /* ST_HEURISTIC_STATISTICS_H */
