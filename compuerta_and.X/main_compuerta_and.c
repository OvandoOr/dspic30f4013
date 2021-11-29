/*
 * File:   main_compuerta_and.c
 * Author: Ovando
 *
 * Created on September 21, 2020, 7:00 PM
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
    
    ADPCFG=0xFFFF;//todo los pines puerto b digitales
    _TRISB1=0;//salida de compuerta, representado por un led
    
    _TRISF1=1;//entrada de compuerta,representada por un switch
    _TRISF2=1;//entrada de compuerta,representada por un switch
    
    _LATB1=0;//apagamos nuestro led
    
    while(1)
    {
        //AND &&
        //OR  ||
        //NOT !
        if(_RF1==1 && _RF2==1)//(_RF1 && _RF2)
        {
            _LATB1=1;//encendemos nuestro led 
        }
        else
        {
           _LATB1=0;//apagamos nuestro led 
        }
        
    }
    return 0;
}
