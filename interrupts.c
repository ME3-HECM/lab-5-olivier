#include <xc.h>
#include "interrupts.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
    //comparator 1 interrupt Enable bit, Enables Timer BIT
    PIE0bits.TMR0IE=1; //the E at the end is for enable
    
     // setup pin for output (connected to LED)
    LATDbits.LATD7=0;   //set initial output state this is the LED 
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    
    //This sets the priority to high for Timer 0 interrupt  bit
    IPR0bits.TMR0IP=1;// The P at the end is for priority and
    
    INTCONbits.PEIE=1;//eNABLE PERIPHERAL INTERRUPTS
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
               //add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if (PIR0bits.TMR0IF) {
            //update happens when low reg is written to
        //Set to 3035 starting val because this means the amount of ticks 
        //before overflow is just enough to overflow every second
             TMR0H=0b00001011;            //write High reg first, update happens when low reg is written to
             TMR0L=0b11011011;
            PIR0bits.TMR0IF=0; //CLEAR THE INTERRUPT FLAG SO IT CAN BE SET again 
            LATDbits.LATD7=!LATDbits.LATD7;
            //signify by a change in light
    }  
}


