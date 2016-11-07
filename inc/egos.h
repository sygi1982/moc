#ifndef __EGOS_H__
#define __EGOS_H__

#include <vector>
#include <string>

namespace osapi {

class egos {
    egos() {};

    class options {
    public:
        std::vector<std::string> rawTokens;

        bool verbose;
        bool interactiveMode;
       // std::string ioPorts[2];

        bool checkOpt(const std::string &opt);

        const std::string getOptArg(const std::string &opt);
    } opts;

    void parseOpts(int &argc, char **argv);

public:
    egos(egos const&) = delete;
    void operator=(egos const&) = delete;

    void initialize(int &argc, char **argv);

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

    class exception {
        int code;
    public:
        exception() : code(0) {;};
        exception(int code) : code(code) {;};
        int getCode() { return code; };
    };
};

}

#endif
