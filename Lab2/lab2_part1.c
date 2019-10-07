/*	Author: apho001
 *      Partner(s) Name: Van Truong
 *	Lab Section: 23
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; 
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	unsigned char tmpfirst = 0x00;
	unsigned char tmpsecond = 0x00;
    /* Insert your solution below */
    while (1) {
	tmpfirst = PINA & 0x01;
	tmpsecond = PINA & 0x02;
	if((tmpfirst == 0x01) && (tmpsecond == 0x00)) {
		PORTB = 0x01;
	}
	else {
		PORTB = 0x00;
	}
    }
    return 1;
}
