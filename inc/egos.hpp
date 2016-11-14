#ifndef __EGOS_HPP__
#define __EGOS_HPP__

#include <vector>
#include <memory>
#include <map>
#include <string>

#include "looper.hpp"
#include "workitem.hpp"

namespace osapi {

enum class comm_ports : uint8_t {
    SERIAL_PORT = 0,
    CAN_PORT = 1,
};

class egos {
    egos() {};

    std::unique_ptr<looper_if<workitem>> main_looper;

    class options {
    public:
        options() {};

        std::vector<std::string> raw_tokens;

        bool verbose;
        bool interactive_mode;
        std::map<uint8_t, std::string> ports;

        bool check_opt(const std::string &opt);

        const std::string get_opt_arg(const std::string &opt);
    } opts;

    void parse_opts(const char *app);

public:
    egos(egos const&) = delete;
    void operator=(egos const&) = delete;

    void initialize(int &argc, char **argv);

    void start() { main_looper->run(); };

    void process(std::shared_ptr<workitem> &item) { main_looper->post(item); };

    /* Static methods */
    static egos& get_instance()
    {
        static egos inst;
        return inst;
    }

    static void prints(const char *fmt, ...);

    static void introduce_self(void);

    class exception {
        int code;
    public:
        exception() : code(0) {;};
        exception(int code) : code(code) {;};
        int get_code() { return code; };
    };
};

}

#endif
