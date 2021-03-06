/*
 * File:   main_ADC_PWM.c
 * Author: MarthaAlicia
 *
 * Created on 11 de julio de 2018, 08:54 PM
 */


#include "xc.h"
#include<p30F4013.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "UART.h"

// FOSC
#pragma config FOSFPR = FRC_PLL16
//#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64         // POR Timer Value (Timer Disabled)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)


#define ECG_pin 5
#define EMG_pin 4
#define gon1_pin 3
#define gon2_pin 2


//duracion de timer = 100uS   
#define ECG_tmr 50
#define EMG_tmr 200
#define tem_tmr 200
#define gon1_tmr 200
#define gon2_tmr 200
#define oxi_tmr 50



void ADC_INI(int pin);
void TM3_INI(void);
void retardo (int t);
int lectura_ADC(int pin);
char buf[20];
int promedio_ADC,cont_ADC=0;

//void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt(void)
//{
//    while(!_DONE);
//           _DONE = 0;
//    promedio_ADC = (ADCBUF0+ADCBUF1+ADCBUF2+ADCBUF3+ADCBUF4)/5;
//    _ADIF=0;
//}
int val_ant=0;
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    if(cont_ADC%ECG_tmr==0){
        sprintf(buf,"%d,",lectura_ADC(ECG_pin));
        WriteStringUART2(buf);
        sprintf(buf,"%d,",lectura_ADC(EMG_pin));
        WriteStringUART2(buf);
        sprintf(buf,"%d,",lectura_ADC(gon1_pin));
        WriteStringUART2(buf);
        sprintf(buf,"%d\n",lectura_ADC(gon2_pin));
        WriteStringUART2(buf);
        cont_ADC=0;
    }
    cont_ADC++;
    _T3IF = 0;
}

int main(void) {
    
    
    ADC_INI(ECG_pin);
    TM3_INI();
    InitUART2();
    TRISC=0x0000;
    LATC=0x0000;
    TRISD=0x000F;//F porque equivale a 1111 en el ultimo byte
    PORTD=0;//definir lectura inicial del dip-switch como 0
    
    while(1)
    {
        LATC=0xFFFF;
        retardo(100);
        LATC=0x0000;
        retardo(100);
    }
    
    return 0;
}

void ADC_INI(int pin)
{
    //TRISB = 0x0020; // Configura todos como salidas excepto PORTB5
    TRISB= 0XFFFF;
    ADCON1bits.ADON = 1;
    ADCON2bits.SMPI=1;
    //_ADIP=6;
    //_ADIF=0;
    ADPCFG = 0; //Solo el bit 5 como entrada analogica
    /*** Configuracion del ADC ***/
    ADCON2bits.VCFG = 0; //Selecci?n de niveles de referencia externos
    ADCHSbits.CH0SA = pin; //Selecci?n de la entrada a muestrear
    //FCY=29,491,200 MIPS, TCY=33.90842 ns, Para 1 muestra cada 20us a 20 TADS, El TAD=1000ns, ADCS=57.997
    ADCON3bits.ADCS = 58; //ADCS = 58, es decir Tad real = 1000.05ns
    // Muestreo automatico (al terminar la conversion comienza a muestrear)
    ADCON1bits.ASAM = 1; // Indica disparo de muestreo automatico
    // Conversion automatica por contador interno
    // (empieza a convertir SAMC TADs despues del inicio del muestreo)
    ADCON1bits.SSRC = 7; // SSRC=?111? conversion automatica
    ADCON3bits.SAMC = 6; // 6 TAD despues del muestreo comienza la conversion
    ADCON1bits.ADON = 1; //Activa el modulo ADC
    //_ADIE=1;
    
}
void TM3_INI(void)
{
    T3CONbits.TCKPS = 0; // Prescaler de 1
    PR3 = 2949;//Para un 100us 29,491,200/100=29,49.12, entre el prescaler 29,491.2/8=2949.2
    TMR3 = 0;//Iniciamos conteo del Timer 3 en 0
    _T3IF = 0;// LImpia la bandera
    _T3IE = 1;// Habilita la interrupci?n del TMR3
    T3CONbits.TON = 1; //Habilita el TMR3
}

void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<2000;j++);
}

int lectura_ADC(int pin){
    int adc=0;
    ADCHSbits.CH0SA = pin;
    while(!_DONE);
           _DONE = 0;
    adc=ADCBUF0;
    
    return adc;
}
