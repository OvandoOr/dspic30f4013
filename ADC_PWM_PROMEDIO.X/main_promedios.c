#include "xc.h"
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

#define valores_promedio 5

void ADC_INI(void);
void PWM_INI(void);
void retardo (int t);
int cont_ADC=0,toma_ADC[valores_promedio],acomodo,ini_media=0,i;

int promedio_ADC=0;

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    if(valores_promedio>cont_ADC) 
    {
        cont_ADC=0;
        ini_media=1;
    }
    while(!_DONE);
       _DONE = 0;
    toma_ADC[cont_ADC]=ADCBUF0;

    if(ini_media==1)
    {
        promedio_ADC=0;
        for(i=0;i<valores_promedio;i++)
            promedio_ADC+=toma_ADC[i];
        promedio_ADC/=valores_promedio;
    }
           
    cont_ADC++;
    OC3RS = promedio_ADC*.57;//Transformamos resultado del ADC a PWM con 2359/4096=.72
    _T3IF = 0;
}

int main(void) {
    
    
    ADC_INI();
    PWM_INI();
    TRISC=0xFFFF;
    TRISD=0x0000;//F porque equivale a 1111 en el ultimo byte
    PORTD=0;//definir lectura inicial del dip-switch como 0

    while(1)
    {
        retardo(50);
    }
    
    return 0;
}

void ADC_INI(void)
{
    TRISB = 0x00FF; // Configura todos como salidas excepto PORTB5
    ADCON2bits.SMPI=1;
    ADPCFGbits.PCFG5 = 0; //Solo el bit 5 como entrada analogica
    /*** Configuracion del ADC ***/
    ADCON2bits.VCFG = 0; //Selección de niveles de referencia externos
    ADCHSbits.CH0SA = 5; //Selección de la entrada a muestrear
    //FCY=29,491,200 MIPS, TCY=33.90842 ns, Para 1 muestra cada 20us a 20 TADS, El TAD=1000ns, ADCS=57.997
    ADCON3bits.ADCS = 58; //ADCS = 58, es decir Tad real = 1000.05ns
    // Muestreo automatico (al terminar la conversion comienza a muestrear)
    ADCON1bits.ASAM = 1; // Indica disparo de muestreo automatico
    // Conversion automatica por contador interno
    // (empieza a convertir SAMC TADs despues del inicio del muestreo)
    ADCON1bits.SSRC = 7; // SSRC=?111? conversion automatica
    ADCON3bits.SAMC = 6; // 6 TAD despues del muestreo comienza la conversion
    ADCON1bits.ADON = 1; //Activa el modulo ADC    
}
void PWM_INI(void)
{
    T3CONbits.TCKPS = 0; // Prescaler de 1
    PR3 = 2359;//Para un 100us 29,491,200/100=29,49.12, entre el prescaler 29,491.2/8=2949.2
    TMR3 = 0;//Iniciamos conteo del Timer 3 en 0
    _T3IF = 0;// LImpia la bandera
    _T3IE = 1;// Habilita la interrupción del TMR3
    T3CONbits.TON = 1; //Habilita el TMR3
    OC3R = OC3RS = 0; // Inicia en 0%, Inicia el de lectura y escritura
    // Activa el modulo PWM
    OC3CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
    
}

void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<29000;j++);
}
