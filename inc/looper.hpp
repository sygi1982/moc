#ifndef __LOOPER_HPP__
#define __LOOPER_HPP__

#include <vector>
#include <map>
#include <string>
#include <iostream>

namespace osapi {

class looperIf {

public:
    virtual ~looperIf() {};

    virtual void run() {};
};

class looper : public looperIf {

public:
    looper() {};

    void run() { std::cout << "Running !" << std::endl; };
};

}

#endif
