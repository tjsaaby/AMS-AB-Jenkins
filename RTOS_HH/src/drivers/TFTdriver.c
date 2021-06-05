/************************************************************
  File name: "TFTdriver.c"

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

  Henning Hargaard, February 14, 2019
************************************************************/

#define F_CPU 16000000
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "TFTdriver.h"
#include "gfxfont.h"
#include "stdlib.h"

// Data port definitions:
#define DATA_PORT_HIGH PORTA
#define DATA_PORT_LOW PORTC

// Control port definitions:
#define WR_PORT PORTG
#define WR_BIT 2
#define DC_PORT PORTD
#define DC_BIT 7 //"DC" signal is at the shield called RS
#define CS_PORT PORTG
#define CS_BIT 1
#define RST_PORT PORTG
#define RST_BIT 0

// Swap two bytes
#define SWAP(x, y)       \
    do                   \
    {                    \
        (x) = (x) ^ (y); \
        (y) = (x) ^ (y); \
        (x) = (x) ^ (y); \
    } while (0)

// LOCAL FUNCTIONS //

void SleepOut();
void MemoryAccessControl(unsigned char parameter);
void InterfacePixelFormat(unsigned char parameter);
void MemoryWrite();
void WritePixel(unsigned int x, unsigned int y, RGB color);
void WritePixelAtCurrentColumnPageAddress(RGB color);
void SetColumnAddress(unsigned int Start, unsigned int End);
void SetPageAddress(unsigned int Start, unsigned int End);
void DrawHLine(uint8_t StartX, uint8_t EndX, uint8_t YHeight, RGB color);
void DrawVLine(uint8_t StartY, uint8_t EndY, uint8_t XHeight, RGB color);

// ILI 9341 data sheet, page 238
void WriteCommand(unsigned int command)
{
    DATA_PORT_LOW = command;   // Lower byte [7:0]
    DC_PORT &= ~(1 << DC_BIT); // DC low
    CS_PORT &= ~(1 << CS_BIT); // CS low
    WR_PORT &= ~(1 << WR_BIT); // WR low
    _NOP();                    // Min 15 ns
    WR_PORT |= (1 << WR_BIT);  // WR high
    _NOP();                    // Min 10 ns
}
void SleepOut()
{
    WriteCommand(0x11);
}

// ILI 9341 data sheet, page 238
void WriteData(unsigned int data)
{
    DATA_PORT_HIGH = data >> 8; // Higher byte [15:8]
    DATA_PORT_LOW = data;       // Lower byte [7:0]
    DC_PORT |= (1 << DC_BIT);   // DC high
    CS_PORT &= ~(1 << CS_BIT);  // CS low
    WR_PORT &= ~(1 << WR_BIT);  // WR low
    _NOP();                     // Min 15 ns
    WR_PORT |= (1 << WR_BIT);   // WR high
    _NOP();                     // Min 10 ns
}

void MemoryAccessControl(unsigned char parameter)
{
    WriteCommand(0x36);
    WriteData(parameter);
}

void InterfacePixelFormat(unsigned char parameter)
{
    WriteCommand(0x3A);
    WriteData(parameter);
}

void MemoryWrite()
{
    WriteCommand(0x2C);
}

// Initializes (resets) the display
void TFT_DisplayInit()
{
    // Control pins are outputs
    DDRG |= 0b00000111;
    DDRD |= 0b10000000;
    // Data pins are outputs
    DDRA |= 0b11111111;
    DDRC |= 0b11111111;
    // All control pins high
    PORTG |= 0b00000111;
    PORTD |= 0b10000000;

    RST_PORT &= ~(1 << RST_BIT); // RST low
    _delay_ms(500);              // Wait 500 ms
    RST_PORT |= (1 << RST_BIT);  // RST high
    _delay_ms(130);              // Wait 130 ms

    SleepOut();      // Exit sleep mode
    TFT_DisplayOn(); // Display on

    MemoryAccessControl(0b00001000);  // Set scanning direction to BGR
    InterfacePixelFormat(0b00000101); // Set pixel format to 16 bits/pixel
}
// Set Column Address (0-239)
// OBS: Start > End
void SetColumnAddress(unsigned int Start, unsigned int End)
{
    WriteCommand(0x2A);
    WriteData(Start >> 8);
    WriteData(Start);
    WriteData(End >> 8);
    WriteData(End);
}

