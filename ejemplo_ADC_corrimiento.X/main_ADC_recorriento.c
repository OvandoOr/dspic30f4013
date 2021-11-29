/*
 * File:   main_ADC_recorriento.c
 * Author: Ovando
 *
 * Created on March 19, 2021, 1:33 PM
 */


#include "xc.h"

void config_ADC_manual_automatico(int pin);
void retardo(int t);
int lectura_ADC(int pin);

int valores_ADC[10],indice=0;
int promedio=0;


void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)///interrupcion a 1ms
{
    int val_adc=lectura_ADC(6);
    int i;
    
    valores_ADC[indice]=val_adc;
    
    indice++;
    
    if(indice>9) indice=0;
    
    for(i=0;i<9;i++)
    {
       promedio += valores_ADC[i];
    }
    promedio = promedio/10;
    
    //sprintf(buf,%d,promedio);
    //putsU1(buf);
    
    _T3IF=0;
}

int main(void) {
    config_ADC_manual_automatico(6);
    _TRISB0=0;//para configurar un led en el puerto b sin modificar la funcion del 
    _PCFG0=1;//adc debemos modificar solamente por bits de preferencia
    
    while(1)
    {
        _LATB0=~_LATB0;
        retardo(100);
    }
    
    return 0;
}

void config_ADC_manual_automatico(int pin){
    TRISB=0xFFFF;// todos los pines como entradas
    //ADPCFG=0xFFEF; //valor 1111 1111 1110 1111
    ADPCFG=0;//todos los pines como analogico
    _VCFG=0;//referencias de voltaje internas
    ADCHSbits.CH0SA=pin;//seleccionamos el pin que se muestreara
    //Si queremos 100k, tenemos que dividir esta frecuencia entre los tads por conversion que en este caso son 15
    //1/90k=.011111ms = 11.1111us/15 = .74074us
    ////FCY=29,491,200 MIPS, TCY=33.90842 ns
    ADCON3bits.ADCS=44;//ADCS=((2*740.74ns)/33.9043ns)-1=(44.8933-1)=44
    ADCON1bits.ASAM=0;//configuramos disparo o toma muestra manual del micro
    ADCON1bits.SSRC=7;//indicamos que la conversion sera de forma automatica
    ADCON3bits.SAMC=1;//Cantidad de tads de espera despues de la toma de una muestra
    
    ADCON1bits.ADON=1;//Iniciamos ADC
}

void TM3_INI(void)
{
    T3CONbits.TCKPS = 0; // Prescaler de 1
    PR3 = 29491;//Para un 1ms 29,491,200/1000=29491.2
    TMR3 = 0;//Iniciamos conteo del Timer 3 en 0
    _T3IF = 0;// LImpia la bandera
    _T3IE = 1;// Habilita la interrupción del TMR3
    T3CONbits.TON = 1; //Habilita el TMR3
}

void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<2000;j++);
}

int lectura_ADC(int pin){
    int adc=0;
    ADCHSbits.CH0SA = pin;
    _SAMP=1;
    while(!_DONE);
           _DONE = 0;
    adc=ADCBUF0;
    
    return adc;
}