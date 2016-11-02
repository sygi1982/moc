#include "os.h"

extern "C" {
extern void debug_print(const char*);
}

void os::introduce(void)
{
    debug_print("Hello from arm");
}

