#include<p30fxxxx.h>

void InitZiroskop(void)
{
    InitSPI();
}

void SPI_Ziroskop_W(unsigned int sd_data)
{
    LATBbits.LATB2 = 0; //CS = 0
    SPIWrite(sd_data);
    LATBbits.LATB2 = 1; //CS = 1
}

signed int SPI_Ziroskop_R()
{
    unsigned char d1, d2;
    signed int gyro_ret;
    LATBbits.LATB2 = 0; //CS = 0
    d1=SPIRead();
    d2=SPIRead();
    gyro_ret=d1;
    gyro_ret<<=8;
    gyro_ret+=d2;
    gyro_ret>>=2;
    LATBbits.LATB2 = 1; //CS = 1
    return gyro_ret;
}
