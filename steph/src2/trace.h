#ifndef ST_HEURISTIC_TRACE_H
#define ST_HEURISTIC_TRACE_H

#define TRACE_RUN 1

#define TRACE(x) do { if (TRACE_RUN) trace_printf x; } while (0)

#if TRACE_RUN
extern void trace_printf(const char *fmt, ...);
#endif /* TRACE_RUN */

#endif /* ST_HEURISTIC_TRACE_H */
