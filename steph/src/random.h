#ifndef ST_HEURISTIC_RANDOM_H
#define ST_HEURISTIC_RANDOM_H



#define PROBABILITY(p) ((p) >= 0 && (p) <= 1)

extern void random_init();

extern void random_shuffle(void *array, size_t n, size_t m);



#endif /* ST_HEURISTIC_RANDOM_H */
