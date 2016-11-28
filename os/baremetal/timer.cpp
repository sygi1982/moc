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
#include <functional>
#include <cassert>

#include "egos.hpp"
#include "timerpool.hpp"

namespace osapi {

struct timerdat {
    int tim; // TODO: put hw timer here
};

timer::timer() : _is_async(false),
                 _priv_data(nullptr)
{
    _priv_data = reinterpret_cast<void *>(new timerdat);
    assert(_priv_data);
};

timer::~timer()
{
    timerdat *data = reinterpret_cast<timerdat *>(_priv_data);
    if (data)
        delete data;
};

void timer::wait_async(int msecs, std::function<void()> delegate)
{
    egos::prints("wait_async %u\n", msecs);
    _is_async = true;
    timerdat *data = reinterpret_cast<timerdat *>(_priv_data);
    // TODO: start hw timer
};

void timer::cancel_async()
{
    egos::prints("cancel_async\n");
    assert(_is_async);
    timerdat *data = reinterpret_cast<timerdat *>(_priv_data);
    // TODO: cancel hw timer
};

void timer::wait_sync(int msecs)
{
    egos::prints("wait_sync %u\n", msecs);
    _is_async = false;
    // TODO: sleep with hw timer

    _pool->try_return(this);
};

}
