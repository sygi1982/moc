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
#ifndef __TIMERPOOL_HPP__
#define __TIMERPOOL_HPP__

#include <list>

namespace osapi {

class timer {
    bool is_async;

public:
    timer() : is_async(false) {};

    void wait_async() {};

    void wait_cancel() {};

    void wait_sync() {};

};

class timerpool {

    typedef struct _timerinfo {
        int id;
        bool acquired;
        timer tmr;
    } timerinfo;

    std::list<timerinfo> tmrs_info;

public:
    timerpool() {};

    timer& get_timer() {};

    void put_timer(const timer& tmr) {};

    void wipe_out() {};

    friend class timer;

};

}

#endif
