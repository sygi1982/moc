#ifndef __SYNCER_HPP__
#define __SYNCER_HPP__

namespace osapi {

template <typename Tlock>
class syncer {

public:
    syncer() {};

    void wait(Tlock& lock) {
        // call asm("WFI")
    };

    void wake() {
        // nothing to do
    };
};

}

#endif
