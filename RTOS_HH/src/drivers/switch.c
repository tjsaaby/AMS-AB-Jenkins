/**********************************************************
* "Switch.c"                                              *
* Implementation for "Mega2560 I/O Shield" SWITCH driver. *
* Henning Hargaard, 23/9 2015                             *
***********************************************************/
#include <avr/io.h>
#define MAX_SWITCH_NR 7

// Klarg�r switch-porten
void SWITCH_InitSwitchPort() {
    // Switch-port = All inputs
    DDRA = 0;
}

// L�ser alle switches samtidigt
unsigned char SWITCH_SwitchStatus() {
    return (~PINA);
}

// Returnerer TRUE, hvis switchen med nummeret
// "switch_nr" er aktiveret - ellers returneres FALSE
unsigned char SWITCH_SwitchOn(unsigned char switch_nr) {
    unsigned char mask;
    if (switch_nr <= MAX_SWITCH_NR) {
        mask = 0b00000001 << switch_nr;
        return (~PINA & mask);
    } else
        return 0;
}