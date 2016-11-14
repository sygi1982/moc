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
#ifndef __SYNCER_HPP__
#define __SYNCER_HPP__

#include <mutex>
#include <condition_variable>

namespace osapi {

template <typename Tlock>
class syncer {
    std::condition_variable cond;

public:
    syncer() {};

    void wait(Tlock& lock) {
        std::unique_lock<std::mutex> ul(lock.get_raw());
        cond.wait(ul);
    };

    void wake() {
        cond.notify_all();
    };
};

}

#endif
