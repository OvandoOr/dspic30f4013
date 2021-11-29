/*
 * File:   timer1_main.c
 * Author: Ovando
 *
 * Created on October 14, 2020, 12:44 PM
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
#include "p30F4013.h"
void retardo(int t);
void configuracion_timer1(void);
void configuracion_CN3(void);
int contador_timer=0;

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt( void)//Interrupcion timer1
{
    contador_timer++;
    if(contador_timer>=2){//cuando pasen dos interrupciones de .5 segundos entra la funcion que apaga o enciende el led
        //<< hace un recorrimiento de bits hacia la izquierda 
        //>> hace un recorrimiento de bits hacia la derecha
        //~ hace cambio de lugar de bits o negacion
        //TRISB=0x0001  
        //TRISB=TRISB<<5; TRISB=0x0002
        //TRISB=0x0001 
        //TRISB=~TRISB; TRISB=0xFFFE 
        _LATB0=~_LATB0;//Cambia el estado actual del pin RB0 por el contrario, es decir si esta apagado se enciende
        //y si esta encendido se apaga
        contador_timer=0;
    }
    _T1IF = 0;
}
//Programa que encienda y apague un led especificamente cada 1 segundo

//void __attribute__((interrupt, no_auto_psv)) _T1Interrupt( void)//Interrupcion timer1
//{
//    _LATB0=~_LATB0;//Cambia el estado actual del pin RB0 por el contrario, es decir si esta apagado se enciende
//    _T1IF = 0; //Limpia la bandera
//}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt( void)//Interrupcion cn en esta situacion solo esta habilitada en el pin CN3 o B1
{
    while(_RB1);//Pausara nuestro programa hasta que la entrada deje de ser positiva(_RB1==1)
    retardo(550);
    PR2=5757;
    _CNIF = 0; //Limpia la bandera
}


int main(void) {
    ADPCFG=0xFFFF;//Todos los pines digitales en el puerto b
    TRISB=0;//Todos los pines como salidas
    configuracion_timer1();
    configuracion_CN3();
    _LATB0=1;
    while(1)
    {
    }
    return 0;
}

void configuracion_timer1(void)
{
    // Inicializaciones
    _T1IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
    TMR1 = 0; //Limpia el Timer
    
    //Calculamos nuestro PR primero obteniendo nuestro FOSC especifico
    //FOSC=Frecuencia Oscilador x PLL
    //FOSC=7.37Mhz x 16 = 117.92M ciclos por segundo
    //Luego necesitamos calcular nuestro FCY
    //FCY=FOSC/4
    //FCY=117.92M/4 =29.48 Millones de instrucciones por segundo(MIPS)
    //Luego necesitamos calcular nuestro TCY
    //TCY=1/FCY
    //TCY=1 / 29.48M = 33.9213 ns
    //Tresolucion = TCY x Preescaler(Definido por TCKPS)
    //Tresolucion = 33.9213 ns x 256 = 8683.8528 ns
    //PR = Periodo_deseado / Tresolucion
    //PR = 1 / 8683.8528 ns = 115,156 no nos alcanza entonces lo dividiremos entre dos para que nos de intervalos de medio segundo
    //PR= 115,156/2 = 57,578 para medio segundo
    PR1 = 57578; //Ajusta el registro de periodo para 
    //medio segundo ya que el segundo completo no entra 
    //en nuestro registro para el interno
    
    //PR1 = 62500; //Ajusta el registro de periodo

    // Configura la fuente del módulo del Timer1
    T1CONbits.TCKPS = 3; //Prescaler de 256
    T1CONbits.TCS = 0; // Fuente interna TCY
    T1CONbits.TON = 1; // Habilita el timer

    // Inicializa los bits de control del Timer1
    _T1IF = 0; //Limpia la bandera
    _T1IE = 1; //Habilita la interrupción T1

}

void configuracion_CN3(void)//las interrupciones cn detectan cambios de voltaje digital en las entradas cn habilitadas todas las interrupciones cn tienen las mismas configuraciones e interrupcion de forma predeterminada
{
    _TRISB1 = 1; //bit 1 de PORTB como entrada para CN3
    _CN3IE = 1; // Habilita la interrupcion del CN3 pin RB1
    _CN3PUE = 1; // Habilita la pull-up del pin CN3
    _CNIF = 0; // Limpia la bandera de interrupcion de CN
    _CNIP = 5; // Prioridad 5 de interrupcion CN
    _CNIE = 1; // Habilita la interrupcion CN

}

void retardo(int t){
    int i,j;
        for(i=0;i<t;i++) //random delay
            for(j=0;j<29000;j++);
}