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
#ifndef __PORTS_HPP__
#define __PORTS_HPP__

#include <functional>
#include <string>
#include <vector>

namespace osapi {

struct frame {
    int _timestamp;
};

class port;

struct port_clbk_if {
    virtual void frame_received(port *owner, frame &) const = 0;

};

class port {
    std::function<void(frame &f)> _handler;

public:
    port(std::string name, port_clbk_if *rcv, int baudrate) :
        _name(name),
        _rcv(rcv),
        _idx(-1),
        _baudrate(baudrate),
        _attached(false),
        _priv_data(nullptr) {
    };

    virtual ~port() {};

    inline bool is_ready() { return _attached; };

    virtual bool init() = 0;
    virtual void deinit() = 0;

    virtual bool send_frame(frame &) = 0;

    inline void set_handler(std::function<void(frame &f)> handler) {
        _handler = handler;
    }

    inline void call_handler(frame &f) {
        if (_handler != nullptr)
            _handler(f);
    }

protected:
    std::string _name;
    port_clbk_if *_rcv;
    int _idx;
    int _baudrate;
    bool _attached;
    void *_priv_data;

};

struct CAN_FRAME : frame {
    unsigned int _id;
    std::vector<unsigned char> _data;
    unsigned char _flags;
};

struct generic_port_watcher : port_clbk_if
{
    void frame_received(port *owner, frame &f) const {
        owner->call_handler(f);
    }
};

enum class can_mode : uint8_t {
    NORMAL = 0,
    LISTEN_ONLY = 1,
    LOOPBACK = 2
};

class can_port : public port
{
    can_mode _mode;

    bool init();
    void deinit();

public:
    inline can_port(std::string name, int baudrate, can_mode mode) :
        port(name, new generic_port_watcher, baudrate), _mode(mode) {

        if (init())
            _attached = true;
    };

    inline can_port(std::string name, int baudrate) :
        can_port(name, baudrate, can_mode::NORMAL) {};

    inline can_port(std::string name) :
        can_port(name, 1000, can_mode::NORMAL) {};

    ~can_port() {
        if (_attached)
            deinit();
    }

    bool send_frame(frame &f);
};

struct SER_FRAME : frame {
    unsigned char _data;
};

class serial_port : public port
{
    bool init();
    void deinit();

public:
    inline serial_port(std::string name, int baudrate) :
        port(name, new generic_port_watcher, baudrate) {

        if (init())
            _attached = true;
    };

    inline serial_port(std::string name) :
        serial_port(name, 9600) {};

    ~serial_port() {
        if (_attached)
            deinit();
    };

    bool send_frame(frame &f);
};

}

#endif
