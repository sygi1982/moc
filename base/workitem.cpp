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

namespace osapi {

workitem::workitem(int id) : _id(id)
{
    egos::prints("workitem %u\n", _id);
}

workitem::~workitem()
{
    egos::prints(" ~workitem %u\n", _id);
}

bool workitem::utilize()
{
    egos::prints(" workitem %u utilize\n", _id);
    return false;
}

int workitem::get_id() const
{
    return _id;
}

int workitem::get_type() const
{
    return -1;
}

}
