/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#define A0 (PINA & 0x01)

enum LT_states {LT_SMStart, LT_release0, LT_pressed0, LT_release1, LT_pressed1} LT_state;

void LightOnFnc() {
	switch(LT_state) {
		case LT_SMStart:
			LT_state = LT_release0;
			break;
		case LT_release0:
			if(A0) {
				LT_state = LT_pressed1;
			}
			else if(!A0) {
				LT_state = LT_release0;
			}
			break;
		case LT_pressed1:
			if(A0) {
				LT_state = LT_pressed1;
			}
			else if(!A0) {
				LT_state = LT_release1;
			}
			break;
		case LT_release1:
			if(A0) {
				LT_state = LT_pressed0;
			}
			else if(!A0) {
				LT_state = LT_release1;
			}
			break;
		case LT_pressed0:
			if(A0) {
				LT_state = LT_pressed0;
			}
			else if(!A0) {
				LT_state = LT_release0;
			}
			break;
		default:
			LT_state = LT_SMStart;
			break;
	}
	switch(LT_state) {
		case LT_release0:
			PORTB = 0x01;
			break;
		case LT_pressed1:
			PORTB = 0x02;
			break;
		case LT_release1:
			PORTB = 0x02;
			break;
		case LT_pressed0:
			PORTB = 0x01;
			break;
		default:
			break;
	}
}
int main(void) {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	LT_state = LT_SMStart;
	while(1) {
		LightOnFnc();
	}
	
    	return 1;
}
