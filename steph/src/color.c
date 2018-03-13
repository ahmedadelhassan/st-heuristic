#include "color.h"

/**
 *
 * @param c A color
 * @return BLACK if \a c is WHITE and WHITE otherwise.
 */
color_t color_switch(color_t c) {
    return (c == WHITE ? BLACK : WHITE);
}