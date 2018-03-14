#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/**
 *
 */
void random_init() {
    srand(time(NULL));
}

/**
 *
 * @param array
 * @param n
 * @param m
 */
void random_shuffle(void *array, size_t n, size_t m) {
    char *char_array = array;
    size_t stride = n * sizeof(char);

    char *buffer = (char *) calloc(m, sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t j = i + (size_t) rand() / (RAND_MAX / (n - i) + 1);

            memcpy(buffer, char_array + j * stride, m);
            memcpy(char_array + j * stride, char_array + i * stride, m);
            memcpy(char_array + i * stride, buffer, m);
        }
    }

    /* clean and free allocated buffer */
    memset(buffer, 0x0, m * sizeof(char));
    free(buffer);
}

