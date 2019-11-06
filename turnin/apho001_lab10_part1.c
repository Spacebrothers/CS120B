/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *   *	Lab Section: 023
 *    *	Assignment: Lab #10  Exercise #1
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

unsigned char B = 0x00;
unsigned char B3 = 0x00;

//FOR TIMER
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B; //bit3 = 0: CTC mode (clear timer on compare)
	//bit2...bit0 = 11: prescaler /64
	//00001011: 0x0B
	//So 8MHz clock or 8,000,000 /64 = 125,000 ticks/s

	//AVR putput compare register OCR1A
	OCR1A = 125; //timer interrupt will be generated when TCNT == OCR1A
	TIMSK1 = 0x02; //bit1: OCIE1A -- enables compare match interrupt
	
	//initiate AVR counter
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M; // TimerISR will be called every _avr_timer_cntcurr 		milliseconds
	
	//enable global interrupts
	SREG |= 0x80; //0x90: 10000000
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum BlinkLed {bl_init, on} blinkstate;
//int BlinkLed(int state);

enum ThreeLeds {init, on1, on2, on3} threestate;
//int ThreeLeds(int state);

enum Combine {start, output} combinestate;
void BlinkLed() {
	switch(blinkstate) {
		case bl_init:
		blinkstate = on;
		break;
		case on:
		blinkstate = bl_init;
		break;
		default:
		blinkstate = bl_init;
		break;
	}
	switch(blinkstate) {
		case bl_init:
		B3 = 0x00;
		break;
		case on:
		B3 = 0x08;
		break;
		default:
		break;
	}
	//return state;
}

void ThreeLeds() {
	switch(threestate) {
		case init:
		threestate = on1;
		break;
		case on1:
		threestate = on2;
		break;
		case on2:
		threestate = on3;
		break;
		case on3:
		threestate = on1;
		break;
		default:
		threestate = init;
		break;
	}
	switch(threestate) {
		case init:
		break;
		case on1:
		B = 0x01;
		break;
		case on2:
		B = 0x02;
		break;
		case on3:
		B = 0x04;
		break;
		default:
		break;
	}
}

void Combine() {
	unsigned char tmp = 0x00;
	switch(combinestate) {
		case start:
			combinestate = output;
			break;
		case output:
			tmp = B;
			if(B3 > 0) {
				tmp = B | B3;
			}
			combinestate = output;
			PORTB = tmp;
			break;
	}
}

int main(void) {
	//DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	//unsigned char i = 0;
	blinkstate = bl_init;
	threestate = init;
	combinestate = start;
	TimerSet(1000);
	TimerOn();

	while (1) {
		BlinkLed();
		ThreeLeds();
		Combine();
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 1;
}



	
