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
#ifndef __WATCHDOG_HPP__
#define __WATCHDOG_HPP__

#include <functional>
#include <cassert>

#include "timerpool.hpp"

namespace osapi {

class watchdog {
public:
    struct bark {
        virtual void operator()(void) const = 0;

    };

    watchdog(bark &handler) :
        _handler(handler),
        _timer(nullptr),
        _msecs(0) {
    };

    virtual ~watchdog() {};

    void watch(timer *timer, int msecs) {
        assert(timer);
        assert(msecs);
        _timer = timer;
        _msecs = msecs;
        _timer->wait_async(msecs, [this]() {
                _handler();
            });
    }

    void hug() {
        assert(_timer);
        _timer->cancel_async();
    }

    void feed() {
        hug();
        watch(_timer, _msecs);
    }

private:
    bark &_handler;
    timer *_timer;
    int _msecs;
};

}

#endif
