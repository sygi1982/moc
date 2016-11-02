#include "stdio.h"

#ifdef __cplusplus
extern "C"
{
#endif

void debug_print(const char *message)
{
    printf("%s\n", message);
}

#ifdef __cplusplus
}
#endif
