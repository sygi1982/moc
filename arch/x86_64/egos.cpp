#include <iostream>
#include <cstdarg>
#include <vector>
#include <string>
#include "egos.h"

namespace osapi {

void egos::prints(const char *fmt, ...)
{
    va_list args;
    char buf[256];

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    std::cout << buf;
}

void egos::introduceSelf(void)
{
    egos::prints("Hello from x86_64\n");
}

void egos::initialize(void)
{
}

void egos::parseOpts(int &argc, char **argv)
{
    for(int arg = 1; arg < argc; arg++)
    {
        std::string astr(std::string(argv[arg]));
        this->opts.push_back(astr);
    }
}

void egos::waitForEvent(int timeoutMsecs)
{
}

void* egos::getWorkItem()
{
    return nullptr;
}

}
