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
#ifndef __LOOPER_HPP__
#define __LOOPER_HPP__

#include <list>
#include <memory>
#include <cassert>

#include "utils.hpp"
#include "workitem.hpp"

namespace osapi {

using namespace utils;

typedef autoptr<workitem> autoitem;

class looper_if {
public:
    virtual ~looper_if() {};

    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void post(autoitem item) = 0;

};

template <typename Tlock, typename Tsync>
class looper : public looper_if {
    typedef Tlock locker;
    typedef Tsync syncer;

    locker _lock;
    syncer _sync;
    std::list<autoitem> _queue;
    bool _stopped;

public:
    looper() : _stopped(true) {};

    void run() {
        assert(_stopped == true);

        _stopped = false;

        while(true) {

            _sync.wait(_lock, [this]() {
                return !_queue.empty() || _stopped;
            });

            {
                guard<Tlock> lk(_lock);

                if (_stopped)
                    break;

                if (_queue.empty())
                    assert(false); // bug

                auto item = _queue.front();
                lk.unlock();

                item->utilize();

                lk.lock();
                /* We may stopped and cleared the
                   queue during item utilisation */
                if (!_queue.empty())
                    _queue.pop_front();
            }
        };
    }

    void stop() {
         assert(_stopped == false);
         {
             guard<Tlock> lk(_lock);

             _stopped = true;
             _queue.clear();
         }
         _sync.wake();
    }

    void post(autoitem item) {
         {
             guard<Tlock> lk(_lock);

             _queue.push_back(item);
         }
         _sync.wake();
    }

};

}

#endif
