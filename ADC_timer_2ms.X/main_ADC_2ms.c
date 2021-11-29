/*
 * File:   main_ADC_2ms.c
 * Author: Ovando
 *
 * Created on January 22, 2021, 2:28 PM
 */

// FOSC
#pragma config FOSFPR = FRC_PLL16       
// Oscillator (FRC w/PLL 16x) FCY = 7.37Mhz x 16= 117.92M ciclos por segundo
//#pragma config FOSFPR = XT_PLL16       
// Oscillator (FRC w/PLL 16x) FCY = 4Mhz x 16 =64M ciclos por segundo
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

void ADC_config();
void timer3();

int main(void) {
    ADC_config();
    
    
    while(1){
        
    }
    
    
    return 0;
}


void ADC_config(){//configuracion de adc manual-automatico
    TRISB=0xFFFF;
    ADPCFG=0;
    _CH0SA=6;
    _VCFG=0;
    _ASAM=0;
    _SSRC=0b111;//7
    _SAMC=26;//valor maximo 31 tads extras es decir valor maximo de conversion 
    //se le agrega este valor a los 14 tads que toma la conversion
    
    //Queremos que nuestra tenga un muestreo de 25k por segundo
    //1/25k = 40 us/40tads= 1us=1000ns
    ////FCY=29,491,200 MIPS, TCY=33.90842 ns
    //ADCS=(2*Tad/Tcy)-1 = (2*1000ns/33.908ns)-1 = 57.98
    _ADCS= 58;
    
    _ADON=1;
}

void timer3(){
    
}