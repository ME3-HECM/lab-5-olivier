#include <xc.h>
#include "serial.h"
#include <stdio.h>
#include "LCD.h"

void initUSART4(void) {
    TRISCbits.TRISC4=1; //Set c4 as input
    TRISCbits.TRISC1=1; //Set c1 as input
    RC0PPS = 0x12; // Map EUSART4 TX to RC0
    RX4PPS = 0x11; // RX is RC1   
    BAUD4CONbits.BRG16 = 0; 	//set baud rate scaling
    TX4STAbits.BRGH = 0; 		//high baud rate select bit
    //set baud rate 64*10^6*(64*(SP4BGRL+1)) where 64*10^6 is Fosc
    SP4BRGL = 51; 			
    SP4BRGH = 0;			//not used

    RC4STAbits.CREN = 1; 		//enable continuos reception
    TX4STAbits.TXEN = 1; 		//enable transmitter
    RC4STAbits.SPEN = 1; 		//enable serial port
	//code to set up USART4 for Reception and Transmission =
	//see readme for detials
}

//function to wait for a byte to arrive on serial port and read it once it does 
// This takes data input from the PC: RC4
char getCharSerial4(void) {
    while (!PIR4bits.RC4IF);//wait for the data to arrive
    return RC4REG; //return byte in RCREG
}

//function to check the TX reg is free and send a byte
//This sends data to the real term program: TX4
void sendCharSerial4(char charToSend) {
    while (!PIR4bits.TX4IF); // wait for flag to be set
    TX4REG = charToSend; //transfer char to transmitter
}


//function to send a string over the serial interface
void sendStringSerial4(char *string){
    // This counts the amount of spaces used 
   // LCD_sendstring(string); //SANity check to see it acc works
    while(*string != 0){  // While the data pointed to isn't a 0x00 do below (strings in C must end with a NULL byte) 
        //Send out the current byte pointed to and increment the pointer
		sendCharSerial4(*string++); 
        __delay_us(20);
	}
}

//functions below are for Ex3 and 4 (optional)

// circular buffer functions for RX
// retrieve a byte from the buffer
char getCharFromRxBuf(void){
    if (RxBufReadCnt>=RX_BUF_SIZE) {RxBufReadCnt=0;} 
    return EUSART4RXbuf[RxBufReadCnt++];
}

// add a byte to the buffer
void putCharToRxBuf(char byte){
    if (RxBufWriteCnt>=RX_BUF_SIZE) {RxBufWriteCnt=0;}
    EUSART4RXbuf[RxBufWriteCnt++]=byte;
}

// function to check if there is data in the RX buffer
// 1: there is data in the buffer
// 0: nothing in the buffer
char isDataInRxBuf (void){
    return (RxBufWriteCnt!=RxBufReadCnt);
}

// circular buffer functions for TX
// retrieve a byte from the buffer
char getCharFromTxBuf(void){
    if (TxBufReadCnt>=TX_BUF_SIZE) {TxBufReadCnt=0;} 
    return EUSART4TXbuf[TxBufReadCnt++];
}

// add a byte to the buffer
void putCharToTxBuf(char byte){
    if (TxBufWriteCnt>=TX_BUF_SIZE) {TxBufWriteCnt=0;}
    EUSART4TXbuf[TxBufWriteCnt++]=byte;
}

// function to check if there is data in the TX buffer
// 1: there is data in the buffer
// 0: nothing in the buffer
char isDataInTxBuf (void){
    return (TxBufWriteCnt!=TxBufReadCnt);
}

//add a string to the buffer
void TxBufferedString(char *string){
	//Hint: look at how you did this for the LCD lab 
      while(*string != 0){  // While the data pointed to isn't a 0x00 do below (strings in C must end with a NULL byte) 
        //Send out the current byte pointed to and increment the pointer
		putCharToTxBuf(*string++); 
	}
}

//initialise interrupt driven transmission of the Tx buf
//your ISR needs to be setup to turn this interrupt off once the buffer is empty
void sendTxBuf(void){
    if (isDataInTxBuf()) {PIE4bits.TX4IE=1;//enable the TX interrupt to send dat
    DataFlag=1;}//data flag just signifies for other scripts that there is data in buff or not
    else { 
        DataFlag=0;
    }
}