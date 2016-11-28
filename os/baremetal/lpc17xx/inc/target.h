#ifndef __TARGET_H__
#define __TARGET_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PLL0_MValue            11    //49
#define PLL0_NValue            0     //2
#define CCLKDivValue           3

#define Fosc     12000000
#define Fcclk    72000000        //100000000
#define Fcco    288000000        //400000000

#define Fpclk    (Fcclk / 4)

extern void target_reset(void);

#ifdef __cplusplus
}
#endif
#endif /* end __TARGET_H */
