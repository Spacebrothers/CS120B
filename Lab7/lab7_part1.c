
/*      Author: An Pho
 *  Partner(s) Name: Van Truong
 *      Lab Section:
 *      Assignment: Lab #7  Exercise #1
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
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
unsigned char number = 0;

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

signed char i = 0;
enum ID_States {ID_SMStart, ID_Wait, ID_Init, ID_Incr, ID_HoldIncr, ID_Decr, ID_HoldDecr, ID_Reset, ID_HoldReset} ID_State;

void TickFct_Latch() {
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA & 0x02;
	
	switch(ID_State) {
		case ID_SMStart:
		ID_State = ID_Init;
		break;
		
		case ID_Wait:
		if (!A0 && !A1) {
			ID_State = ID_Wait;
		}
		else if (A0 && !A1) {
			ID_State = ID_Incr;
		}
		else if (!A0 && A1) {
			ID_State = ID_Decr;
		}
		else if (A0 && A1) {
			ID_State = ID_Reset;
		}
		break;
		
		case ID_Init:
		if (A0 && !A1) {
			ID_State = ID_Incr;
		}
		else if (!A0 && A1) {
			ID_State = ID_Decr;
		}
		else if (A0 && A1) {
			ID_State = ID_Reset;
		}
		else if (!A0 && !A1) {
			ID_State = ID_Init;
		}
		break;
		
		case ID_Incr:
		if (A0 && !A1) {
			ID_State = ID_HoldIncr;
		}
		else if (A0 && A1) {
			ID_State = ID_Reset;
		}
		else if (!A0 && !A1) {
			ID_State = ID_Wait;
		}
		break;
		
		case ID_HoldIncr:
		if (A0 && !A1) {
			ID_State = ID_HoldIncr;
		}
		else if (A0 && A1) {
			ID_State = ID_Reset;
		}
		else if (!A0 && !A1) {
			ID_State = ID_Wait;
		}
		break;
		
		case ID_Decr:
		if (!A0 && A1) {
			ID_State = ID_HoldDecr;
		}
		else if (!A0 && !A1) {
			ID_State = ID_Wait;
		}
		else if (A0 && A1) {
			ID_State = ID_Reset;
		}
		break;
		
		case ID_HoldDecr:
		if (!A0 && A1) {
			ID_State = ID_HoldDecr;
		}
		else if (!A0 && !A1) {
			ID_State = ID_Wait;
		}
		else if (A0 && A1) {
			ID_State = ID_Reset;
		}
		break;
		
		case ID_Reset:
		if (A0 && A1) {
			ID_State = ID_HoldReset;
		}
		else if (!A0 && !A1) {
			ID_State = ID_Wait;
		}
		
		case ID_HoldReset:
		if (A0 && A1) {
			ID_State = ID_HoldReset;
		}
		else if (!A0 && !A1) {
			ID_State = ID_Wait;
		}
		break;
	}
	switch (ID_State) {
		case ID_Wait:
			i = 0;
			break;
		
		case ID_Init:
			number = 0;
			//i=0;
			LCD_Cursor(1);
			LCD_WriteData(number + '0');
			i = 0;
			break;
		case ID_Incr:
			if (number < 9) {
				++number;
				LCD_Cursor(1);
				LCD_WriteData(number + '0');
				//PORTB = PORTB + 0x01;
				i = 0;
			}
		
			break;
		
		case ID_HoldIncr:
			if(!(i < 10)) {
				if (number < 9) {
					++number;
					LCD_Cursor(1);
					LCD_WriteData(number + '0');
					//PORTB = PORTB + 0x01;
				}
				i = 0;
			}	
		
			break;
		
		case ID_Decr:
			if (number > 0) {
				--number;
				LCD_Cursor(1);
				LCD_WriteData(number + '0');
				//PORTB = PORTB - 0x01;
				i = 0;
			break;
		
		case ID_HoldDecr:
			if(!(i < 10)) {
				if (number > 0) {
					--number;
					LCD_Cursor(1);
					LCD_WriteData(number + '0');
					//PORTB = PORTB - 0x01;
				}
				i =0;
			}
			break;
		case ID_Reset:
			number = 0x00;
			LCD_Cursor(1);
			LCD_WriteData(number + '0');
			i = 0;
			break;
		
		case ID_HoldReset:
			i=0;
			break;
		
		default:
			break;
	}
	++i;
}
int main (void)
{
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ID_State = ID_SMStart;
	LCD_init();
	TimerSet(100);
	TimerOn();
	
	while (1) {
		TickFct_Latch();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}


			
