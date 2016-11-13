#ifndef __EGOS_HPP__
#define __EGOS_HPP__

#include <vector>
#include <memory>
#include <map>
#include <string>

#include "looper.hpp"

namespace osapi {

enum class commPorts : uint8_t {
    SERIAL_PORT = 0,
    CAN_PORT = 1,
};

class egos {
    egos() {};

    std::unique_ptr<looperIf> mainLooper;

    class options {
    public:
        options() {};

        std::vector<std::string> rawTokens;

        bool verbose;
        bool interactiveMode;
        std::map<uint8_t, std::string> ports;

        bool checkOpt(const std::string &opt);

        const std::string getOptArg(const std::string &opt);
    } opts;

    void parseOpts(const char *app);

public:
    egos(egos const&) = delete;
    void operator=(egos const&) = delete;

    void initialize(int &argc, char **argv);

    void start() { mainLooper->run(); };

    void process() { mainLooper->post(); };

    /* Static methods */
    static egos& getInstance()
    {
        static egos inst;
        return inst;
    }

    static void prints(const char *fmt, ...);

    static void introduceSelf(void);

    class exception {
        int code;
    public:
        exception() : code(0) {;};
        exception(int code) : code(code) {;};
        int getCode() { return code; };
    };
};

}

#endif
