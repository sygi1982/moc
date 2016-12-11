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
#include "hwtmr.hpp"

namespace osapi {

using namespace halapi;

timer::timer() : _is_async(false),
                 _priv_data(nullptr)
{
    _priv_data = static_cast<void *>(new hwtmr(_id));
    assert(_priv_data);
};

timer::~timer()
{
    hwtmr *tmr = static_cast<hwtmr *>(_priv_data);
    if (tmr)
        delete tmr;
};

void timer::wait_async(int msecs, std::function<void()> delegate)
{
    egos::prints("wait_async %u\n", msecs);
    _is_async = true;
    hwtmr *tmr = static_cast<hwtmr *>(_priv_data);
    tmr->start(msecs, delegate);
};

void timer::cancel_async()
{
    egos::prints("cancel_async\n");
    assert(_is_async);
    hwtmr *tmr = static_cast<hwtmr *>(_priv_data);
    tmr->stop();
};

void timer::wait_sync(int msecs)
{
    egos::prints("wait_sync %u\n", msecs);
    _is_async = false;
    // TODO: simple busy loop wait

    _pool->try_return(this);
};

}