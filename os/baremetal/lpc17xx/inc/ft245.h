#ifndef __FT245_H__
#define __FT245_H__

#ifdef __cplusplus
extern "C" {
#endif

void ft245_init(void);
unsigned char ft245_write(unsigned int);
unsigned char ft245_read(unsigned char *);

#ifdef __cplusplus
}
#endif

#endif



