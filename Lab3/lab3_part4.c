/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *	Lab Section: 023
 *	Assignment: Lab #3  Exercise #4
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

	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	//unsigned char inputA = 0x00;
	//unsigned char tempB = 0x00;
	while(1) {
		unsigned char tmpA = PINA;
		unsigned char tmpB = 0x00;
		unsigned char tmpC = 0x00;

		tmpB = (tmpA >> 4) & 0xFF;
		PORTB = tmpB;

		tmpC = (tmpA << 4) & 0xFF;
		PORTC = tmpC;
	}
	
    	return 1;
}
