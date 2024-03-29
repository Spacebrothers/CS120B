/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *   *	Lab Section: 023
 *    *	Assignment: Lab #8  Exercise #3
 *     *	Exercise Description: [optional - include for your own benefit]
 *      *
 *       *	I acknowledge all content contained herein, excluding template or example
 *        *	code, is my own original work.
 *         */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#endif


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned short max = 0x8F;

	ADC_init();

	while (1) {
		
		unsigned short x = ADC;
		
		if(x >= max/2 ) {
			PORTB = 0x01;
		}
		else {
			PORTB = 0;
		}


	return 1;
}


	
