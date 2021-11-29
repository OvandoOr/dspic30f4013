#pragma config FOSFPR = FRC_PLL4       //oscilador interno con pll de 4
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF           // Watchdog Timer (Disabled)

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


#define    dac            0x60

#include "xc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int dato=0;
void retardo(int t);
int ini_dac(char dispositivo);
int enviar_sig(char direccion,char valor);
void inicio_i2c(void);
int setBaudRate();

int main(void) {
    ini_dac(dac);
    retardo(1000);
    
    while(1)
    {
        retardo(1);
        dato++;//aumenta de 1 en 1
        if(dato==255)dato=0;// cuando la señal llega a su valor maximo se reinicia a 0
        enviar_sig(0x40, dato); //escribe dentro del dispositivo en el registro 0x40 que corresponde al valor del dac
    }
    
    
    return 0;
}




void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<2000;j++);
}

int ini_dac(char dispositivo)//las direcciones i2c normalmente son de 7 bits
{
    inicio_i2c();
    char escribir_dispositivo = dispositivo << 1;//deja a ultimo bit en 0 que indica que sera una escritura
	I2CCONbits.SEN = 1;	//activas transferencias		
	while(I2CCONbits.SEN == 1);//si hay transferia activa, espera
	I2CTRN = escribir_dispositivo;		//escribe datos a enviar	
	while(I2CSTATbits.TRSTAT);		//proceso de transferencia	
	if (I2CSTATbits.ACKSTAT)	//checa si todo salio bien			
	{								
		I2CCONbits.PEN = 1; //si huvo error comprueba que si huvo un error y retorna -1
		while(I2CCONbits.PEN);			
		return -1;					
	}
    return 0; //si todo salio bien retorna 0
}

int enviar_sig(char direccion,char valor)
{
    I2CTRN = direccion; //escribe direccion que se modificara
	while(I2CSTATbits.TRSTAT); //proceso de transferencia	
	if (I2CSTATbits.ACKSTAT)//checa si todo salio bien	
	{
		I2CCONbits.PEN = 1; //si huvo error comprueba que si huvo un error y retorna -1
		while(I2CCONbits.PEN);
		return -1;
	}  
	I2CTRN = valor; //escribe valor que modificara la señal parte 1
	while(I2CSTATbits.TRSTAT);
	if (I2CSTATbits.ACKSTAT)//checa si todo salio bien	
	{
		I2CCONbits.PEN = 1; //si huvo error comprueba que si huvo un error y retorna -1
		while(I2CCONbits.PEN);
		return -1;
	}
    I2CTRN = valor; //escribe valor que modificara la señal parte 2
	while(I2CSTATbits.TRSTAT);//proceso de transferencia
	if (I2CSTATbits.ACKSTAT)//checa si todo salio bien	
	{
		I2CCONbits.PEN = 1; //si huvo error comprueba que si huvo un error y retorna -1
		while(I2CCONbits.PEN);
		return -1;
	}
    return 0;
}

void inicio_i2c(void)
{
	I2CCONbits.DISSLW 	= 1;
    I2CBRG              = setBaudRate();    //100k velocidad standard i2c
    I2CCONbits.I2CEN 	= 1;
}

int setBaudRate(){
    return (7372800/100000 - 7372800/1111111) - 1; //(ciclo trabajo/velocidad i2c - ciclo trabajo/1111111) - 1;
}
