#include <cstring>
#include <vector>
#include <string>

#include "egos.hpp"
#include "looper.hpp"
#include "workitem.hpp"

#include "locker.hpp"
#include "syncer.hpp"

namespace osapi {

extern "C" {
    extern void debug_print(const char *fmt, ...);
}

void egos::prints(const char *fmt, ...)
{
    debug_print(fmt);
}

void egos::introduce_self(void)
{
    egos::prints("Hello from arm\n");
}

void egos::initialize(int &argc, char **argv)
{
    main_looper =
        std::unique_ptr<looper<locker, syncer<locker>, workitem>>(
            new looper<locker, syncer<locker>, workitem>);
}

void egos::parse_opts(const char *app)
{
}

}
