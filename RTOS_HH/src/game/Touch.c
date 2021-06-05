/*
 * Touch.c
 *
 * Created: 09-05-2021 12:55:43
 *  Author: Thoma
 */ 
#define F_CPU 16000000
#include "Touch.h"
#include "drivers/Touchdriver.h"
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "coordinate.h"

uint16_t getZ1Coord()
{
	uint8_t byte = 0x8B;
	CS_low();			    
	bitBangTransmit(byte);
	uint16_t  z1 = bitBangRecieve();
	CS_high();
	
	return z1;
}

uint16_t getZ2Coord()
{
	uint8_t byte = 0x83;
	CS_low();			    
	bitBangTransmit(byte);
	uint16_t  z2 = bitBangRecieve();
	CS_high();
	
	return z2;
}

uint16_t getXCoord()  
{
	uint8_t byte = 0x0B; 
	CS_low();
	bitBangTransmit(byte);
	uint16_t x = bitBangRecieve();
	bitBangTransmit(byte);
	uint16_t x1 = bitBangRecieve();
	bitBangTransmit(byte);
	uint16_t x2 = bitBangRecieve();
	CS_high();
	uint16_t  x_pos = besttwoavg(x,x1,x2);
	uint16_t  x_cal = cal_Xpos(x_pos);
	return x_cal;
}

uint16_t getYCoord()
{
	uint8_t byte = 0x9;
	CS_low();
	bitBangTransmit(byte);
	uint16_t  y = bitBangRecieve(); 
	bitBangTransmit(byte);
	uint16_t  y1 = bitBangRecieve(); // first can be noicy
	bitBangTransmit(byte);
	uint16_t  y2 = bitBangRecieve();
	CS_high();
	uint16_t  y_pos = besttwoavg(y,y1,y2);
	uint16_t  y_cal = cal_Ypos(y_pos);
	return y_cal;
}

//uint16_t getLastYCoord()
//{
	//uint8_t byte = 0x0B; 
	//CS_low();
	//bitBangTransmit(byte);
	//uint16_t  Ly = bitBangRecieve();
	//CS_high();
	//return Ly;
//}

uint16_t  besttwoavg( uint16_t  sample , uint16_t  sample1 , uint16_t  sample2 )
 {
	 uint16_t  a, b, c;
	 uint16_t  reta = 0;
	 if ( sample > sample1 )
	 a = sample - sample1; else a = sample1 - sample;
	 
	 if ( sample > sample2 )
	 b = sample - sample2; else b = sample2 - sample;
	 
	 if ( sample2 > sample1 )
	 c = sample2 - sample1; else c = sample1 - sample2;

	 if ( a <= b && a <= c )
	 reta = (sample + sample1) >> 1;
	 
	 else if ( b <= a && b <= c )
	 reta = (sample + sample2) >> 1;
	 
	 else
	 reta = (sample1 + sample2) >> 1;
	 return (reta);
 }
  	//x: 515 3540
	//y: 572 3566
	//240x320
uint16_t  cal_Xpos( uint16_t  sample)
 {
	 uint16_t  reta = 0;
	 reta = 0.0793*(sample-515);
	 return (reta);
 }
 
uint16_t  cal_Ypos( uint16_t  sample)
  {
	  uint16_t  reta = 0;
	  reta = 0.1068*(sample-572);
	  return (reta);
  }

uint16_t touched_readX()
{
	uint16_t x = getXCoord();
	return x;
}

uint16_t touched_readY()
{
	uint16_t y = getYCoord();
	return y;
}

uint16_t touchTaskOnInterrupt()
{
	static uint16_t coordinateTouch[1];
	coordinateTouch[0] = touched_readX();
	coordinateTouch[1] = touched_readY();
	EIFR |= 1 << INTF4;
	EIMSK |= 1 << INT4;

	return coordinateTouch;
}

void initTouch()
{
	bitBang_Init();
}

int touch_flag = 0;

ISR(INT4_vect)
{
	//EIMSK &= ~(1 << 4);
	touch_flag = 1;
}

