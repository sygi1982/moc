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
#ifndef __LOCKER_HPP__
#define __LOCKER_HPP__

#include <mutex>

namespace osapi {

class locker {
    std::mutex mutex;

public:
    locker() {};

    void lock() {
        mutex.lock();
    };

    void unlock() {
        mutex.unlock();
    };

    std::mutex& get_raw() {
        return mutex;
    }
};

}

#endif
