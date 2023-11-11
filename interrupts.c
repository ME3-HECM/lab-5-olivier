#include <xc.h>
#include "interrupts.h"
#include "serial.h"
#include "LCD.h"
/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
    //RC4 interrupt Enable bit for serial port and TX4 FOR TRANSMITTER   
    PIE4bits.RC4IE=1; //the E at the end is for enable
    
    INTCONbits.IPEN=1;//Enable priority level setting
    //This sets the priority to high  interrupt  bit
    //IPR4bits.RC4IP=1;// The P at the end is for priority and
   // IPR4bits.TX4IP=1;
    
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
// When reg is cleared this flag is set and the ISR 
{  if (PIR4bits.TX4IF) {
    //This sends out the current pointed char in the buffer
    //and by doing so to TX4 REG it clears the flag
            TX4REG=getCharFromTxBuf();
        } 
    if (DataFlag&0){
        //when buffer is empty turn off the interrupt so the ISR isnt called
             PIE4bits.TX4IE=0;
    } 
//When dTA IS RECEIVED FROM PC run this
if (PIR4bits.RC4IF) {
    //reads the rc4 register which clears the flag
    putCharToRxBuf(RC4REG);
    //send out the byte to LCD screen
    LCD_sendbyte(getCharFromRxBuf(),1);
}

}


