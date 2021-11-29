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

#include "C:\Users\MarthaAlicia\Desktop\basura\Microcontroladores\libraries\uart\UART.h"

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



void ADC_INI(void);
void PWM_INI(void);
void retardo (int t);
char buf[20];
int promedio_ADC;

void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt(void)
{
    while(!_DONE);
           _DONE = 0;
    int adc=ADCBUF0;
    LATD=1;
    LATB= adc & 0x0FFF;
    LATD=0;
    LATB=0;
    _ADIF=0;
}

//void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
//{
//    while(!_DONE);
//           _DONE = 0;
//    int adc=ADCBUF0;
//    LATD=1;
//    LATB= adc & 0x0FFF;
//    LATD=0;
//    _T3IF = 0;
//}

int main(void) {
    //PWM_INI();
    //InitUART2();
    ADC_INI();
    TRISD=0x0000;
    LATD=0x0000;
    while(1)
    {
//        LATD=0x000F;
//        retardo(1000);
//        LATD=0x0000;
//        retardo(1000);
    }
    
    return 0;
}

void ADC_INI(void)
{
    TRISB = 0x1000; // Configura todos como salidas excepto PORTB12
    ADCON2bits.SMPI=1;
    _ADIP=6;
    _ADIF=0;
    ADPCFGbits.PCFG12 = 0; //Solo el bit 12 como entrada analogica
    /*** Configuracion del ADC ***/
    ADCON2bits.VCFG = 0; //Selección de niveles de referencia externos
    ADCHSbits.CH0SA = 12; //Selección de la entrada a muestrear
    //FCY=29,491,200 MIPS, TCY=33.90842 ns, Para tener 98800 muestras/s cada a 15 TADS, El TAD=668ns, ADCS=38.4
    ADCON3bits.ADCS = 38; //ADCS = 2(TAD/TCY)-1 ===> 38
    // Muestreo automatico (al terminar la conversion comienza a muestrear)
    ADCON1bits.ASAM = 1; // Indica disparo de muestreo automatico
    // Conversion automatica por contador interno
    // (empieza a convertir SAMC TADs despues del inicio del muestreo)
    ADCON1bits.SSRC = 7; // SSRC=?111? conversion automatica
    ADCON3bits.SAMC = 1; // 1 TAD despues del muestreo comienza la conversion
    ADCON1bits.ADON = 1; //Activa el modulo ADC
    _ADIE=1;
    
}
//void PWM_INI(void)
//{
//    T3CONbits.TCKPS = 1; // Prescaler de 8-- 29,491,200/8 = 3,686,400
//    PR3 = 36860;//Para un 10ms 3,686,400/100=36,860
//    TMR3 = 0;//Iniciamos conteo del Timer 3 en 0
//    _T3IF = 0;// LImpia la bandera
//    _T3IE = 1;// Habilita la interrupción del TMR3
//    T3CONbits.TON = 1; //Habilita el TMR3
//    //OC3R = OC3RS = 0; // Inicia en 0%, Inicia el de lectura y escritura
//    // Activa el modulo PWM
//    //OC3CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
//    
//}

void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<29000;j++);
}
