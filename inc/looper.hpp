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
#include <mutex>
#include <memory>
#include <cassert>

namespace osapi {

template <typename Titem>
class looper_if {

public:
    virtual ~looper_if() {};

    virtual void run() {};
    virtual void stop() {};
    virtual void post(std::shared_ptr<Titem> &item) {};
};

template <typename Tlock, typename Tsync, typename Titem>
class looper : public looper_if<Titem> {

    typedef Tlock locker;
    typedef Tsync syncer;

    locker lock;
    syncer sync;
    std::list<std::shared_ptr<Titem>> queue;
    bool stopped;

public:
    looper() : stopped(true) {};

    void run() {
        assert(stopped == true);
        stopped = false;

        while(!stopped) {
            lock.lock();

            while (queue.empty())
                sync.wait(lock);

            auto item = queue.front();
            item->utilize();
            queue.pop_front();

            lock.unlock();
        }
    };

    void stop() {
         std::lock_guard<locker> lg(lock);

         stopped = true;
         queue.clear();
         sync.wake();
    };

    void post(std::shared_ptr<Titem> &item) {
         std::lock_guard<locker> lg(lock);

         queue.push_back(item);
         sync.wake();
    };
};

}

#endif
