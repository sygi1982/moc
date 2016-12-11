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
#ifndef __UTILS_HPP__
#define __UTILS_HPP__

template <typename Tlock>
class guard {
    typedef Tlock locker;

    locker& _lock;

public:
    guard(Tlock& lk) : _lock(lk) {
        lock();
    }

    ~guard() {
        unlock();
    }

    void lock() {
        _lock.acquire();
    }

    void unlock() {
        _lock.release();
    }

};

template <typename Ttype>
class singleton
{
protected:
    inline explicit singleton() {};

public:
    singleton(singleton const&) = delete;
    void operator=(singleton const&) = delete;

    static Ttype& get_instance()
    {
        static Ttype inst;
        return inst;
    }
};

template <typename Ttype>
class autoptr
{
    Ttype* _data;

public:
    autoptr(Ttype *ptr);
    ~autoptr();
    Ttype& operator*();
    Ttype* operator->();
    autoptr();
    autoptr(const autoptr<Ttype>& aptr);
    autoptr<Ttype>& operator=(const autoptr<Ttype>& aptr);

    class holder
    {
        int _clients;

    public:
        holder() : _clients(0) { ; }
        ~holder() { ; }
        void get() { _clients++; }
        bool put() { return --_clients ? false : true; }
        int state() { return _clients; }
    }* _holder;

};

template <typename Ttype>
autoptr<Ttype>::autoptr(Ttype *ptr) : _data(ptr),
                                      _holder(0)
{
    _holder = new holder();
    _holder->get();
}

template <typename Ttype>
autoptr<Ttype>::~autoptr()
{
    if (_holder->put())
    {
        delete _data;
        delete _holder;
        _data = 0;
    }
}

template <typename Ttype>
Ttype& autoptr<Ttype>::operator*()
{
    return *_data;
}

template <typename Ttype>
Ttype* autoptr<Ttype>::operator->()
{
    return _data;
}

template <typename Ttype>
autoptr<Ttype>::autoptr() : _data(0),
                            _holder(0)
{
    // Create a new reference
    _holder = new holder();
    // Increment the reference count
    _holder->get();
}

template <typename Ttype>
autoptr<Ttype>::autoptr(const autoptr<Ttype>& aptr) : _data(aptr._data),
                                                      _holder(aptr._holder)
{
    _holder->get();
}

template <typename Ttype>
autoptr<Ttype>& autoptr<Ttype>::operator=(const autoptr<Ttype>& aptr)
{
    if (this != &aptr)
    {
         if (_holder->put())
         {
             delete _data;
             delete _holder;
         }
         _data = aptr._data;
         _holder = aptr._holder;
         _holder->get();
    }
    return *this;
}

#endif
