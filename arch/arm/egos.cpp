#include <cstring>
#include <vector>
#include <string>
#include "egos.h"

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
    std::memset(reinterpret_cast<void*>(&this->opts), 0, sizeof(this->opts));
}

void egos::parseOpts(int &argc, char **argv)
{
    this->opts.rawTokens.push_back(std::string("test"));
}

void egos::waitForEvent(int timeoutMsecs)
{
}

void* egos::getWorkItem()
{
    return nullptr;
}

}
