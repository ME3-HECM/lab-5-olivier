#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
    //RC4 interrupt Enable bit for serial port and TX4 FOR TRANSMITTER   
    PIE4bits.RC4IE=1; //the E at the end is for enable
    PIE4bits.TX4IE=1; 
    
    INTCONbits.IPEN=1;//Enable priority level setting
    //This sets the priority to high  interrupt  bit
    IPR4bits.RC4IP=1;// The P at the end is for priority and
    IPR4bits.TX4IP=1;
    
    INTCONbits.PEIE=1;//ENABLE PERIPHERAL INTERRUPTS
    //Interrupt control INTCON register, enables all interrupts globally
    INTCONbits.GIE=1;
    // It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
}

/************************************
* High priority interrupt service routine
* Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{
    //check the TX reg is free and send a byte
    if (PIR0bits.TMR0IF) {
        TX4REG = charToSend; //transfer char to transmitter
    }  
}


