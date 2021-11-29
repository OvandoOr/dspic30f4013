/*
 * File:   main_frec.c
 * Author: MarthaAlicia
 *
 * Created on 26 de junio de 2018, 06:20 PM
 */


#include "xc.h"
#include<p30F4013.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#define FCY (unsigned long)7372800*4   //add your operating frequency 29,491,200
#define UART1_BAUD 9600
#define UART2_BAUD 9600
#define UBRG1_VALUE (FCY/UART1_BAUD)/16 - 1
#define UBRG2_VALUE (FCY/UART2_BAUD)/16 - 1

void InitUART1(void);
void WriteUART1(unsigned int data);
void WriteUART1dec2string(unsigned int data);
void WriteStringUART1(const char * s);//for strings
void InitUART2(void);
void WriteUART2(unsigned int data);
void WriteUART2dec2string(unsigned int data);
void WriteStringUART2(const char * s);

void retardo (int t);//INSTANCIA DECLARACION DE LA FUNCION
void reloj(void);
void ADC_INI(void);
void cambio_tiempo(int div,int format,char type);


unsigned int i,j,estado=0,hora=0,minutos=0,segundos=0; //variables for getting loop delay
int tempRX, tempRX2, lectura, lectura_ant;
unsigned long ant_cont=0,cont=0,tiempo,tiempo_ant;
double ticks=0;
char buf[20];
char timer[20];

//-----------------------------------------------------------------
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;
    tempRX = U1RXREG;
}
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    IFS1bits.U2RXIF = 0;
    tempRX2 = U2RXREG;
}
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    cont++;
    if(cont>=86400)cont=0;
    _T3IF=0;
}

void main(void)
{
//TRISB=0x0000;//definir pins b como salidas/ salidas=0 entradas=1
    
//ADPCFG=0xFFFF;// definir pins b como digital 1=digital 0=analogico
//LATB=0x0000;//poner los leds en estado apagado
_TRISD0=1;
_RD0=0;
//TIMER 2/3
_T3IP=6;
TMR2=0;
TMR3=0;
PR3=450;//452
T2CONbits.T32=1;
T2CONbits.TCKPS=0;
T2CONbits.TCS=0;
T2CONbits.TON=1;
_T3IF=0;
_T3IE=1;
ADC_INI();
InitUART2();
while(1)
{
    switch(estado)
    {
        case 0:
            reloj();
            break;
        case 1:
                cambio_tiempo(170,3600,'h');
//                _T3IE=0;
//                while(!_DONE);
//                    _DONE = 0;
//                lectura= ADCBUF0;
//                cont+=(lectura/170)*3600;
            break;
        case 2:
                cambio_tiempo(68,60,'m');
//                _T3IE=0;
//                while(!_DONE);
//                    _DONE = 0;
//                lectura= ADCBUF0;
//                cont+=(lectura/68)*60;
            break;
        default:
                cambio_tiempo(68,1,'s');
//                _T3IE=0;
//                while(!_DONE);
//                    _DONE = 0;
//                lectura= ADCBUF0;
//                cont+=(lectura/68);
            break;
    }
    if(!_RD0)
    {
        retardo(500);
        if(!_RD0)
        {
            _T3IE=1;
            estado++;
            if(estado>4)estado=0;
        }
            
    }
    
}//main
}

void retardo(int t){
        for(i=0;i<t;i++) //random delay
        for(j=0;j<2000;j++);
}

void cambio_tiempo(int div,int format,char type){
        if(_T3IE)
        {
            switch(type)
            {
                case 's':
                    cont-=segundos;
                    break;
                case 'm':
                    cont-=(minutos*60);
                    break;
                case 'h':
                    cont-=(hora*3600);
                    break;
                default:
                    break;
            }
            _T3IE=0;
        }
        while(!_DONE);
            _DONE = 0;
        lectura= ADCBUF0;

        if(abs(tiempo-tiempo_ant)>5)
        {
            lectura=tiempo/div;
            if(lectura_ant!=lectura)
            {
                cont+=(lectura-lectura_ant)*format;
                lectura_ant=lectura;
                reloj();
                retardo(500);
            }
            tiempo_ant=tiempo;
        }
}

void reloj(void){
    if(cont!=ant_cont)
    {
        hora=cont/3600;//72563=20
        minutos=(cont%3600)/60;//09
        segundos=(cont%60);//22.999
        struct tm t;
            t.tm_sec=segundos;
            t.tm_min=minutos;
            t.tm_hour=hora;
        
        strftime(timer, sizeof(timer), "%I:%M:%S %p\n", &t);
        /*
        if(hora>12)
            sprintf(buf, "%d:%2d:%2d p.m.\n", hora-12,minutos,segundos);
        else if(hora==12)
            sprintf(buf, "%d:%2d:%2d p.m.\n", hora,minutos,segundos);
        else
            sprintf(buf, "%d:%2d:%2d a.m.\n", hora,minutos,segundos);
        */
        WriteStringUART2(timer);
        //WriteStringUART2(buf);
        ant_cont=cont;
    }
}

void ADC_INI(void)
{
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
}

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
