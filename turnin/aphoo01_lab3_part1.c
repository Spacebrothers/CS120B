/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *	Lab Section: 023
 *	Assignment: Lab #3  Exercise #1
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
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char inputB = 0x00;
	unsigned char inputA = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;

	while(1) {
		unsigned char countOnes = 0x00;
		unsigned char iter = 0x00;
		inputB = PINB;
		inputA = PINA;
		for(iter; iter < 0x08; iter++) {	//one loop A & B
			tempA = (inputA >> iter) & 0x01;
			tempB = (inputB >> iter) & 0x01;
			if(tempA == 0x01) {
				countOnes = countOnes + 1;
			}
			if(tempB == 0x01) {
				countOnes = countOnes + 1;
			}
		}
		PORTC = countOnes;
		//iter = 0x00;	//reset iteration
	}
	
    	return 1;
}
