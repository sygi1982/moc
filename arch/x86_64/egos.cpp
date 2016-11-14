#include <iostream>
#include <cstdarg>
#include <cstring>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <memory>
#include <mutex>

#include "egos.hpp"
#include "looper.hpp"
#include "workitem.hpp"

#include "locker.hpp"
#include "syncer.hpp"

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

void egos::introduce_self(void)
{
    egos::prints("Hello from x86_64\n");
}

void egos::initialize(int &argc, char **argv)
{
    for(int arg = 1; arg < argc; arg++) {
        auto astr(std::string(argv[arg]));
        opts.raw_tokens.push_back(astr);
    }

    parse_opts(argv[0]);

    main_looper =
        std::unique_ptr<looper<locker, syncer<locker>, workitem>>(
            new looper<locker, syncer<locker>, workitem>);
}

void egos::parse_opts(const char *app)
{
    if (opts.check_opt("-help")) {
        egos::prints("Usage [%s] [options]\n", app);
        egos::prints("Options:\n");
        egos::prints("-help    -> prints this info\n");
        egos::prints("-verbose -> enables verbose mode\n");
        egos::prints("-imode   -> enables interactive mode\n");
        egos::prints("-serialp [serial port] -> defines serial port\n");
        egos::prints("-canp [can port] -> defines CAN port\n");

        throw egos::exception();
    }

    opts.verbose = opts.check_opt("-verbose");
    opts.interactive_mode = opts.check_opt("-imode");

    auto serial = opts.get_opt_arg("-serialp");
    if (serial.empty())
        serial = std::string("/dev/ttyS0");

    auto insert_val = [this]
        (comm_ports type, std::string name) {
            opts.ports.insert(
                std::pair<uint8_t, std::string>(
                static_cast<uint8_t>(type), name));
        };

    insert_val(comm_ports::SERIAL_PORT, serial);

    auto can = opts.get_opt_arg("-canp");
    if (can.empty())
        can = std::string("/dev/can0");

    insert_val(comm_ports::CAN_PORT, can);

    if (opts.verbose) {
        egos::prints("--------------------------\n");
        egos::prints("interactive mode %u\n", opts.interactive_mode);

        auto port =
            opts.ports.find(static_cast<uint8_t>(comm_ports::SERIAL_PORT));
        egos::prints("serial port %s\n", port->second.c_str());

        port =
            opts.ports.find(static_cast<uint8_t>(comm_ports::CAN_PORT));
        egos::prints("can port %s\n", port->second.c_str());
        egos::prints("--------------------------\n");
    }
}

const std::string egos::options::get_opt_arg(const std::string &opt)
{
    auto arg = std::find(raw_tokens.begin(), raw_tokens.end(), opt);

    return (arg != raw_tokens.end() && ++arg != raw_tokens.end()) ?
         *arg : "";
}

bool egos::options::check_opt(const std::string &opt)
{
    return std::find(raw_tokens.begin(), raw_tokens.end(), opt) !=
        raw_tokens.end();
}


}
