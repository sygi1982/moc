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
#include "egos.hpp"
#include "looper.hpp"
#include "workitem.hpp"

#include "locker.hpp"
#include "syncer.hpp"

namespace osapi {

extern "C" {
    extern void debug_print(const char *fmt, ...);
}

void egos::prints(const char *fmt, ...)
{
    debug_print(fmt);
}

void egos::introduce_self(void)
{
    egos::prints("Hello from arm\n");
}

void egos::initialize(int &argc, char **argv)
{
    main_looper =
        std::unique_ptr<looper<locker, syncer<locker>, workitem>>(
            new looper<locker, syncer<locker>, workitem>);
}

void egos::start()
{
    main_looper->run();
};

void egos::parse_opts(const char *app)
{
}

}
