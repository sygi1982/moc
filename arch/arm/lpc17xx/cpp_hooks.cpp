/*
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
#include <stdlib.h>
#include <new>
#include <memory>
#include <stdexcept>
#include <exception>

extern "C" int __aeabi_atexit(void *object,
        void (*destructor)(void *),
        void *dso_handle)
{
    return 0;
}

void *operator new(size_t size) {
#ifndef CPP_NO_HEAP
    void *ptr = malloc(size);

    if (ptr == 0)
        throw std::bad_alloc();

    return ptr;
#else
    return (void *)0;
#endif
}

void operator delete(void *p) {
#ifndef CPP_NO_HEAP
    free(p);
#endif
}

void *operator new[](size_t size) {
#ifndef CPP_NO_HEAP
    void *ptr = malloc(size);

    if (ptr == 0)
        throw std::bad_alloc();

    return ptr;
#else
    return (void *)0;
#endif
}

void operator delete[](void *p) {
#ifndef CPP_NO_HEAP
    free(p);
#endif
}
