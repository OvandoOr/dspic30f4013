/**********************************************************************************************
 www.Electronics-Base.Com
 ************************************************************************************************/

#include <p30fxxxx.h>
#include <stdlib.h>
#include <stdio.h>
#include "adc.h"
#include "timer.h"
#include <math.h>
#include "uart.h"
#include "gyro.h"

_FOSC(CSW_FSCM_OFF & FRC_PLL4); //instruction takt je isti kao i interni oscilator
_FWDT(WDT_OFF);
//-----------------------------------------------------------------
#define GyroSteadyValue_MAX 502
#define GyroSteadyValue_MIN 509
#define GyroSteadyValue_AVARAGE 506
#define GyroQuantsPerDegree 103
#define GyroQuantsPerCircle (GyroQuantsPerDegree*360)
#define GytoSpeed_INTERVAL 10

unsigned char gyro_speed_time;
signed int    gyro_adc, gyro_speed;
long          angle, circle, degrees;
float         integraled_gyro, pervious_integraled_gyro, integraled_gyro_copy;
char buf[10];

int tempRX2; //UART2
unsigned int GYRO_timer, stoperica;
//-----------------------------------------------------------------
void Delay_ms(unsigned int vreme)
//-----------------------------------------------------------------
// funkcija za kasnjenje u ms
{
    stoperica = 0;
    while(stoperica < vreme);
}

//-----------------------------------------------------------------

void __attribute__((__interrupt__)) _T1Interrupt(void) // TIMER INTERRUPT
//-----------------------------------------------------------------
// prekidna rutina Thajmera 1
{

    TMR1 = 0;               //Stop Timer
    stoperica++;            //for Delay_ms() function
    GYRO_timer++;           //get output from gyro every 10ms
    if(GYRO_timer == 10)
    {
        GYRO_timer = 0;
        gyro_adc = SPI_Ziroskop_R();
        if((gyro_adc<GyroSteadyValue_MIN)||(gyro_adc>GyroSteadyValue_MAX))
        {
            integraled_gyro=integraled_gyro+gyro_adc-GyroSteadyValue_AVARAGE;
        }
        gyro_speed_time++;
        if(gyro_speed_time==GytoSpeed_INTERVAL)
        {
            gyro_speed_time=0;
            gyro_speed=((100/GytoSpeed_INTERVAL)*(pervious_integraled_gyro-integraled_gyro))/GyroQuantsPerDegree;
            pervious_integraled_gyro=integraled_gyro;
        }
    }

    IFS0bits.T1IF = 0;      //Clear timer flag
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
void __attribute__((__interrupt__)) _U2RXInterrupt(void) //UART2 UINTERRUPT
//-----------------------------------------------------------------
{
    IFS1bits.U2RXIF = 0;
    tempRX2 = U2RXREG;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
int main(void)  //MAIN MAIN MAIN
//-----------------------------------------------------------------
{
    InitUART2();
    Init_T1();
    InitZiroskop();
    GYRO_timer = 0;
    stoperica = 0;


   
    WriteStringUART2("**********START**********\r");
    WriteStringUART2("Gyroscope speed and position measurement example from www.Electronics-Base.com\r");
    Delay_ms(1000);
   
    while (1)
    {
          Delay_ms(150);
          integraled_gyro_copy=integraled_gyro;
          WriteStringUART2("Gyroscope ADC readout: ");
          WriteUART2dec2string(gyro_adc);
          WriteStringUART2(" Gyroscope summed integral: ");
          sprintf(buf, "%d", integraled_gyro_copy);
          WriteStringUART2(buf);
          WriteStringUART2(" Calculated Angle: ");
          angle=integraled_gyro/GyroQuantsPerDegree;
          sprintf(buf, "%ld", angle);
          WriteStringUART2(buf);
          WriteStringUART2("°");
          WriteStringUART2(" Full circles: ");
          circle=integraled_gyro_copy/GyroQuantsPerCircle;
          sprintf(buf, "%ld", circle);
          WriteStringUART2(buf);
          WriteStringUART2(" Degrees: ");
          degrees =  integraled_gyro_copy-circle*GyroQuantsPerCircle;
          degrees=degrees/GyroQuantsPerDegree;
          sprintf(buf, "%ld", degrees);
          WriteStringUART2(buf);
          WriteStringUART2(" Speed: ");
          sprintf(buf, "%d", gyro_speed);
          WriteStringUART2( buf);
          WriteStringUART2(" °/sec\r");
    }//while
    return 0;
}//main
