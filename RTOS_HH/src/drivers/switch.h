/**********************************************************
* "Switch.h"                                              *
* Header file for "Mega2560 I/O Shield" SWITCH driver.    *
* Henning Hargaard, 23/9 2015                             *
***********************************************************/
/**********************************************************/
#ifndef switch_h__
#define switch_h__

void SWITCH_InitSwitchPort();
unsigned char SWITCH_SwitchStatus();
unsigned char SWITCH_SwitchOn(unsigned char switch_nr);

#endif  // switch_h__