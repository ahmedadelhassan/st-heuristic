#include <stdarg.h>
#include <stdio.h>

#include "trace.h"

#ifdef TRACE_RUN
void trace_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}
#endif /* TRACE_RUN */

