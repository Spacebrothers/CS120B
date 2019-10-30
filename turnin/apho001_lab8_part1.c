
/*      Author: An Pho
 *  Partner(s) Name: Van Truong
 *      Lab Section:
 *      Assignment: Lab #8  Exercise #1
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main (void)
{
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	ADC_init();

	while (1) {
		
		unsigned short x = ADC;
		unsigned char tmp = (char)x;
		x = (char)(x >> 4);
		PORTB = tmp;
		PORTD = x;
	}
	
	return 1;
}


			
