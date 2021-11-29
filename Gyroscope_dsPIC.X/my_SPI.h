/* 
 * File:   my_SPI.h
 * Created on November 21, 2013, 4:44 PM
 */

void InitSPI(void);                     // Initialization of SPI
void SPIWrite(unsigned int data);       // dsPIC SPI Write function
unsigned int SPIRead(void);		// dsPIC SPI Read function
void SPIClose (void);                   // close SPI
