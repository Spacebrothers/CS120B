/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
/*
#define buttonX (PINA & 0x01)
#define buttonY (PINA & 0x02)
#define buttonP (PINA & 0x04)
#define buttonLock (PINA & 0x80)
*/
enum States {start, lock, press, release, unlock} states;

void DoorLock() {
	unsigned char buttonX = (PINA & 0x01);
	unsigned char buttonY = (PINA & 0x02);
	unsigned char buttonP = (PINA & 0x04);
	unsigned char buttonLock =  (PINA & 0x80);

	switch(states) {
		case start:
			states = lock;
			break;
		case lock:
			if(buttonP && !buttonX && !buttonY && !buttonLock) {
				states = press;
			}
			else if(!buttonP && !buttonX && !buttonY && buttonLock) {
				states = lock;
			}
			else {		//other combinations won't work
				states = lock;
			}
			break;
		case press:
			if(buttonP && !buttonX && !buttonY && !buttonLock) {
				states = press;
			}
			else if(!buttonP && !buttonX && !buttonY && buttonLock) {
				states = lock;
			}
			else if(!buttonP && !buttonX && !buttonY && !buttonLock) {
				states = release;
			}
			else {
				states = press;
			}
			break;
		case release:
			if(!buttonP && !buttonX && !buttonY && !buttonLock) {
				states = release;
			}
			else if(!buttonP && !buttonX && !buttonY && buttonLock) {
				states = lock;
			}
			else if(!buttonP && !buttonX && buttonY && !buttonLock) {
				states = unlock;
			}
			else {
				states = release;
			}
			break;
		case unlock:
			if(!buttonP && !buttonX && !buttonY && buttonLock) {
				states = lock;
			}
			else {
				states = unlock;
			}
			break;
		default:
			states = lock;
			break;
	}
	switch(states) {
		case start:
			//PORTC = 0x00;
			break;
		case lock:
			PORTB = 0x00;
			break;
		case press:
			PORTB = 0x02;
			break;
		case release:
			PORTB = 0x03;
			break;
		case unlock:
			PORTB = 0x01;
			break;
	}
	
}
int main(void) {

        DDRA = 0x00; PORTA = 0xFF;
        DDRB = 0xFF; PORTB = 0x00;
	states = start;
        while(1) {
                DoorLock();
//		PORTC = temp;
        }

        return 0;
}


