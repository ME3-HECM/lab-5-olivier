/*
 * File:   main.c
 * Author: olive
 *
 * Created on 12 February 2023, 14:49
 */


#include <xc.h>
#include "serial.h"
#include "LCD.h"

void main(void) {
    initUSART4();
    LCD_Init();
}
