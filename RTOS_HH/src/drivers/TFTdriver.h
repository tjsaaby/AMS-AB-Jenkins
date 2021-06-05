/************************************************************
  File name: "TFTdriver.h"

  Driver for "ITDB02 320 x 240 TFT display module, Version 2"
  mounted at "ITDB02 Arduino Mega2560 Shield".
  Display controller = ILI 9341.
  
  Max. uC clock frequency = 16 MHz (Tclk = 62,5 ns)

  Connections:
  DB15-DB8:   PORT A
  DB7-DB0:    PORT C

  RESETx:     PORT G, bit 0
  CSx:        PORT G, bit 1
  WRx:        PORT G, bit 2
  RS (=D/Cx): PORT D, bit 7

  
  Some inspiration taken from CPP driver https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp and converted to C
************************************************************/

#ifndef TFTdriver_H
#define TFTdriver_H

#include "color.h"

void TFT_DisplayInit();
void TFT_DisplayOff();
void TFT_DisplayOn();
void TFT_FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, RGB color);
void TFT_FillCircle(unsigned int x0, unsigned int y0, unsigned int r, RGB color);
void TFT_DrawVLine(unsigned int x, unsigned int y, unsigned int h, RGB color);
void TFT_FillTriangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, RGB color);
void TFT_DrawCharacter(unsigned int x, unsigned int y, unsigned char c, RGB color, RGB bgcolor, unsigned int size_x, unsigned int size_y);

RGB TFT_HexToTFTColor();

#endif /* TFTdriver_H_ */