// Set Page Address (0-319)
// OBS: Start > End
void SetPageAddress(unsigned int Start, unsigned int End)
{
    WriteCommand(0x2B);
    WriteData(Start >> 8);
    WriteData(Start);
    WriteData(End >> 8);
    WriteData(End);
}

// Write pixel at coordinate (x,y), with specified color.
// Color: R-G-B = 5-6-5 bits = 31-63-31.
// Remember: Top left corner coordinates = (0,0). Bottom right corner coordinates = (319,239).
void WritePixel(unsigned int x, unsigned int y, RGB color)
{
    SetPageAddress(x, x);
    SetColumnAddress(y, y);
    MemoryWrite();
    WriteData(((unsigned int)color.Red << 11) | ((unsigned int)color.Green << 5) | color.Blue);
}

void WritePixelAtCurrentColumnPageAddress(RGB color)
{
    WriteData(((unsigned int)color.Red << 11) | ((unsigned int)color.Green << 5) | color.Blue);
}

// Draw a horizontal line from StartX to EndX at the YHeight of the display, with specified color.
// Color: R-G-B = 5-6-5 bits = 31-63-31.
// Remember: Top left corner coordinates = (0,0). Bottom right corner coordinates = (319,239).
void DrawHLine(uint8_t StartX, uint8_t EndX, uint8_t YHeight, RGB color)
{
    if (StartX >= EndX)
        SWAP(StartX, EndX);
    for (; StartX <= EndX; StartX++)
        WritePixel(StartX, YHeight, color);
}

// Draw a vertical line from StartY to EndY at the XHeight of the display, with specified color.
// Color: R-G-B = 5-6-5 bits = 31-63-31.
// Remember: Top left corner coordinates = (0,0). Bottom right corner coordinates = (319,239).
void DrawVLine(uint8_t StartY, uint8_t EndY, uint8_t XHeight, RGB color)
{
    if (StartY >= EndY)
        SWAP(StartY, EndY);
    for (; StartY <= EndY; StartY++)
        WritePixel(XHeight, StartY, color);
}

// Fill a rectangle with a given Width and Height from coordinates: (StartX, StartY), with specified color.
// Color: R-G-B = 5-6-5 bits = 31-63-31.
// Remember: Top left corner coordinates = (0,0). Bottom right corner coordinates = (319,239).
void TFT_FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, RGB color)
{
    SetPageAddress(StartX, StartX + Width);
    SetColumnAddress(StartY, StartY + Height);
    MemoryWrite();
    for (unsigned long i = 0; i < ((unsigned long)(Width + 1) * (Height + 1)); i++)
    {
        WritePixelAtCurrentColumnPageAddress(color);
    }
}

// Fill a circle with a given Radius from center-coordinates: (CenterX, CenterY), with specified color.
// Color: R-G-B = 5-6-5 bits = 31-63-31.
// Remember: Top left corner coordinates = (0,0). Bottom right corner coordinates = (319,239).
void TFT_FillCircle(unsigned int CenterX, unsigned int CenterY, unsigned int Radius, RGB color)
{
    for (int x = -Radius; x < Radius; x++)
    {
        int hh = (int)sqrt(Radius * Radius - x * x);
        int rx = CenterX + x;
        int ph = CenterY + hh;

        for (int y = CenterY - hh; y < ph; y++)
        {
            WritePixel(rx, y, color);
        }
    }
}

