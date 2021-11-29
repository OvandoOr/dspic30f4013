/*
 * File:   main_adc.c
 * Author: MarthaAlicia
 *
 * Created on 1 de julio de 2018, 11:29 PM
 */


#include "xc.h"
#include "UART.h"
#include<p30F4013.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// FOSC
#pragma config FOSFPR = FRC_PLL16
//#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_OFF         // POR Timer Value (Timer Disabled)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

//#define FCY (unsigned long)7372800*4   //add your operating frequency 29,491,200
//#define UART1_BAUD 9600
//#define UART2_BAUD 9600
//#define UBRG1_VALUE (FCY/UART1_BAUD)/16 - 1
//#define UBRG2_VALUE (FCY/UART2_BAUD)/16 - 1
//
//void InitUART1(void);
//void WriteUART1(unsigned int data);
//void WriteUART1dec2string(unsigned int data);
//void WriteStringUART1(const char * s);//for strings
//void InitUART2(void);
//void WriteUART2(unsigned int data);
//void WriteUART2dec2string(unsigned int data);
//void WriteStringUART2(const char * s);

void retardo (int t);//INSTANCIA DECLARACION DE LA FUNCION
unsigned int i,j; //variables for getting loop delay
char buf[20];

int ADResult; //En esta variable se almacena el resultado del ADC

void main(void) {
    
TRISB = 0x0020; // Configura todos como salidas excepto PORTB5
ADPCFGbits.PCFG5 = 0; //Solo el bit 5 como entrada analogica
/*** Configuracion del ADC ***/
ADCON2bits.VCFG = 0; //Selección de niveles de referencia externos
ADCHSbits.CH0SA = 5; //Selección de la entrada a muestrear
ADCON3bits.ADCS = 47; //ADCS = 47, es decir Tad = 1.5117uS 
// Muestreo automatico (al terminar la conversion comienza a muestrear)
ADCON1bits.ASAM = 1; // Indica disparo de muestreo automatico
// Conversion automatica por contador interno
// (empieza a convertir SAMC TADs despues del inicio del muestreo)
ADCON1bits.SSRC = 7; // SSRC=?111? conversion automatica
ADCON3bits.SAMC = 1; // 1 TAD despues del muestreo comienza la conversion
ADCON1bits.ADON = 1; //Activa el modulo ADC

InitUART2();

// Ciclo principal digitaliza el canal 0-5
while(1)
{
    while(!_DONE);
        _DONE = 0;
    ADResult= ADCBUF0;
    sprintf(buf,"%d\n",ADResult);
    WriteStringUART2(buf);
    retardo(100);
}
    
 
}

void retardo(int t){
        for(i=0;i<t;i++) //random delay
        for(j=0;j<2000;j++);
}



//void InitUART1(void)
//{
//    U1BRG=UBRG1_VALUE; ///baud rate 115200
//    U1MODEbits.ALTIO=0;
//    IEC0bits.U1RXIE=1;
//    U1STA&=0xfffc;
//    U1MODEbits.UARTEN=1;
//    U1STAbits.UTXEN=1;
//}
//
//void WriteUART1(unsigned int data)
//{
//    while (U1STAbits.TRMT==0);
//    if(U1MODEbits.PDSEL == 3)
//        U1TXREG = data;
//    else
//        U1TXREG = data & 0xFF;
//}
//void WriteUART1dec2string(unsigned int data)
//{
//    unsigned char temp;
//    temp=data/1000;
//    WriteUART1(temp+'0');
//    data=data-temp*1000;
//    temp=data/100;
//    WriteUART1(temp+'0');
//    data=data-temp*100;
//    temp=data/10;
//    WriteUART1(temp+'0');
//    data=data-temp*10;
//    WriteUART1(data+'0');
//}
//void WriteStringUART1(const char * s)
//{
//    while(*s)
//            WriteUART1(*s++);
//}
////UART2
//void InitUART2(void)
//{
//    U2BRG = UBRG2_VALUE; // baud rate 9600
//    U2STA &= 0xfffc;
//    IEC1bits.U2RXIE = 1; // enable RX2 interrupt
//    U2MODEbits.UARTEN = 1; // UARTEN: UART Enable bit
//    U2STAbits.UTXEN = 1; // transmition ON
//}
//void WriteUART2 (unsigned int data)
//{
//    U2TXREG = data;
//    while(!U2STAbits.TRMT){}
//}
//void WriteUART2dec2string(unsigned int data)
//{
//    unsigned char temp;
//    temp=data/1000;
//    WriteUART2(temp+'0');
//    data=data-temp*1000;
//    temp=data/100;
//    WriteUART2(temp+'0');
//    data=data-temp*100;
//    temp=data/10;
//    WriteUART2(temp+'0');
//    data=data-temp*10;
//    WriteUART2(data+'0');
//}
//void WriteStringUART2(const char * s)
//{
//    while(*s)
//            WriteUART2(*s++);
//} 

//EJEMPLO DE USO DEL ADC CON MUESTREO AUTOMATICO Y CONVERSION AUTOMATICA





