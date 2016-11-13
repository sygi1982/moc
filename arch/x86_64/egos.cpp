#include <iostream>
#include <cstdarg>
#include <cstring>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "egos.hpp"
#include "looper.hpp"

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
    for(int arg = 1; arg < argc; arg++) {
        auto astr(std::string(argv[arg]));
        this->opts.rawTokens.push_back(astr);
    }

    this->parseOpts(argv[0]);

    this->mainLooper =
        std::unique_ptr<looper<std::mutex, std::condition_variable>>(
            new looper<std::mutex, std::condition_variable>());
}

void egos::parseOpts(const char *app)
{
    if (this->opts.checkOpt("-help")) {
        egos::prints("Usage [%s] [options]\n", app);
        egos::prints("Options:\n");
        egos::prints("-help    -> prints this info\n");
        egos::prints("-verbose -> enables verbose mode\n");
        egos::prints("-imode   -> enables interactive mode\n");
        egos::prints("-serialp [serial port] -> defines serial port\n");
        egos::prints("-canp [can port] -> defines CAN port\n");

        throw egos::exception();
    }

    this->opts.verbose = this->opts.checkOpt("-verbose");
    this->opts.interactiveMode = this->opts.checkOpt("-imode");

    auto serial = this->opts.getOptArg("-serialp");
    if (serial.empty())
        serial = std::string("/dev/ttyS0");

    auto insert_val = [this]
        (commPorts type, std::string name) {
            this->opts.ports.insert(
                std::pair<uint8_t, std::string>(
                static_cast<uint8_t>(type), name));
        };

    insert_val(commPorts::SERIAL_PORT, serial);

    auto can = this->opts.getOptArg("-canp");
    if (can.empty())
        can = std::string("/dev/can0");

    insert_val(commPorts::CAN_PORT, can);

    if (this->opts.verbose) {
        egos::prints("--------------------------\n");
        egos::prints("interactive mode %u\n", this->opts.interactiveMode);

        auto port =
            this->opts.ports.find(static_cast<uint8_t>(commPorts::SERIAL_PORT));
        egos::prints("serial port %s\n", port->second.c_str());

        port =
            this->opts.ports.find(static_cast<uint8_t>(commPorts::CAN_PORT));
        egos::prints("can port %s\n", port->second.c_str());
        egos::prints("--------------------------\n");
    }
}

const std::string egos::options::getOptArg(const std::string &opt)
{
    auto arg = std::find(this->rawTokens.begin(), this->rawTokens.end(), opt);

    return (arg != this->rawTokens.end() && ++arg != this->rawTokens.end()) ?
         *arg : "";
}

bool egos::options::checkOpt(const std::string &opt)
{
    return std::find(this->rawTokens.begin(), this->rawTokens.end(), opt) !=
        this->rawTokens.end();
}

}
