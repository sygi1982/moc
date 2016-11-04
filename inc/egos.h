#ifndef __EGOS_H__
#define __EGOS_H__

#include <vector>
#include <string>

namespace osapi {

class egos {
    egos() {};

    std::vector<std::string> opts;

public:
    egos(egos const&) = delete;
    void operator=(egos const&) = delete;

    void initialize(void);

    void parseOpts(int &argc, char **argv);

    void waitForEvent(int timeoutMsec);

    void* getWorkItem();

    /* Static methods */
    static egos& getInstance()
    {
        static egos inst;
        return inst;
    }

    static void prints(const char *fmt, ...);

    static void introduceSelf(void);

};

}

#endif
