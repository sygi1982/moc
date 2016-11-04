#include "stdio.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C"
{
#endif

void debug_print(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

#ifdef __cplusplus
}
#endif
