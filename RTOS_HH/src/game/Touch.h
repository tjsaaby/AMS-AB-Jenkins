/*
 * Touch.h
 *
 * Created: 09-05-2021 12:55:59
 *  Author: Thoma
 */ 


#ifndef TOUCH_H_
#define TOUCH_H_
#include <avr/io.h>
uint16_t getZ1Coord();
uint16_t getZ2Coord();
uint16_t getXCoord();
uint16_t getYCoord();
uint16_t getLastYCoord();
uint16_t besttwoavg( uint16_t  x , uint16_t  y , uint16_t  z );
uint16_t cal_Xpos(uint16_t );
uint16_t cal_Ypos(uint16_t );
uint16_t touched_readX();
uint16_t touched_readY();
uint16_t touchTaskOnInterrupt();
void initTouch();
extern int touch_flag;
ISR(INT4_vect);
#endif /* TOUCH_H_ */