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
    statistics.sum = 0.0;

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

    for (int i = 0; i < n; i++) {
        statistics.sum += array[i];
    }

    statistics.mean = (double) statistics.sum / (double) n;

    double acc = 0.0;
    for (int i = 0; i < n; i++) {
        acc += +pow(((double) array[i] - statistics.mean), 2);
    }
    statistics.variance = acc / (double) n;
    statistics.std_deviation = sqrt(statistics.variance);

    return (statistics);
}

/**
 *
 * @param statistics
 */
void statistics_print(statistics_t statistics) {
    fprintf(stdout, "n=%lu", statistics.n_samples);
    fprintf(stdout, "\t");
    fprintf(stdout, "mean=%f", statistics.mean);
    fprintf(stdout, "\t");
    fprintf(stdout, "var=%f", statistics.variance);
    fprintf(stdout, "\t");
    fprintf(stdout, "std dev=%f", statistics.std_deviation);
    fprintf(stdout, "\t");
    fprintf(stdout, "sum=%u", statistics.sum);
    fprintf(stdout, "\n");
}