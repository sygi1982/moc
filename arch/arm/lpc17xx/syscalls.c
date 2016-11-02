#include <sys/stat.h>
#include <stdio.h>

#include "target.h"
#include "compiler.h"
#include "ft245.h"

#ifdef __cplusplus
extern "C" {
#endif

void _llprint(const char *str)
{
    int i = 0;

    while (str[i]) {
        if (ft245_write(str[i]) == TRUE)
            i++;
        else
            continue;
    }
}

int _kill (int pid, int sig)
{
    _llprint("\nKill called! This is bad situation!");
    (void)pid; (void)sig;
    return 0;
}

void _exit (int status)
{
    _kill(status, -1);
}

int _getpid (int n)
{
    return 1;
}

char *heap_end = 0;
caddr_t _sbrk(int incr) {
   extern char __heap_base__; /* Defined by the linker */
   extern char __heap_limit__; /* Defined by the linker */
   char *prev_heap_end;

   if (heap_end == 0) {
       heap_end = &__heap_base__;
   }
   prev_heap_end = heap_end;

   if (heap_end + incr > &__heap_limit__) {
       /* Heap and stack collision */
       _llprint("\n Heap & stack collision!!!");
       return (caddr_t)0;
   }

   heap_end += incr;
   return (caddr_t) prev_heap_end;
}

int _isatty (int fd)
{
    return (fd <= 2) ? 1 : 0;  /* one of stdin, stdout, stderr */
}

int _fstat (int fd, struct stat *st)
{
    ft245_init();
    return 0;
}

int _write(int fd, char *ptr, int len)
{
    int i = 0;

    while (i < len) {
        char val = ptr[i];
        if (ft245_write(val) == TRUE)
            i++;
        else
            continue;
    }

    return len;
}

int _lseek (int file, int ptr, int dir)
{
    return 0;
}

int _read (int fd, char *ptr, int len)
{
    int i = 0;

    while (i < len) {
        char *pval = &ptr[i];
        if (ft245_read((unsigned char *)pval) == TRUE)
            i++;
        else
            continue;
    }

    return 0;
}

int _close (int file)
{
    return 0;
}

#ifdef __cplusplus
}
#endif
