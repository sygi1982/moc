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
#include <cstdlib>
#include <new>
#include <memory>
#include <stdexcept>
#include <exception>
#include <stdio.h>

#include "egos.hpp"
#include "timerpool.hpp"

using namespace osapi;

int main(int argc, char **argv)
{
    egos &os_api = egos::get_instance();

    try {
        os_api.initialize(argc, argv);
    } catch (egos::exception &e) {
        int err = e.get_code();

        if (!err)
            std::exit(0);
        else
            egos::prints("Unexpected error -> %u\n", err);
    }

    os_api.introduce_self();

    try {
        std::shared_ptr<workitem> item(new workitem(1));
        os_api.process(item);
    } catch (std::bad_alloc &e) {
        egos::prints("\nBad alloc error\n");
    }
    os_api.start();

    egos::prints("Finished!\n");
}
