#include <iostream>
#include <cstdarg>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
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

void egos::initialize(int &argc, char **argv)
{
    std::memset(reinterpret_cast<void*>(&this->opts), 0, sizeof(this->opts));
    this->parseOpts(argc, argv);
}

void egos::parseOpts(int &argc, char **argv)
{
    for(int arg = 1; arg < argc; arg++)
    {
        std::string astr(std::string(argv[arg]));
        this->opts.rawTokens.push_back(astr);
    }

    this->opts.verbose = this->opts.checkOpt("-verbose");
    this->opts.interactiveMode = this->opts.checkOpt("-imode");
}

const std::string egos::options::getOptArg(const std::string &opt)
{
    std::vector<std::string>::const_iterator arg;
    arg = std::find(this->rawTokens.begin(), this->rawTokens.end(), opt);

    return (arg != this->rawTokens.end() && ++arg != this->rawTokens.end()) ?
         *arg : nullptr;
}

bool egos::options::checkOpt(const std::string &opt)
{
    return std::find(this->rawTokens.begin(), this->rawTokens.end(), opt) !=
        this->rawTokens.end();
}

void egos::waitForEvent(int timeoutMsecs)
{
}

void* egos::getWorkItem()
{
    return nullptr;
}

}
