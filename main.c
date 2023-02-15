/*
 * File:   main.c
 * Author: olive
 *
 * Created on 12 February 2023, 14:49
 */

#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "serial.h"
#include "LCD.h"
#include <stdio.h>
#include "ADC.h"
#include "interrupts.h"
#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  
extern volatile int sendflag;
extern volatile int getflag;


void main(void) {
    char senddata[20];
    initUSART4();
    ADC_init();
    Interrupts_init();
    LCD_Init();
    LCD_setline(0);
    while (1){
        ADC2String(senddata,ADC_getval());
        TxBufferedString(senddata);
         //Check if any data is left in buffer if so enable on interrupt
        //this waits for the entire string to be sent before moving on
       while (DataFlag){
           sendTxBuf();
        }
}
}
