/*
 *   Copyright (C) 2016 Grzegorz Sygieda
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __EGOS_HPP__
#define __EGOS_HPP__

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <cassert>

#include "looper.hpp"
#include "timerpool.hpp"
#include "workitem.hpp"
#include "utils.hpp"

namespace osapi {

enum class comm_ports : uint8_t {
    SERIAL_PORT = 0,
    CAN_PORT = 1,
};

class egos : public singleton<egos> {

    std::unique_ptr<timerpool> _timers;
    std::unique_ptr<looper_if<workitem>> _main_looper;

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
    void initialize(int &argc, char **argv);

    void start();

    void process(autoptr<workitem> item) {

        prints("process %u\n", item->get_id());
        _main_looper->post(item);
        prints("process done %u\n", item->get_id());
    }

    void process_delayed(autoptr<workitem> item, int msecs) {

        prints("process_delayed(%u) %u\n", msecs, item->get_id());
        timer *tmr = _timers->get_timer(true);
        assert(tmr);

        tmr->wait_async(msecs, [this, item]() {
                _main_looper->post(item);
            });

        prints("process_delayed(%u) %u done\n", msecs, item->get_id());
    }

    /* Static methods */
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
