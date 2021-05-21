#ifndef __GBA_TIMER_H
#define __GBA_TIMER_H

#include <gba_types.h>
#include <gba_interrupt.h>

#define REG_TM0D *((vu16 *)0x4000100)
#define REG_TM1D *((vu16 *)0x4000104)
#define REG_TM2D *((vu16 *)0x4000108)
#define REG_TM3D *((vu16 *)0x400010C)

#define REG_TM0CNT *((vu16 *)0x4000102)
#define REG_TM1CNT *((vu16 *)0x4000106)
#define REG_TM2CNT *((vu16 *)0x400010A)
#define REG_TM3CNT *((vu16 *)0x400010E)

#define TM_F1    0x0
#define TM_F64   0x1
#define TM_F256  0x2
#define TM_F1024 0x3

#define TM_CASCADE      (1 << 2)
#define TM_IRQ          (1 << 6)
#define TM_EN           (1 << 7)

extern vu32 msec;

void timer_init(void);
vu32 millis(void);

void timer0_isr(void);

#endif
