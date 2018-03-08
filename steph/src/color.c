#include "color.h"

/**
 *
 * @param c
 * @return
 */
color_t color_switch(color_t c) {
    return (c == WHITE ? BLACK : WHITE);
}