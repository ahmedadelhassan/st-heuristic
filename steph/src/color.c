#include "color.h"

/**
 *
 * @param c A color
 * @return BLACK if \a c is WHITE and WHITE otherwise.
 */
color_t color_switch(color_t c) {
    color_t switched_c = WHITE ? BLACK : WHITE;
    return (switched_c);
}