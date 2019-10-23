#include <avr/io.h>

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
	SREG |= 0x80;
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
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum states {start, initial, on1, wait1, on2, wait2, on3, wait3, reset} state;
void Tick() {
	unsigned char button = ~PINA & 0x01;
	switch(state) {
		case start:
			state = initial;
			break;
		case initial:
			state = on1;
			break;
		case on1:
			if(button) {
				state = wait1;
			}
			else {
				state = on2;
			}
			break;
		case wait1:
			if(button) {
				state = reset;
			}
			else if(!button) {
				state = wait1;
			}
			break;
		case on2:
			if(button) {
				state = wait2;
			}
			else {
				state = on3;
			}
			break;
		case wait2:
			if(button) {
				state = reset;
			}
			else if(!button){
				state = wait2;
			}
			break;
		case on3:
			if(button) {
				state = wait3;
			}
			else {
				state = on1;
			}
			break;
		case wait3:
			if(button) {
				state = reset;
			}
			else if(!button){
				state = wait3;
			}
			break;
		case reset:
			//state = on1;
			if(button) {
				state = on1;
			}
			else if(!button) {
				state = reset;
			}
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
		case wait1:
			break;
		case on2:
			tmpB = 0x02;
			PORTB = tmpB;
			break;
		case wait2:
			break;
		case on3:
			tmpB = 0x04;
			PORTB = tmpB;
			break;
		case wait3:
			break;
		default:
			break;
	}
	
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300);
	TimerOn();
	state = start;
	
	while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}