// Fill a triangle enclosed by the coordinates (x1,y1), (x2,y2) and (x3,y3), with specified color.
// Color: R-G-B = 5-6-5 bits = 31-63-31.
// Remember: Top left corner coordinates = (0,0). Bottom right corner coordinates = (319,239).
void TFT_FillTriangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, RGB color)
{
    uint8_t t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int8_t signx1, signx2, dx1, dy1, dx2, dy2;
    uint8_t e1, e2;

    // Sort vertices
    if (y1 > y2)
    {
        SWAP(y1, y2);
        SWAP(x1, x2);
    }
    if (y1 > y3)
    {
        SWAP(y1, y3);
        SWAP(x1, x3);
    }
    if (y2 > y3)
    {
        SWAP(y2, y3);
        SWAP(x2, x3);
    }

    t1x = t2x = x1;
    y = y1; // Starting points

    dx1 = (int8_t)(x2 - x1);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int8_t)(y2 - y1);

    dx2 = (int8_t)(x3 - x1);
    if (dx2 < 0)
    {
        dx2 = -dx2;
        signx2 = -1;
    }
    else
        signx2 = 1;
    dy2 = (int8_t)(y3 - y1);

    if (dy1 > dx1)
    { // swap values
        SWAP(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2)
    { // swap values
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (uint8_t)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2)
        goto next;
    e1 = (uint8_t)(dx1 >> 1);

    for (uint8_t i = 0; i < dx1;)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            i++;
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                    t1xp = signx1; //t1x += signx1;
                else
                    goto next1;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
        }
    // Move line
    next1:
        // process second line until y value is about to change
        while (1)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2; //t2x += signx2;
                else
                    goto next2;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next2:
        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        DrawHLine(minx, maxx, y, color); // Draw line from min to max points found on the y
        // Now increase y
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2)
            break;
    }
next:
    // Second half
    dx1 = (int8_t)(x3 - x2);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int8_t)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1)
    { // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else
        changed1 = false;

    e1 = (uint8_t)(dx1 >> 1);

    for (uint8_t i = 0; i <= dx1; i++)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                {
                    t1xp = signx1;
                    break;
                } //t1x += signx1;
                else
                    goto next3;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
            if (i < dx1)
                i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2;
                else
                    goto next4;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next4:

        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        DrawHLine(minx, maxx, y, color); // Draw line from min to max points found on the y
        // Now increase y
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y > y3)
            return;
    }
}

void TFT_DisplayOff()
{
    WriteCommand(0x28);
}

void TFT_DisplayOn()
{
    WriteCommand(0x29);
}

void TFT_DrawVLine(unsigned int x, unsigned int y, unsigned int h, RGB color)
{
    TFT_FillRectangle(x, y, 1, h, color);
}

void TFT_DrawCharacter(unsigned int x, unsigned int y, unsigned char c, RGB color, RGB bgcolor, unsigned int size_x, unsigned int size_y)
{
    for (int8_t i = 0; i < 5; i++)
    { // Char bitmap = 5 columns
        unsigned int line = g_font[c * 5 + i];
        for (int8_t j = 0; j < 8; j++, line >>= 1)
        {
            if (line & 1)
            {
                if (size_x == 1 && size_y == 1)
                    WritePixel(x + i, y + j, color);
                else
                    TFT_FillRectangle(x + i * size_x, y + j * size_y, size_x, size_y,
                                      color);
            }
            else if (bgcolor.Blue != color.Blue || bgcolor.Green != color.Green || bgcolor.Red != color.Red)
            {
                if (size_x == 1 && size_y == 1)
                    WritePixel(x + i, y + j, bgcolor);
                else
                    TFT_FillRectangle(x + i * size_x, y + j * size_y, size_x, size_y, bgcolor);
            }
        }
    }
    if (bgcolor.Blue != color.Blue || bgcolor.Green != color.Green || bgcolor.Red != color.Red)
    { // If opaque, draw vertical line for last column
        if (size_x == 1 && size_y == 1)
            TFT_DrawVLine(x + 5, y, 8, bgcolor);
        else
            TFT_FillRectangle(x + 5 * size_x, y, size_x, 8 * size_y, bgcolor);
    }
}
