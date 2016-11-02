#ifndef __FT245_H__
#define __FT245_H__

#ifdef __cplusplus
extern "C" {
#endif

#define FT245_TXE  (FIO1PIN&(1<<28))  //P1
#define FT245_RXF  (FIO1PIN&(1<<29))  //P1
#define FT245_WR   (1<<26) //P3
#define FT245_RD   (1<<25) //P3

void ft245_init(void);
unsigned char ft245_write(unsigned int);
unsigned char ft245_read(unsigned char *);

#ifdef __cplusplus
}
#endif

#endif



