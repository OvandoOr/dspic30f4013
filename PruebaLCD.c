#include "p30fxxxx.h"
#include <stdio.h>
#include "librerias/LCD/LCD.h"
//#include "LCD.h"
//#include "../../librerias/delay/delay.h"
_FOSC(CSW_FSCM_OFF & XT_PLL16);
_FWDT(WDT_OFF); 
_FBORPOR(MCLR_EN & PWRT_OFF); 
_FGS(CODE_PROT_OFF);

#define BOTON _RD8 // En este pin se conecta el boton
int contador; // Variable contador
char cad_contador[6]; // Cadena con contador para enviar a la pantalla LCD
int main()
{
contador = 0; // Inicia contador
_TRISD8 = 1; // Configura como entrada el bit RRD8/INT1 (BOTON)
// Inicializa la pantalla en modo 4 bits de 2 renglones
iniciaLCD(FOUR_BIT & TWO_LINE );
while(busyLCD()); // Espera a que la pantalla LCD se desocupe
putsLCD(" CONTADOR "); // Escribe "CONTADOR" en el primer renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("0"); // Escribe "0" la pantalla (renglon 2)
/* A continuación los pasos para configurar y programar la interrupción */
// Paso 1. Configura el periferico
_INT1EP = 1; // Disparo de interrupcion por flanco descendente
// Paso 2. Ajusta la prioridad de la fuente de interrupcion
_INT1IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
// Paso 3. Limpia la bandera de interrupcion
_INT1IF = 0;
// Paso 4. Habilita la interrupcion externa 1
_INT1IE = 1;

/* Ciclo principal */
while(1)
{
/* En este ejemplo, el programa solo espera a que se pulse el boton para
incrementar y mostrar el contador
*/
} // Ciclo principal
return 0;
} //main
/* RUTINA DE SERVICIO DE INTERRUPCION */
// Esta rutina se ejecuta cada vez que se produce un flanco descendente en el BOTON
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt( void)
{
//retardo(1000); // Espera a que se estabilice el nivel de voltaje
if(BOTON==0) // Si no es un rebote...
{
contador++; // Incrementa contador
sprintf(cad_contador, "%d", contador); // Convierte el valor en contador a
//cadena en cad_contador
setDDRamAddr(0x40); // Se mueve al renglon 2
putsLCD(cad_contador); // Escribe el valor del contador
}
_INT1IF = 0; //Limpia la bandera de interrupción antes de salir
} //INT1Interrupt


//Funcion retardo
void retardo(int t)
{
int i=1,j;
for(j=0; j<t; j++) //Repite el ciclo t veces
while(i) //Cicla hasta que i se desborde
i++;
}