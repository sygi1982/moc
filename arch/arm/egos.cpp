#include "egos.h"
#include <vector>
#include <string>

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

void egos::initialize(void)
{
}

void egos::parseOpts(int &argc, char **argv)
{
    this->opts.push_back(std::string("test"));
}

void egos::waitForEvent(int timeoutMsecs)
{
}

void* egos::getWorkItem()
{
    return nullptr;
}

}
