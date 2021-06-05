/*
 * Touchdriver.c
 *
 * Created: 06-05-2021 11:50:38
 *  Author: Thoma
 */ 
#define F_CPU 16000000
#include "Touchdriver.h"
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <stdio.h>


#define BYTE_LENGTH 8
#define HIGH 1
#define LOW 0
#define CLK PH3
#define CS PE3
#define DIN PG5
#define DOUT PE5
#define IRQ_TOUCH PE4


void bitBang_Init()
{
	DDRH  |= 0b00001000;
	DDRE  |= 0b00011000;
	DDRE  &= 0b11001111;
	DDRG  |= 0b00100000;
	PORTE |= (HIGH << CS);
	PORTE |= (HIGH << DIN);	
	unsigned char mask = 0b00000001 << 3;	
	PORTH = PINH & (~mask); //PORTH &= ~mask;
}

void bitWrite(uint16_t *x, uint8_t n, uint8_t value) {
	if (value)
	*x |= (1 << n);
	else
	*x &= ~(1 << n);
}

uint8_t bitRead(uint8_t *x, uint8_t n) {
	return (*x & (1 << n)) ? 1 : 0;
}

unsigned char readPin()
{
	unsigned char mask;
	mask = (PINE & 0b00100000);
	return mask >> DOUT;
}

void CS_low()
{
	char mask = 0b00000001 << 3;	
	PORTE = PINE & (~mask);
	_delay_us(2);
}

void CS_high()
{
	PORTE |= (HIGH << CS);
}

void CLK_high()
{
	PORTH |= (HIGH << CLK);	
}

void CLK_low()
{
	char mask = 0b00000001 << 3;	
	PORTH = PINH & (~mask);
}

void busyWait()
{	
	CLK_low();
	_delay_us(1);
	CLK_high();
	_delay_us(1);
}

void bitBangTransmit(uint8_t byte)
{
	for(int i=0; i<BYTE_LENGTH; i++)		// 8 bits in a byte
	{
		uint8_t bit = bitRead(&byte, i);
		if (bit == 0)
		{
			unsigned char mask = 0b00000001 << 5;	
			PORTG = PING & (~mask);
		}
		else if(bit == 1)
		{
			PORTG |= bit << DIN;
		}
		CLK_low();
		_delay_us(1);
		CLK_high();
		_delay_us(1);
	}
}

uint16_t bitBangRecieve()
{
	busyWait(); 
	uint16_t receive = 0;					// Return the received data
	for (int i = BYTE_LENGTH*2; i>0; i--)
	{
		CLK_high();
		_delay_us(1);
		CLK_low();
		_delay_us(1);
		uint8_t pin = readPin();	
		bitWrite(&receive, i-1, pin);   // Fetch bit into array
	}
	receive = receive>>4;
	return receive;
}