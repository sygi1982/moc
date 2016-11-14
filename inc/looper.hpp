#ifndef __LOOPER_HPP__
#define __LOOPER_HPP__

#include <list>
#include <string>
#include <mutex>
#include <cassert>
#include <iostream>

namespace osapi {

template <typename Titem>
class looperIf {

public:
    virtual ~looperIf() {};

    virtual void run() {};
    virtual void stop() {};
    virtual void post(Titem& item) {};
};

template <typename Tlock, typename Tsync, typename Titem>
class looper : public looperIf<Titem> {

    typedef Tlock locker;
    typedef Tsync syncer;

    locker lock;
    syncer sync;
    std::list<Titem> queue;
    bool stopped;

public:
    looper() : stopped(true) {};

    void run() {
        assert(stopped == true);
        stopped = false;

        while(!stopped) {
            lock.lock();
            std::cout << "Running !" << std::endl;
            while (queue.empty()) {
                  sync.wait(lock);
            }
            auto item = queue.front();
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

    void post(Titem& item) {
         std::lock_guard<locker> lg(lock);

         queue.push_back(item);
         sync.wake();
         std::cout << "Post item !" << std::endl;
    };
};

}

#endif
