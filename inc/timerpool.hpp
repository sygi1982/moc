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

#include <vector>
#include <memory>
#include <functional>
#include <cassert>

#include "utils.hpp"

namespace osapi {

using namespace utils;

class timerpool;

class timer {
    int _id;
    bool _is_async;
    timerpool *_pool;
    void *_priv_data;

public:
    explicit timer(int &id);

    ~timer();

    void wait_async(int msecs, std::function<void()> delegate);

    void cancel_async();

    void wait_sync(int msecs);

    friend class timerpool;
};

class timerpool {

    struct timerinfo {
        timer tmr;

        int id;
        bool acquired;
        bool auto_return;

        timerinfo(int id) : tmr(id),
                            id(id),
                            acquired(false),
                            auto_return(false)
                            {};

    };

    std::vector<autoptr<timerinfo>> _tmrs_info;

    void try_return(timer *tmr) {
        assert(tmr);

        timerinfo *tmri = reinterpret_cast<timerinfo*>(tmr);
        if (tmri->auto_return)
            put_timer(&tmri->tmr);

    }

public:
    explicit timerpool(int max_timers) {
        autoptr<timerinfo> tmri;

        for (int i = 0; i < max_timers; i++) {
            tmri = autoptr<timerinfo>(new timerinfo(i));
            tmri->tmr._pool = this;
            _tmrs_info.push_back(tmri);
        }
    }

    timer *get_timer(bool auto_ret) {
        assert(!_tmrs_info.empty());

        for (auto tmri : _tmrs_info) {
            if (!tmri->acquired) {
                tmri->acquired = true;
                tmri->auto_return = auto_ret;
                return &tmri->tmr;
            }
        }

        return nullptr;
    }

    void put_timer(timer *tmr) {
        assert(tmr);

        timerinfo *tmri = reinterpret_cast<timerinfo*>(tmr);
        assert(tmri->id == tmr->_id);
        assert(tmri->acquired);

        if (tmr->_is_async)
            tmr->cancel_async();

        tmri->acquired = false;
        tmri->auto_return = false;
    }

    void wipe_out() {
        for (auto tmri : _tmrs_info) {
            if (tmri->acquired)
                put_timer(&tmri->tmr);
        }
    }

    friend class timer;
};

}

#endif
