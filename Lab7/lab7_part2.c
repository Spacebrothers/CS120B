/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *   *	Lab Section: 023
 *    *	Assignment: Lab #7  Exercise #2
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
#include "io.h"
#include "io.c"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpB = 0x00;
unsigned char number = 0x05;
unsigned char track = 0;
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

void Tick() {
	unsigned char button = ~PINA & 0x01;
	switch(state) {
		case initial:
			state = on1;
			break;
		case on1:
			if(button) {
				state = decrement;
			}
			else {
				state = on2;
			}
			break;
		case wait1:
			if(button) {
				state = on2;
			}
			else if(!button) {
				state = wait2;
			}
			break;
		case on2:
			if(button) {
				state = increment;
			}
			else {
				state = on3;
			}
			break;
		case wait2:
			if(button) {
				state = on3;
			}
			else if(!button){
				state = wait2;
			}
			break;
		case on3:
			if(button) {
				state = decrement;
			}
			else {
				state = on1;
			}
			break;
		case wait3:
			if(button) {
				state = on1;
			}
			else if(!button){
				state = wait3;
			}
			break;
		case increment:
			state = wait2;
			break;
		case decrement:
			if(track == 0x02) {
				state = wait3;
			}
			else if(track == 0x01) {
				state = wait1;
			}
			break;
		default:
			state = initial;
			break;
	}
	switch(state) {
		case initial:
			tmpB = 0x00;
			PORTB = tmpB;
			break;
		case on1:
			tmpB = 0x01;
			PORTB = tmpB;
			track = 0x01;
			LCD_Cursor(1);
			LCD_WriteData(number + '0');
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
			track = 0x02;
			LCD_Cursor(1);
			LCD_WriteData(number + '0');
			break;
		case wait3:
			break;
		case increment:
			if(number < 9) {
				++number;
				if(number == 9) {
					LCD_DisplayString(1,"You Win!");
				}
				else {
					LCD_Cursor(1);
					LCD_WriteData(number + '0');
				}
			}
			break;
			
		case decrement:
			if(number > 0) {
				--number;
				LCD_Cursor(1);
				LCD_WriteData(number + '0');
			}
			break;
		default:
			break;
	}
	
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	state = initial;
	LCD_init();
	TimerSet(300);
	TimerOn();
	
	
	while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}


	
