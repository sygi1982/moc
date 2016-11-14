#ifndef __SYNCER_HPP__
#define __SYNCER_HPP__

#include <mutex>
#include <condition_variable>

namespace osapi {

template <typename Tlock>
class syncer {
    std::condition_variable cond;

public:
    syncer() {};

    void wait(Tlock& lock) {
        std::unique_lock<std::mutex> ul(lock.get_raw());
        cond.wait(ul);
    };

    void wake() {
        cond.notify_all();
    };
};

}

#endif
