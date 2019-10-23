/*	Author: vtruo009
 *  Partner(s) Name: An Pho
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#endif
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpB = 0x00;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG != 0x80;
}
void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TIMERISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
enum states {start, initial, on1, on2, on3} state;

void Tick() {
	switch(state) {
		case start:
			state = initial;
			break;
		case initial:
			state = on1;
			break;
		case on1:
			state = on2;
			break;
		case on2:
			state = on3;
			break;
		case on3:
			state = on1;
			break;
		default:
			state = initial;
			break;
	}
	switch(state) {
		case start:
			break;
		case initial:
			//PORTB = 0x00;
			tmpB = 0x00;
			PORTB = tmpB;
			break;
		case on1:
			tmpB = 0x01;
			PORTB = tmpB;
			break;
		case on2:	
			tmpB = 0x02;
			PORTB = tmpB;
			break;
		case on3:
			tmpB = 0x04;
			PORTB = tmpB;
			break;
		default:
			break;
	}
	
}

int main(void) {

	DDRB = 0x00; PORTB = 0xFF;
	TimerSet(1000);
	TimerOn();
    	state = initial;

    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
