#include <stdio.h>
#include <new>
#include <memory>
#include <stdexcept>
#include <exception>
#include "egos.h"
#include "compiler.h"

using namespace osapi;

int main(int argc, char **argv)
{
    int *ptest(nullptr);
    egos &os_api = egos::getInstance();

    os_api.introduceSelf();

    os_api.initialize();

    os_api.parseOpts(argc, argv);

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

    while(TRUE) {
        os_api.waitForEvent(-1);
    }

    egos::prints("Finished!\n");

    return 0 ;
}
