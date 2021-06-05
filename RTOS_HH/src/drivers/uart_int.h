/******************************************************
* "uart_int.h":                                       *
* Header file for Mega2560 UART driver.               *
* Using UART 1 - original af: Henning Hargaard.       *
If parameter Rx_Int <> 0 :                            *
* Receiver interrupt will be enabled                  *
* Jesper Riddersholm og Victor Duun - 14-05-2018 - AU *
******************************************************/

#pragma once
#include <stdio.h>

int UART_PutChar(char c, FILE *file);
void UART_InitUART(unsigned long BaudRate, unsigned char DataBit, char Parity, unsigned char RX_Int);
unsigned char UART_CharReady();
char UART_ReadChar();
void UART_SendChar(char Tegn);
void UART_SendString(char* Streng);


/*
void SendString(char* Streng);
void SendInteger(int Tal);
*/
/****************************************/
