/*
 * color.h
 *
 * Created: 15-04-2021 11:23:15
 *  Author: victo
 */ 


#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

typedef struct RGB {
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
} RGB;

RGB COLOR_RGBFromHex(uint32_t hex);


RGB COLOR_White();
RGB COLOR_Black();
RGB COLOR_Red();
RGB COLOR_Green();
RGB COLOR_Blue();
RGB COLOR_Magenta();
RGB COLOR_Cyan();

#endif /* COLOR_H_ */