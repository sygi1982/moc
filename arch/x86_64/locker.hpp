#ifndef __LOCKER_HPP__
#define __LOCKER_HPP__

#include <mutex>

namespace osapi {

class locker {
    std::mutex mutex;

public:
    locker() {};

    void lock() {
        mutex.lock();
    };

    void unlock() {
        mutex.unlock();
    };

    std::mutex& getRaw() {
        return mutex;
    }
};

}

#endif
