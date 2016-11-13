#ifndef __LOOPER_HPP__
#define __LOOPER_HPP__

#include <list>
#include <string>
#include <mutex>
#include <iostream>

namespace osapi {

class looperIf {

public:
    virtual ~looperIf() {};

    virtual void run() {};

    virtual void post() {};
};

template <typename Tlock, typename Tsync>
class looper : public looperIf {

    typedef Tlock locker;
    locker lock;

    typedef Tsync syncer;
    syncer sync;

    std::list<int> queue;

public:
    looper() {};

    void run() {
        while(true) {
            std::unique_lock<locker> ul(lock);
            std::cout << "Running !" << std::endl;
            if (!queue.empty()) {
                auto item = queue.front();
                queue.pop_front();
            }

            sync.wait(ul);
        }
    };

    void post() {
         std::lock_guard<locker> lg(lock);
         queue.push_back(0);
         sync.notify_all();
         std::cout << "Post item !" << std::endl;
    };
};

}

#endif
