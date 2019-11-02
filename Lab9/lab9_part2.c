/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency > .954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) - 1; }
		
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1<< CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {init, press, on, waiton, button2, button3} states;
double Notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25}; 
unsigned char count = 0x00;
unsigned char flag = 0x00;

void MusicFnct() {
	unsigned char A0 = ~PINA & 0x01;	//on/off
	unsigned char A1 = ~PINA & 0x02;	//up
	unsigned char A2 = ~PINA & 0x04;	//down
	switch (states) {
		case init:
			states = press;
			break;
		case press:
			if(A0 && !A1 && !A2) {
				states = on;
			}
			else if(A1 && !A0 && !A2) {
				states = button2;
			}
			else if(A2 && !A1 && !A0) {
				states = button3;
			}
			else {
				states = init;
			}
			break;
		case on:
			states = waiton;
			break;
		case waiton:
			if(A0) {
				states = init;
			}
			else {
				states = waiton;
			}
			break;
		case button2:
			if(A1 && !A0 && !A2) {
				if(count < 7) {
					++count;
				}
				states = waiton;
			}
			break;
		case button3:
			if(A1 && !A0 && !A2) {
				if(count > 0) {
					--count;
				}
				states = waiton;
			}
			break;
		default:
			break;
	}
	
	switch (states) {
		case init:
			//PWM_off();
			break;
		case press:
			break;
		case waiton:
			set_PWM(Notes[count]);
			break;
		case on:
			if(flag) {
				PWM_off();
				flag = 0;
			}
			else {
				PWM_on();
				flag = 1;
			}
			break;
		case button2:
			set_PWM(Notes[count]);
			//PORTB = 0x40;
			break;
		case button3:
			set_PWM(Notes[count]);
			//PORTB = 0x40;
			break;
		default:
			break;
	}
	
}
int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	//set_PWM(261.63);
	states = init;
	while (1)
	{
		//PORTB = 0x40;
		MusicFnct();
	}
}




