#ifndef __SYNCER_HPP__
#define __SYNCER_HPP__

namespace osapi {

class syncer {

public:
    syncer() {};

    void wait() {
        // call asm("WFI")
    };

    void wake() {
        // nothing to do
    };
};

}

#endif
