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

#include "C:\Users\Inv\Desktop\Brandon Ovando\Microcontroladores\libraries\uart\UART.h"

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
int media_ADC, cont_ADC=0, resul_ADC[7],toma_ADC[7],acomodo,ini_media=0,i,j,k,val_ant,valor_D=0;

void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt(void)
{
    while(!_DONE);
           _DONE = 0;
           toma_ADC[cont_ADC]=ADCBUF0;
           if(cont_ADC>6) 
           {
               cont_ADC=0;
               ini_media=1;
           }
           if(ini_media==1)
           {
               for(i=0;i<7;i++)
                   resul_ADC[i]=toma_ADC[i];
               
               for(i=0;i<7;i++)
               {
                  for(j=0;j<7;j++)
                  {
                      if(resul_ADC[j]>resul_ADC[i])
                      {
                          acomodo=resul_ADC[j];
                          resul_ADC[j]=resul_ADC[i];
                          resul_ADC[i]=acomodo;
                      }
                  }
               }
               media_ADC=resul_ADC[3];
           }
        if(!_RF2)
        {
            retardo(5);
            if(!_RF2)
            {
                //_ADIE=0;
                sprintf(buf,"Media=%d, ",media_ADC);
                WriteStringUART2(buf);
                for(k=0;k<7;k++)
                {
                          sprintf(buf,"%d,",resul_ADC[k]);
                          WriteStringUART2(buf);
                }
                WriteStringUART2("\n");
                //_ADIE=1;
                //sprintf(buf,"PWM=%d",OC3RS);
                //WriteStringUART2(buf);
            }
        }
           
           cont_ADC++;
    _ADIF=0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    OC3RS = media_ADC*.7;//Transformamos resultado del ADC a PWM con 2949/4096=.72
    _T3IF = 0;
}

int main(void) {
    
    
    ADC_INI();
    PWM_INI();
    InitUART2();
    _TRISF2=1;
    _RF2=0;
    TRISC=0xFFFF;
    TRISD=0x0000;//F porque equivale a 1111 en el ultimo byte
    PORTD=0;//definir lectura inicial del dip-switch como 0

    while(1)
    {
        
        
//        valor_D= PORTC;
//        if(valor_D != val_ant)
//        {
//            switch(valor_D)
//            {
//                case 0x2000:
//                    ADCHSbits.CH0SA = 5;
//                    break;
//                case 0x4000:
//                    ADCHSbits.CH0SA = 4;
//                    break;
//                case 0x8000:
//                    ADCHSbits.CH0SA = 3;
//                    break;
//                default:
//                    
//                    break; 
//            }
//            val_ant=valor_D;
//        }
        retardo(50);
    }
    
    return 0;
}

void ADC_INI(void)
{
    TRISB = 0x00FF; // Configura todos como salidas excepto PORTB5
    ADCON2bits.SMPI=1;
    _ADIP=6;
    _ADIF=0;
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
    _ADIE=1;
    
}
void PWM_INI(void)
{
    T3CONbits.TCKPS = 0; // Prescaler de 1
    PR3 = 2949;//Para un 100us 29,491,200/100=29,49.12, entre el prescaler 29,491.2/8=2949.2
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
