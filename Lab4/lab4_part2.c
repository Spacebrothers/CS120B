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
enum C_States {C_SMStart, C_initial, C_reset, C_decrement, C_increment, C_wait, C_wait1} C_state;void Count() {
	switch(C_state) {
                case C_SMStart:	
                        C_state = C_initial;
                        break;
                case C_initial:
                        if((PINA & 0x03) == 0x03) {
                                C_state = C_reset;
                        }
                        else if((PINA & 0x03) == 0x02) {
                                C_state = C_decrement;
                        }
                        else if((PINA & 0x03) == 0x01) {
                                C_state = C_increment;
                        }
                        else {
                                C_state = C_initial;
                        }
                        break;
                case C_decrement:
                       	C_state = C_wait;
                        break;
		case C_wait: 
			if((PINA & 0x03) == 0x02) {
                                C_state = C_decrement;
                        }
                        else if((PINA & 0x03) == 0x01) {
                                C_state = C_increment;
                        }
                        else if((PINA & 0x03) == 0x03) {
                                C_state = C_reset;
                        }
			else {
				C_state = C_wait;
			}
			break;
                case C_increment:
			C_state = C_wait1;
                        break;
		case C_wait1:
			if((PINA & 0x03) == 0x01) {
                                C_state = C_increment;
                        }
                        else if((PINA & 0x03) == 0x03) {
                                C_state = C_reset;
                        }
			else if((PINA & 0x03) == 0x02) {
				C_state = C_decrement;
			}
			else {
				C_state = C_wait1;
			}
                        break;
		case C_reset:
                        if((PINA & 0x03) == 0x02) {
                                C_state = C_decrement;
                        }
                        else if((PINA & 0x03) == 0x01) {
                                C_state = C_increment;
                        }
                        else if((PINA & 0x03) == 0x03) {
                                C_state = C_reset;
                        }
                        else {
                                C_state = C_initial;
                        }
                        break;

                default:
                        C_state = C_initial;
                        break;
        }
        switch(C_state) {
		case C_SMStart:
			//PORTB = 0x07;
                case C_initial:
			PORTB = 0x07;
			 break;
		case C_decrement:
                        if(PORTB > 0) {
                                PORTB = PORTB - 1;
                        }
                        break;
		case C_wait:
                        break;

                case C_increment:
                        if(PORTB < 0x09) {
                                PORTB = PORTB + 1;
                        }
                        break;
		case C_wait1:
			break;
		case C_reset:
                        PORTB = 0x00;
                        break;

                default:
                        break;
        }
}
int main(void) {

        DDRA = 0x00; PORTA = 0xFF;
        DDRB = 0xFF; PORTB = 0x00;
	C_state = C_SMStart;
  	//PORTB = 0x07;
        while(1) {
                Count();
		//PORTB = 0x07;
        }

        return 1;
}

