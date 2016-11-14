#ifndef __LOOPER_HPP__
#define __LOOPER_HPP__

#include <list>
#include <string>
#include <mutex>
#include <cassert>
#include <iostream>

namespace osapi {

class looperIf {

public:
    virtual ~looperIf() {};

    virtual void run() {};
    virtual void stop() {};
    virtual void post() {};
};

template <typename Tlock, typename Tsync>
class looper : public looperIf {

    typedef Tlock locker;
    typedef Tsync syncer;

    locker lock;
    syncer sync;
    std::list<int> queue;
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
                  std::unique_lock<locker> ul(lock);
                  sync.wait(ul);
            }
            auto item = queue.front();
            queue.pop_front();
            lock.unlock();
        }
    };

    void stop() {
         std::lock_guard<locker> lg(lock);

         stopped = true;
         sync.notify_all();
    }

    void post() {
         std::lock_guard<locker> lg(lock);

         queue.push_back(0);
         sync.notify_all();
         std::cout << "Post item !" << std::endl;
    };
};

}

#endif
