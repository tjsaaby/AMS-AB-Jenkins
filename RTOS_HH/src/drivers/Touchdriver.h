/*
 * Touchdriver.h
 *
 * Created: 06-05-2021 11:50:49
 *  Author: Thoma
 */ 


#ifndef TOUCHDRIVER_H_
#define TOUCHDRIVER_H_
#include <avr/io.h>
void CS_low(void);
void CS_high(void);
void CLK_high(void);
void CLK_low(void);
void busyWait(void);

unsigned char readPin();
void bitWrite(uint16_t *x, uint8_t n, uint8_t value);
uint8_t bitRead(uint8_t *x, uint8_t n);
void bitBang_Init(void);
void bitBangTransmit(uint8_t byte);
uint16_t bitBangRecieve(void);

#endif /* TOUCHDRIVER_H_ */