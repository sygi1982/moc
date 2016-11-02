#ifndef __COMPILER_H
#define __COMPILER_H

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef STATUS_OK
#define STATUS_OK (1)
#endif

#ifndef STATUS_ERROR
#define STATUS_ERROR (0)
#endif

#ifndef TRUE
#define TRUE     (1)
#endif

#ifndef FALSE
#define FALSE    (0)
#endif

#ifndef ENABLE
#define ENABLE   (1)
#endif

#ifndef DISABLE
#define DISABLE  (0)
#endif

#ifndef HIGH
#define HIGH     (1)
#endif

#ifndef LOW
#define LOW      (0)
#endif

#define ON       (1)
#define OFF      (0)

#ifndef EMPTY
#define EMPTY    (0)
#endif

typedef unsigned char        Bool;
typedef unsigned char        U8;
typedef signed char          S8;
typedef unsigned int         U16;
typedef signed int           S16;
typedef unsigned long int    U32;
typedef signed long int      S32;
typedef unsigned short       US16;

typedef unsigned char        BYTE;
typedef unsigned short       WORD;
typedef unsigned long        DWORD;
typedef unsigned int         BOOL;


typedef union {
    unsigned int byte;
    struct {
        unsigned int b0 :1;
        unsigned int b1 :1;
        unsigned int b2 :1;
        unsigned int b3 :1;
        unsigned int b4 :1;
        unsigned int b5 :1;
        unsigned int b6 :1;
        unsigned int b7 :1;
    } bit;
} Bits_8;

typedef union {
    unsigned int dbyte;
    struct {
        unsigned int b0 :1;
        unsigned int b1 :1;
        unsigned int b2 :1;
        unsigned int b3 :1;
        unsigned int b4 :1;
        unsigned int b5 :1;
        unsigned int b6 :1;
        unsigned int b7 :1;
        unsigned int b8 :1;
        unsigned int b9 :1;
        unsigned int b10 :1;
        unsigned int b11 :1;
        unsigned int b12 :1;
        unsigned int b13 :1;
        unsigned int b14 :1;
        unsigned int b15 :1;
    } bit;
} Bits_16;

typedef union {
    unsigned int word;
    struct {
        unsigned int b0 :1;
        unsigned int b1 :1;
        unsigned int b2 :1;
        unsigned int b3 :1;
        unsigned int b4 :1;
        unsigned int b5 :1;
        unsigned int b6 :1;
        unsigned int b7 :1;
        unsigned int b8 :1;
        unsigned int b9 :1;
        unsigned int b10 :1;
        unsigned int b11 :1;
        unsigned int b12 :1;
        unsigned int b13 :1;
        unsigned int b14 :1;
        unsigned int b15 :1;
        unsigned int b16 :1;
        unsigned int b17 :1;
        unsigned int b18 :1;
        unsigned int b19 :1;
        unsigned int b20 :1;
        unsigned int b21 :1;
        unsigned int b22 :1;
        unsigned int b23 :1;
        unsigned int b24 :1;
        unsigned int b25 :1;
        unsigned int b26 :1;
        unsigned int b27 :1;
        unsigned int b28 :1;
        unsigned int b29 :1;
        unsigned int b30 :1;
        unsigned int b31 :1;
    } bit;
} Bits_32;

#endif /* __COMPILER_H */

