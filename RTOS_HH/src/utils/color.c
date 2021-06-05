/*
 * color.c
 *
 * Created: 15-04-2021 11:23:06
 *  Author: victo
 */
#include "color.h"


RGB COLOR_RGBFromHex(uint32_t hex)
{
    RGB rgbColor;
    rgbColor.Red = ((hex >> 16 & 0xFF) / 255.0);
    rgbColor.Green = ((hex >> 8 & 0xFF) / 255.0);
    rgbColor.Blue = ((hex & 0xFF) / 255.0);
    return rgbColor;
}
RGB COLOR_White()
{
    RGB rgb = {.Red = 31, .Green = 63, .Blue = 31};
    return rgb;
}
RGB COLOR_Black()
{
    RGB rgb = {.Red = 0, .Green = 0, .Blue = 0};
    return rgb;
}
RGB COLOR_Red() {
    RGB rgb = {.Red = 31, .Green = 0, .Blue = 0};
    return rgb;
}
RGB COLOR_Green() {
    RGB rgb = {.Red = 0, .Green = 63, .Blue = 0};
    return rgb;
}
RGB COLOR_Blue() {
    RGB rgb = {.Red = 0, .Green = 0, .Blue = 31};
    return rgb;
}
RGB COLOR_Yellow() {
    RGB rgb = {.Red = 31, .Green = 63, .Blue = 0};
    return rgb;
}
RGB COLOR_Magenta() {
    RGB rgb = {.Red = 31, .Green = 0, .Blue = 31};
    return rgb;
}
RGB COLOR_Cyan() {
    RGB rgb = {.Red = 0, .Green = 63, .Blue = 31};
    return rgb;
}