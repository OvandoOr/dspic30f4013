/*
 * File:   main_io.c
 * Author: Ovando
 *
 * Created on September 14, 2020, 12:52 PM
 */

// FOSC
#pragma config FOSFPR = FRC_PLL16       // Oscillator (FRC w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include "xc.h"
#include <p30f4013.h>

void retardo(int retardo_us);

int main(void) {
    
    ADPCFG=0xFFFF; //digital todos los pines del puerto b
    //digital=1, analogico=0
    //solo el puerto b requiere de esta configuracion
    //ya que es el unico puerto que puede funcionar como 
    //analogico, los demas puertos son solo digitales
    TRISB=0;//0x0000 salidas todos los pines del puerto b
    //0 en determinado pin representa que se usara como salida
    TRISF=0xFFFF;//declaramos como entradas todos los pines del puerto f
    //1 en determinado pin representa que se usara como entrada
    LATB=0;//Definimos todas las salidas del puerto b como
    //apagados=0
    
    
    while(1)
    {
        // se lee el valor del puerto f, si la suma de los
        //pines encendidos(en bits) supera el valor de 15 se 
        //encienden los leds. En este caso tiene que superar 
        //el valor 0
        if(PORTF==0x0001)//RF0==1 todos los demas pines 0
        {
            LATB=0x1111;//encendemos pin 0,4,8,12 del puerto b
            retardo(1000);
            LATB=0;//apagamos todos los pines del puerto b
            retardo(1000);
        }
        if(PORTF==0x0002)//RF1==1 todos los demas pines 0
        {
            LATB=0x0001;//encendemos pin 0 del puerto b
            retardo(1000);
            LATB=0x0011;//encendemos pin 0,4 del puerto b
            retardo(1000);
            LATB=0x0111;//encendemos pin 0,4,8 del puerto b
            retardo(1000);
            LATB=0x1111;//encendemos pin 0,4,8,12 del puerto b
            retardo(1000);
            LATB=0;//apagamos todos los pines del puerto b
        }
        if(PORTF==0x0003)//RF0==1 y RF1==1 todos los demas pines 0
        {
            LATB=0x1555;//encendemos pin 0,4,8,12 del puerto b
            retardo(1000);
            LATB=0;//apagamos todos los pines del puerto b
            retardo(1000);
        }
        else
        {
            LATB=0;//apagamos todos los pines del puerto b
        }

    }
    return 0;
}

void retardo(int retardo_us)//funcion para hacer tiempo
{
    int i,j;
    for(i=0;i<retardo_us;i++)
        for(j=0;j<29000;j++);
}