/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *   *	Lab Section: 023
 *    *	Assignment: Lab #10  Exercise #3
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
unsigned char tasksNum = 3;
const unsigned long taskPeriodGCD = 2;
const unsigned long periodbl = 1000;
const unsigned long periodtl = 300;
const unsigned long periodspeaker = 2;

//Tasks
typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFnct)(int);
} task;

//3 state machines
task tasks[3];

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
	unsigned char i;
	for (i = 0; i < tasksNum; ++i) {
		if (tasks[i].elapsedTime >= tasks[i].period) {
			tasks[i].state = 				tasks[i].TickFnct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += taskPeriodGCD;
	}
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


enum blink_state {bl_init, on};
int BlinkLed(int state);

enum three_state {init, on1, on2, on3};
int ThreeLeds(int state);

enum speaker_state {speaker_init, speakeron, speakeroff};
int Speaker(int state);

int BlinkLed(int state) {
	switch(state) {
		case bl_init:
			state = on;
			break;
		case on:
			state = bl_init;
			break;
		default:
			state = bl_init;
			break;
	}
	switch(state) {
		case bl_init:
			B3 = 0x00;
			break;
		case on:
			B3 = 0x08;
			break;
		default:
			break;
	}
	return state;		
}

int ThreeLeds(int state) {
	switch(state) {
		case init:
			state = on1;
			break;
		case on1:
			state = on2;
			break;
		case on2:
			state = on3:
			break;
		case on3:
			state = on1;
			break;
		default:
			state = init;
			break;
	}
	switch(state) {
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
	return state;
}

int Speaker(int state) {
	unsigned char A2 = ~PINA & 0x04;
	switch(state) {
		case speaker_init:
			state = speakeroff;
			break;
		case speakeroff:
			if(A0) {
				state = speakeron;
			}
			else {
				state = speakeroff;
			}
			break;
		case speakeron:
			if(!A2) {
				state = speakeroff;
			}
			else {
				state = speakeron;
			}
			break;
		default:
			state = speaker_init;
			break;
	}
	switch(state) {
		case speaker_init:
			break;
		case speakeroff:
			PORTB = 0x00;
			break;
		case speakeron:
			PORTB = 0x10:
			break;
		default:
			break;
	}

}
void Combine() {
	PORTB = B | B3;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	//DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char i = 0;
	tasks[i].state = bl_init;
	tasks[i].period = periodbl;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFnct = &BlinkLed;
	++i;	
	tasks[i].state = init;
	tasks[i].period = periodtl;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFnct = &ThreeLeds;
	++i;
	tasks[i].state = speaker_init;
	tasks[i].period = periodspeaker;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFnct = &Speaker;
	TimerSet(taskPeriodGCD);
	TimerOn();

	while (1) {
		Combine();
		}

	return 1;
}


	
