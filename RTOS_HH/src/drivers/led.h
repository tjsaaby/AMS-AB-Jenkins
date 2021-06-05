/******************************************
* "LED.H"                                 *
* Header file for "Mega2560 LED driver"   *
* Henning Hargaard, 1.3.2015              *
*******************************************/
#ifndef led_h__
#define led_h__

void LED_InitLEDport();
void LED_WriteAllLEDs(unsigned char pattern);
void LED_TurnOnLED(unsigned char led_nr);
void LED_TurnOffLED(unsigned char led_nr);
void LED_ToggleLED(unsigned char led_nr);
#endif  // led_h__
/******************************************/
