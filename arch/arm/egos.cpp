#include <cstring>
#include <vector>
#include <string>
#include "egos.hpp"

namespace osapi {

extern "C" {
    extern void debug_print(const char *fmt, ...);
}

void egos::prints(const char *fmt, ...)
{
    debug_print(fmt);
}

void egos::introduceSelf(void)
{
    egos::prints("Hello from arm\n");
}

void egos::initialize(int &argc, char **argv)
{
}

void egos::parseOpts(const char *app)
{
}

void egos::waitForEvent(int timeoutMsecs)
{
}

void* egos::getWorkItem()
{
    return nullptr;
}

}