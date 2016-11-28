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

namespace osapi {

template <typename Titem>
class looper_if {
public:
    virtual ~looper_if() {};

    virtual void run() {};
    virtual void stop() {};
    virtual void post(autoptr<Titem> item) {};

};

template <typename Tlock, typename Tsync, typename Titem>
class looper : public looper_if<Titem> {
    typedef Tlock locker;
    typedef Tsync syncer;

    locker _lock;
    syncer _sync;
    std::list<autoptr<Titem>> _queue;
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

                auto item = _queue.front();
                lk.unlock();

                item->utilize();

                lk.lock();
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

    void post(autoptr<Titem> item) {
         {
             guard<Tlock> lk(_lock);

             _queue.push_back(item);
         }
         _sync.wake();
    }

};

}

#endif
