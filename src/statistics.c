#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "statistics.h"
#include "weight.h"

/**
 *
 * @return
 */
static statistics_t statistics_null() {
    statistics_t statistics;

    statistics.n_samples = 0;
    statistics.mean = 0.0;
    statistics.variance = 0.0;
    statistics.std_deviation = 0.0;
    statistics.min = 0;
    statistics.max = 0;
    statistics.sum = 0;

    return (statistics);
}

/**
 *
 * @param array
 * @param n
 * @return
 */
statistics_t statistics_mk(weight_t *array, size_t n) {
    statistics_t statistics = statistics_null();

    if (!array || n == 0) {
        return (statistics);
    }

    statistics.n_samples = n;

    statistics.min = array[0];
    statistics.max = array[0];

    for (int i = 0; i < n; i++) {
        if (statistics.min > array[i]) {
            statistics.min = array[i];
        }
        if (statistics.max < array[i]) {
            statistics.max = array[i];
        }
        statistics.sum += array[i];
    }

    statistics.mean = (double) statistics.sum / (double) n;

    double acc = 0.0;
    for (int i = 0; i < n; i++) {
        acc += pow(((double) array[i] - statistics.mean), 2);
    }
    statistics.variance = acc / (double) n;
    statistics.std_deviation = sqrt(statistics.variance);

    return (statistics);
}

/**
 *
 * @param statistics
 */
void statistics_fprint(FILE *f, statistics_t statistics) {
    fprintf(f, "n=%lu", statistics.n_samples);
    fprintf(f, ", ");
    fprintf(f, "mean=%f", statistics.mean);
    fprintf(f, ", ");
    fprintf(f, "var=%f", statistics.variance);
    fprintf(f, ", ");
    fprintf(f, "std dev=%f", statistics.std_deviation);
    fprintf(f, "\n");
    fprintf(f, "min=%lu", statistics.min);
    fprintf(f, ", ");
    fprintf(f, "max=%lu", statistics.max);
    fprintf(f, ", ");
    fprintf(f, "sum=%lu", statistics.sum);
    fprintf(f, "\n");
}