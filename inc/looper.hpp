#ifndef __LOOPER_HPP__
#define __LOOPER_HPP__

#include <list>
#include <string>
#include <mutex>
#include <memory>
#include <cassert>
#include <iostream>

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
            while (queue.empty()) {
                  sync.wait(lock);
            }
            auto item = queue.front();
            std::cout << "Running " << item->id << std::endl;
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
         std::cout << "Post item " << item->id << std::endl;
    };
};

}

#endif
