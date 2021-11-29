#include <spi.h>
#include "my_SPI.h"

void InitSPI (void)
{
    unsigned int SPICONValue;
    unsigned int SPISTATValue;


    TRISFbits.TRISF6 = 0;        //Output RF6/SCK1
    TRISFbits.TRISF2 = 1;        //Input  RF2/SDI1
    TRISFbits.TRISF3 = 0;        //Output RF3/SDO1
    ADPCFGbits.PCFG2=1;          //Pin RB2 analgoni
    TRISBbits.TRISB2 = 0;        //Output RB2 (SDC_CS RB2)
    LATBbits.LATB2 = 1;

    SPICONValue =           FRAME_ENABLE_OFF &     //FRMEN:  0 = Framed SPI support disabled
                            FRAME_SYNC_OUTPUT &    //SPIFSD: 0 = Frame sync pulse output (master)
                            ENABLE_SDO_PIN &       //DISSDO: 0 = SDOx pin is controlled by the module
                            SPI_MODE16_OFF &       //MODE16: 0 = Communication is byte-wide (8 bits)
                            SPI_SMP_OFF &          //SMP:    0 = Input data sampled at middle of data output time
                            SPI_CKE_OFF &          //CKE:    0 = Serial output data changes on transition from Idle clk state to active clk state
                            SLAVE_ENABLE_OFF  &    //SSEN:   0 = SS pin not used by module. Pin controlled by port function
                            CLK_POL_ACTIVE_HIGH &  //CKP:    0 = SS pin not used by module. Pin controlled by port function
                            MASTER_ENABLE_ON &     //MSTEN:  1 = Master mode
                            SEC_PRESCAL_3_1 &      //SPRE<2:0>: Secondary Prescale 3:1
                            PRI_PRESCAL_1_1;       //PPRE<1:0> Primary Prescale 1:1

    SPISTATValue =          SPI_ENABLE &           //SPIEN:   1 = Enables module and configures SCKx, SDOx, SDIx and SSx as serial port pins
                            SPI_IDLE_CON &         //SPISIDL: 0 = Continue module operation in Idle mode
                            SPI_RX_OVFLOW_CLR;     //SPIROV:  0 = No overflow has occurred. Clear receive overflow bit.

    OpenSPI1(SPICONValue, SPISTATValue);
}

void SPIWrite(unsigned int data)
{
    while(SPI1STATbits.SPITBF);
    WriteSPI1(data);
}

unsigned int SPIRead()
{
    unsigned int tmp = 0;
    WriteSPI1(0);
    while(!DataRdySPI1());
    tmp = ReadSPI1();
    return tmp;
}

void SPIClose (void)
{
    CloseSPI1();
}
