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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  
void main(void) {
    char data[20];
    initUSART4();
    LCD_Init();
    LCD_setline(0);
    ADC_init();
    while (1){
    ADC2String(data,ADC_getval());//Get ADC val
    sendStringSerial4(data); //Send ADC VAL to realterm program
    __delay_ms(1000);
    LCD_sendbyte(DCLEAR,0); //clear LCD
}
}
