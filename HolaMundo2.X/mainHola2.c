/*
 * File:   mainHola2.c
 * Author: Ovando
 *
 * Created on September 14, 2021, 11:32 AM
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

#include "xc.h"

void retardo(int tiempo);

int main(void) {
    ADPCFG=0x001F;//0000 0000 0001 1111
    //ADPCFG analogicos=0 digitales=1 solo puerto B
    
    //A=10 B=11 C=12 D=13 E=14 F=15
    TRISB=0xFFE0;//1111 1111 1110 0000
    //TRIS ENTRADAS=1 SALIDAS=0
    
    //TRISF=0x0001;
    _TRISF0=1;
            
    while(1)
    {
       if(PORTF==0x0001)//_RF0==1
       {
           LATB=0x0003; //0000 0000 0000 0011
           //retardo(200); // solo en fisico requiere retardo
           LATB=0x0018; //0000 0000 0001 1000
           //retardo(200); // solo en fisico requiere retardo
           LATB=0x0004; //0000 0000 0000 0100
           //retardo(200); // solo en fisico requiere retardo
           LATB=0x001F; //0000 0000 0001 1111
           //retardo(200); // solo en fisico requiere retardo
       }
       else
           LATB=0;//0000 0000 0000 0000
    }
    return 0;
}

void retardo(int tiempo)//delay(200)
{
    int i,j;
    for(i=0;i<tiempo;i++)
        for(j=0;j<65000;j++);
}
