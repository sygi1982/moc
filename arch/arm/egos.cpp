#include "egos.h"

namespace osapi {

extern "C" {
    extern void debug_print(const char*);
}

void egos::introduce(void)
{
    debug_print("Hello from arm");
}

}
