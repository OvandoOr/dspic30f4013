/*
 * File:   main_EOG.c
 * Author: MarthaAlicia
 *
 * Created on 14 de agosto de 2018, 02:20 AM
 */


#include "xc.h"
#include<p30F4013.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "C:\Users\MarthaAlicia\Desktop\Basura\Microcontroladores\libraries\uart\UART.h"

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
#define num_muestras 21


void ADC_INI(void);
void TMR3_INI(void);
void retardo (int t);
void EOG(int x, int y);
//const int num_muestras = 7;
char buf[20];
int valor_X, valor_Y, cont_ADC=0,i,j,k,fx,fy;
int estadox=0, estadoy=0, delay=1,cont_t=0;
int min=100,max=100;



void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    cont_t++;
    _T3IF = 0;
}

int main(void) {
    
    int cont_prom=0,promediox,promedioy;
    long promedio=0, promedio1=0;
    InitUART2();
    
    _TRISF2=1;
    TRISD=0x0000;
    _RF2=0;
    
    ADC_INI();
    TMR3_INI();
    LATD=0x000F;
    while(cont_prom<200){
        while(!_DONE);
           _DONE = 0;
        valor_X=ADCBUF0;
        valor_Y=ADCBUF1;
        promedio+=valor_X;
        promedio1+=valor_Y;
        cont_prom++;
        retardo(2);
    }
    promediox=promedio/=cont_prom;
    promedioy=promedio1/=cont_prom;
    LATD=0x0000;
    while(1)
    {
        valor_X=0;
        valor_Y=0;
        //_ADIE=0;
        for(i=0;i<20;i++){
            while(!_DONE);
               _DONE = 0;
            valor_X+=ADCBUF0;
            valor_Y+=ADCBUF1;
            
        }
        fx=valor_X/20;
        fy=valor_Y/20;
        
        if(fx>promediox+max){
            WriteStringUART2("a\n");
            //retardo(100);
        }
        
        if(fx<promediox-min){
            WriteStringUART2("b\n");
            //retardo(100);
        }
        /*
        if(fy>promedioy*max){
            WriteStringUART2("c\n");
            retardo(100);
        }
        
        if(fy<promedioy*min){
            WriteStringUART2("d\n");
            retardo(100);
        }*/
        /*switch(estadox)
        {
            case 0:
                if(valor_X>promediox*max)
                    estadox=1;
                if(valor_X<promediox*min)
                    estadox=-1;
                break;
            case 1:
               // if(valor_X<promediox*min){
                    estadox=0;
                    retardo(delay);
                //}
                break;
            case -1:
                //if(valor_X>promediox*max){
                    estadox=0;
                    retardo(delay);
                //}
                break;
                
        }
        switch(estadoy)
        {
            case 0:
                if(valor_Y>promedioy*max)
                    estadoy=1;
                if(valor_Y<promedioy*min)
                    estadoy=-1;
                break;
            case 1:
                //if(valor_Y<promedioy*min){
                    estadoy=0;
                    retardo(delay);
               // }
                break;
            case -1:
                //if(valor_Y>promedioy*max){
                    estadoy=0;
                    retardo(delay);
                //}
                break;
                
        }*/
        
        //EOG(estadox, estadoy);
        //_ADIE=1;
        //sprintf(buf,"%d,%d,%d,%d\n",fx,promediox+max,promediox-min,promediox);
        //sprintf(buf,"%d,%f,%f,%d\n",fy,promedioy*max,promedioy*min,promedioy);
        //WriteStringUART2(buf);
        //sprintf(buf,"PWM=%d",OC3RS);
        //WriteStringUART2(buf);
        //retardo(1);
        
        if(!_RF2){
        WriteStringUART2("e\n");
        //retardo(40);
        }
        if(cont_t>5000){
            LATD=0x000F;
            promedio=0;
            promedio1=0;
            cont_prom=0;
            while(cont_prom<200){
                while(!_DONE);
                   _DONE = 0;
                valor_X=ADCBUF0;
                valor_Y=ADCBUF1;
                promedio+=valor_X;
                promedio1+=valor_Y;
                cont_prom++;
                retardo(2);
            }
            promediox=promedio/=cont_prom;
            promedioy=promedio1/=cont_prom;
            cont_t=0;
            LATD=0x0000;
        }
    }
    
    return 0;
}

void ADC_INI(void)
{
    TRISB = 0x0030; // Configura todos como salidas excepto PORTB5
    ADCON2bits.SMPI=2;//muestras por interrupt
   // _ADIP=6;
    //_ADIF=0;
    ADPCFGbits.PCFG4 = 0;
    ADPCFGbits.PCFG5 = 0; //Solo el bit 4 y 5 como entrada analogica
    ADCON2bits.CSCNA=1;
    /*** Configuracion del ADC ***/
    ADCON2bits.VCFG = 0; //Selección de niveles de referencia externos
    //FCY=29,491,200 MIPS, TCY=33.90842 ns, Para 1 muestra cada 20us a 20 TADS, El TAD=1000ns, ADCS=57.997
    ADCON3bits.ADCS = 58; //ADCS = 58, es decir Tad real = 1000.05ns
    ADCSSL=0x0030;
    // Muestreo automatico (al terminar la conversion comienza a muestrear)
    ADCON1bits.ASAM = 1; // Indica disparo de muestreo automatico
    // Conversion automatica por contador interno
    // (empieza a convertir SAMC TADs despues del inicio del muestreo)
    ADCON1bits.SSRC = 7; // SSRC=?111? conversion automatica
    ADCON3bits.SAMC = 6; // 6 TAD despues del muestreo comienza la conversion
    
    ADCON1bits.ADON = 1; //Activa el modulo ADC
    //_ADIE=1;
    
}

void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<29000;j++);
}

void EOG(int x, int y){
    
        if(x==1)
            WriteStringUART2("a\n");

        else if(x==-1)
            WriteStringUART2("b\n");
    
    
        if(y==1)
            WriteStringUART2("c\n");

        else if(y==-1)
            WriteStringUART2("d\n");
    if(!_RF2){
        WriteStringUART2("e\n");
        //retardo(40);
    }
    
    /*
    if(x==1 && y==1)
        WriteStringUART2("e\n");
    
    else if(x==-1 && y==1)
        WriteStringUART2("f\n");
    
    else if(x==1 && y==-1)
        WriteStringUART2("g\n");
    
    else if(x==-1 && y==-1)
        WriteStringUART2("h\n");
    else
        WriteStringUART2("n\n");*/
        
}

void TMR3_INI(void)
{
    T3CONbits.TCKPS = 2; // Prescaler de 1
    PR3 = 2950;//Para un 100us 29,491,200/100=29,49.12, entre el prescaler 29,491.2/8=2949.2
    TMR3 = 0;//Iniciamos conteo del Timer 3 en 0
    _T3IF = 0;// LImpia la bandera
    _T3IE = 1;// Habilita la interrupción del TMR3
    T3CONbits.TON = 1; //Habilita el TMR3
    
}
