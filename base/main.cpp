#include <stdio.h>
#include <new>
#include <memory>
#include <stdexcept>
#include <exception>
#include "egos.h"

using namespace osapi;

int main(void)
{
    unsigned long i = 0;
    int *ptest(nullptr);
    egos os_api;

    egos::introduce();
    os_api.introduce();

    try {
        //ptest = new(std::nothrow) int[10];
        //ptest = new int[10000];
        ptest = new int[10];
    }
    catch (std::bad_alloc &e) {
        printf("\nBad alloc error\n");
    }
    printf("Allocating ptest(int) %p\n", ptest);

    delete[] ptest;
    printf("Deleting ptest(int) %p\n", ptest);
    ptest = NULL;

    printf("Finished!\n");

    // force the counter to be placed into memory
    volatile static int j = 0 ;

    // Enter an infinite loop, just incrementing a counter
    while(1)
        j++;
    return 0 ;
}
