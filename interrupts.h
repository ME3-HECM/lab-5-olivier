#ifndef _interrupts_H
#define _interrupts_H
#define LED LATHbits.LATH3
#define SLEEP_CHECK LATDbits.LATD7

#include <xc.h>

#define _XTAL_FREQ 64000000
extern volatile char DataFlag;
void Interrupts_init(void);
void __interrupt(high_priority) HighISR();

#endif
