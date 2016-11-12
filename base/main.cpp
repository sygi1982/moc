#include <cstdlib>
#include <new>
#include <memory>
#include <stdexcept>
#include <exception>
#include <stdio.h>

#include "egos.hpp"

using namespace osapi;

int main(int argc, char **argv)
{
    int *ptest(nullptr);
    egos &os_api = egos::getInstance();

    try {
        os_api.initialize(argc, argv);
    }
    catch (egos::exception &e) {
        int err = e.getCode();

        if (!err)
            std::exit(0);
        else
            egos::prints("Unexpected error -> %u\n", err);
    }

    os_api.introduceSelf();

    try {
        //ptest = new(std::nothrow) int[10];
        //ptest = new int[10000];
        ptest = new int[10];
    }
    catch (std::bad_alloc &e) {
        egos::prints("\nBad alloc error\n");
    }
    egos::prints("Allocating ptest(int) %p\n", ptest);

    delete[] ptest;
    egos::prints("Deleting ptest(int) %p\n", ptest);
    ptest = NULL;

    os_api.start();

    egos::prints("Finished!\n");
}
