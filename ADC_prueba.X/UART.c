/*
 * File:   UART.c
 * Author: MarthaAlicia
 *
 * Created on 11 de julio de 2018, 09:33 PM
 */


#include "UART.h"
#include <p30f4013.h>

void InitUART1(void)
{
    U1BRG=UBRG1_VALUE; ///baud rate 115200
    U1MODEbits.ALTIO=0;
    IEC0bits.U1RXIE=1;
    U1STA&=0xfffc;
    U1MODEbits.UARTEN=1;
    U1STAbits.UTXEN=1;
}

void WriteUART1(unsigned int data)
{
    while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}
void WriteUART1dec2string(unsigned int data)
{
    unsigned char temp;
    temp=data/1000;
    WriteUART1(temp+'0');
    data=data-temp*1000;
    temp=data/100;
    WriteUART1(temp+'0');
    data=data-temp*100;
    temp=data/10;
    WriteUART1(temp+'0');
    data=data-temp*10;
    WriteUART1(data+'0');
}
void WriteStringUART1(const char * s)
{
    while(*s)
            WriteUART1(*s++);
}
//UART2
void InitUART2(void)
{
    U2BRG = UBRG2_VALUE; // baud rate 9600
    U2STA &= 0xfffc;
    IEC1bits.U2RXIE = 1; // enable RX2 interrupt
    U2MODEbits.UARTEN = 1; // UARTEN: UART Enable bit
    U2STAbits.UTXEN = 1; // transmition ON
}
void WriteUART2 (unsigned int data)
{
    U2TXREG = data;
    while(!U2STAbits.TRMT){}
}
void WriteUART2dec2string(unsigned int data)
{
    unsigned char temp;
    temp=data/1000;
    WriteUART2(temp+'0');
    data=data-temp*1000;
    temp=data/100;
    WriteUART2(temp+'0');
    data=data-temp*100;
    temp=data/10;
    WriteUART2(temp+'0');
    data=data-temp*10;
    WriteUART2(data+'0');
}
void WriteStringUART2(const char * s)
{
    while(*s)
            WriteUART2(*s++);
} 
