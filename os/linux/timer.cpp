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

timer::timer() : _is_async(false),
                 _priv_data(nullptr)
{
};

timer::~timer()
{
    std::thread *handler = static_cast<std::thread *>(_priv_data);
    if (handler)
        delete handler;
};

void timer::wait_async(int msecs, std::function<void()> delegate)
{
    egos::prints("wait_async %u\n", msecs);
    _is_async = true;
    std::thread *handler = new std::thread([this, delegate, msecs]() {
            wait_sync(msecs);
            delegate();
            _pool->try_return(this);
        });
    _priv_data = static_cast<void *>(handler);
};

void timer::cancel_async()
{
    egos::prints("cancel_async\n");
    assert(_is_async);
    std::thread *handler = static_cast<std::thread *>(_priv_data);
    assert(handler);
    handler->detach();
};

void timer::wait_sync(int msecs)
{
    egos::prints("wait_sync %u\n", msecs);
    std::chrono::milliseconds delay(msecs);
    std::this_thread::sleep_for(delay);
    _is_async = false;

    _pool->try_return(this);
};

}
