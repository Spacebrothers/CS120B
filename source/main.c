
/*      Author: An Pho
 *  Partner(s) Name: Van Truong
 *      Lab Section:
 *      Assignment: Lab #11  Exercise #1
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#include "keypad.h"
#endif

int main(void) {
	DDRC = 0xF0; PORTC = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	//DDRD = 0xFF; PORTD = 0x00;
	unsigned char x;
	while (1) {
		x = GetKeypadKey();
		switch(x) {
			case '\0': PORTB = 0x1F; break; //all 5 LEDs on
			case '1': PORTB = 0x01; break; //hex equivalent
			case '2': PORTB = 0x02; break;
			case '3': PORTB = 0x03; break;
			case '4': PORTB = 0x04; break;
			case '5': PORTB = 0x05; break;
			case '6': PORTB = 0x06; break;
			case '7': PORTB = 0x07; break;
			case '8': PORTB = 0x08; break;
			case '9': PORTB = 0x09; break;
			case 'A': PORTB = 0x0A; break;
			case 'B': PORTB = 0x0B; break;
			case 'C': PORTB = 0x0C; break; 
			case 'D': PORTB = 0x0D; break;
			case '*': PORTB = 0x0E; break;
			case '#': PORTB = 0x0F; break;
			default: PORTB = 0x1B; break;
		}
	}

	return 1;
}

			
