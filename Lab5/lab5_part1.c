/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #1
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
//	DDRC = 0xFF; PORTC = 0x00;

	//unsigned char inputB = 0x00;
	unsigned char inputA = 0x00;
	//unsigned char tempA = 0x00;
	unsigned char outputB = 0x00;

	while(1) {
		inputA = PINA & 0x0F;
		if(inputA == 0x01 || inputA == 0x02) {
			outputB = 0x20;
		}
		else if(inputA == 0x03 || inputA == 0x04) {
			outputB = 0x30;
		}
		else if(inputA == 0x05 || inputA == 0x06) {
			outputB = 0x38;
		}
		else if (inputA == 0x07 || inputA == 0x08 || inputA == 0x09) {
			outputB = 0x3C;
		}
		else if(inputA == 0x0A || inputA == 0x0B || inputA == 0x0C) {
			outputB = 0x3E;
		}
		else if(inputA == 0x0D || inputA == 0x0E || inputA == 0x0F) {
			outputB = 0x3F;
		}
		else {
			outputB = 0x00;
		}

		if(inputA < 0x05) {
			outputB = outputB + 0x40; // 0100
		}
		PORTC = outputB;

	}
	
    	return 1;
}
