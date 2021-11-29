/*
 * File:   mainAnd.c
 * Author: Ovando
 *
 * Created on September 21, 2020, 12:20 PM
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

/*
    Y lógico && AND
    O lógico || OR
    Negación lógica  ! NOT
 */

//int main(void) {
//    
//    ADPCFG=0xFFFF;
//    
////   _TRISB1=0;
//    TRISB=0;
//    _TRISF2=1;
//    _TRISF1=1;
//    _LATB1=0;
//    while(1)
//    {
//        if(_RF1||_RF2)
//        {
//            _LATB1=1;
//        }
//        else
//        {
//            _LATB1=0;
//        }   
//        
////        int i;
////        for (i =0; i<=1023; i+=4)
////        {
////        LATB = i;
//////        retardo (1000);
////        }
//    }
//    
//    return 0;
//}

void retardo(int t){
    int i,j;
    for(i=0;i<t;i++) //random delay
    for(j=0;j<2000;j++);
}
int main(void) {
    
    ADPCFG=0xFFFF;
    TRISB=0;

    while(1)
    {  
        
        int i;
        for (i =1023; i>0; i--)
        {
        LATB = i;
        retardo (1000);
        }
    }
    
    return 0;
}




//        _LATB1=_RF1&&_RF2;