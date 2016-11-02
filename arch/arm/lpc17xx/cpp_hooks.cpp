#include <stdlib.h>
#include <new>
#include <memory>
#include <stdexcept>
#include <exception>

void *operator new(size_t size) {
#ifndef CPP_NO_HEAP
    void *ptr = malloc(size);

    if (ptr == 0)
        throw std::bad_alloc();

    return ptr;
#else
    return (void *)0;
#endif
}

void operator delete(void *p) {
#ifndef CPP_NO_HEAP
    free(p);
#endif
}

void *operator new[](size_t size) {
#ifndef CPP_NO_HEAP
    void *ptr = malloc(size);

    if (ptr == 0)
        throw std::bad_alloc();

    return ptr;
#else
    return (void *)0;
#endif
}

void operator delete[](void *p) {
#ifndef CPP_NO_HEAP
    free(p);
#endif
}

extern "C" int __aeabi_atexit(void *object,
        void (*destructor)(void *),
        void *dso_handle)
{
    return 0;
}

