#include <p30f4013.h>

#ifndef FCY
#define FCY (unsigned long)7372800*4   //add your operating frequency 29,491,200
#endif
#ifndef UART1_BAUD
#define UART1_BAUD 9600
#endif
#ifndef UART2_BAUD
#define UART2_BAUD 115200
#endif
#define UBRG1_VALUE (FCY/UART1_BAUD)/16 - 1
#define UBRG2_VALUE (FCY/UART2_BAUD)/16 - 1

void InitUART1(void);
void WriteUART1(unsigned int data);
void WriteUART1dec2string(unsigned int data);
void WriteStringUART1(const char * s);//for strings
void InitUART2(void);
void WriteUART2(unsigned int data);
void WriteUART2dec2string(unsigned int data);
void WriteStringUART2(const char * s);
