/*	Author: apho001
 *      Partner(s) Name: Van Truong
 *	Lab Section: 23
 *	Assignment: Lab #2  Exercise #2
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
	DDRC = 0xFF;
	PORTC = 0x00;
	unsigned char first = 0x00;
	unsigned char second = 0x00;
	unsigned char third = 0x00;
	unsigned char fourth = 0x00;
	unsigned char cntavail = 0x00;
    /* Insert your solution below */
    while (1) {
	first = PINA & 0x01;
	second = PINA & 0x02;
	third = PINA & 0x04;
	fourth = PINA & 0x08;
	cntavail = 0x04;
	if(first == 0x01) {
		--cntavail;
	}
	if(second == 0x02) {
		--cntavail;
	}
	if(third == 0x04) {
		--cntavail;
	}
	if(fourth ==0x08) {
		--cntavail;
		
	}
	PORTC = cntavail;
	if(cntavail == 0x00) {
		PORTC = cntavail | 0x80;
	}			

    }
    return 1;
}
