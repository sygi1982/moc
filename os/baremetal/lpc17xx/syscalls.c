/*
 *   Copyright (C) 2016 Grzegorz Sygieda
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void dbg_channel_init();
extern int dbg_channel_write(const char *, int);
extern int dbg_channel_read(char *, int);

void _llprint(const char *str)
{
    dbg_channel_write(str, strlen(str));
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
    dbg_channel_init();
    return 0;
}

int _write(int fd, char *ptr, int len)
{
    dbg_channel_write(ptr, len);
    return len;
}

int _lseek (int file, int ptr, int dir)
{
    return 0;
}

int _read (int fd, char *ptr, int len)
{
    dbg_channel_read(ptr, len);

    return len;
}

int _close (int file)
{
    return 0;
}

#ifdef __cplusplus
}
#endif
