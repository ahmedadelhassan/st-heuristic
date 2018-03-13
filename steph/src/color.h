#ifndef ST_HEURISTIC_COLOR_H
#define ST_HEURISTIC_COLOR_H

typedef enum {
    WHITE, /**< WHITE to denote not selected element */
    BLACK  /**< BLACK to denote selected elements    */
} color_t;

extern color_t color_switch(color_t c);

#endif /* ST_HEURISTIC_COLOR_H */
