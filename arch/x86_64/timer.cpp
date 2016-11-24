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
#include <thread>
#include <chrono>
#include <functional>
#include <cassert>

#include "egos.hpp"
#include "timerpool.hpp"

namespace osapi {

struct timerdat {
    std::thread *handler;

};

timer::timer() : is_async(false),
                 priv_data(nullptr)
{
    priv_data = reinterpret_cast<void *>(new timerdat);
    assert(priv_data);
};

timer::~timer()
{
    timerdat *data = reinterpret_cast<timerdat *>(priv_data);
    if (data)
        delete data;
};

void timer::wait_async(int msecs, std::function<void()> delegate)
{
    is_async = true;
    timerdat *data = reinterpret_cast<timerdat *>(priv_data);
    data->handler = new std::thread([this, delegate, msecs]() {
            wait_sync(msecs);
            delegate();
            pool->try_return(this);
        });
    data->handler->detach();
};

void timer::cancel_async()
{
    assert(is_async);
    timerdat *data = reinterpret_cast<timerdat *>(priv_data);
    assert(data->handler);
    data->handler->join();
};

void timer::wait_sync(int msecs)
{
    std::chrono::milliseconds delay(msecs);
    std::this_thread::sleep_for(delay);
    is_async = false;

    pool->try_return(this);
};

}
