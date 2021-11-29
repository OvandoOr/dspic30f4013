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

int main(void) {
    
    ADPCFG=0xFFFF; //todo el puerto b como digital.
    //digital=1, analogico=0
    //solo el puerto b requiere de esta configuracion
    //ya que es el unico puerto que puede funcionar como 
    //analogico, los demas puertos son solo digitales
    //ADPCFG = 65535   
    //ADPCFG = 0b1111 1111 1111 1111
    TRISB=0x0000;// TRISB=0; todo el puerto b como salida, 
    //0 en determinado pin representa que se usara como salida
    TRISF=0xFFFF;//todo el puerto F como entrada, 
    //1 en determinado pin representa que se usara como entrada
    LATB=0x0000;//Definimos todas las salidas del puerto b como
    //apagados=0
    
    
    while(1)
    {
        // se lee el valor del puerto f, si la suma de los
        //pines encendidos(en bits) supera el valor de 15 se 
        //encienden los leds. En este caso tiene que superar 
        //el valor 0x0010
        if(PORTF>0x0010)
        {
            LATB=LATB+2
            //retardo();
            
        }
        else
        {
            LATB=0x0000;//apagamos todos los pines del puerto b
        }
        
    
    }
    
    
    
    return 0;
}
