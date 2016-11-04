#ifndef __EGOS_H__
#define __EGOS_H__

namespace osapi {

class egos {
    egos() {}

public:
    egos(egos const&) = delete;
    void operator=(egos const&) = delete;

    static egos& getInstance()
    {
        static egos inst;
        return inst;
    }

    static void introduceSelf(void);

    void waitForEvent(int timeoutMsec);

    void* getWorkItem();
};

}

#endif
