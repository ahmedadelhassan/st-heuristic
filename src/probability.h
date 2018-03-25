#ifndef ST_HEURISTIC_PROBABILITY_H
#define ST_HEURISTIC_PROBABILITY_H

#include "utils.h"

typedef double probability_t;

#define probability_mk(x) ((x < 0) ? 0.0 : MIN(1.0, x))

extern probability_t probability_rand();

extern int probability_check(double p);

#endif /* ST_HEURISTIC_PROBABILITY_H */

