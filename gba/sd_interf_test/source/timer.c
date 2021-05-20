#include "timer.h"

vu32 msec = 0;

void timer_init(void){
    REG_TM0D = -0x418C; // 16.78MHz / 16780 == 1kHz;
    REG_TM0CNT = TM_F1 | TM_IRQ | TM_EN;

    irqSet(IRQ_TIMER0, timer0_isr);
    irqEnable(IRQ_TIMER0);
}

vu32 millis(){
    return msec;
}

void timer0_isr(){
    msec++;
}