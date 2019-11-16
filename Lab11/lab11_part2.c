/*	Author: apho001
 *  Partner(s) Name: Van Truong
 *   *	Lab Section: 023
 *    *	Assignment: Lab #11  Exercise #2
 *     *	Exercise Description: [optional - include for your own benefit]
 *      *
 *       *	I acknowledge all content contained herein, excluding template or example
 *        *	code, is my own original work.
 *         */

#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <bit.h>
#include <timer.h>
#include <keypad.h>
#include <scheduler.h>
#include <io.c>

unsigned char k = 0;

enum Cursor_States {indexStart, incr} Cursor_State;
int MoveCursor(int state) {
	switch (state) {
		case indexStart:
			state = incr;
			break;
		case incr:
			if (k <= 51) {
				state = incr;
			}
			else {
				state = indexStart;
			}
			break;
	}
	
	switch (state) {
		case indexStart:
			k = 0;
			break;
		case incr:
			++k;
			break;
		default:
			k = 0;
			break;
	}
	return state;	
}

enum LCD_States {display} LCD_State;
int Display(int state) {
	static unsigned char statement[69] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'S', '1', '2', '0', 'B', ' ', 'i', 's', ' ', 'L', 'e', 'g', 'e', 'n', 'd', '.', '.', '.', ' ', 'w', 'a', 'i', 't', ' ', 'f', 'o', 'r', ' ', 'i', 't', ' ', 'D', 'A', 'R', 'Y', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
	static unsigned char j, temp;
	temp = k;
	switch(state) {
		case display:
			state = display;
			break;
		default:
			state = display;
			break;
	}
	
	switch (state) {
		case display:
			for (j = 1; j <= 16; ++j) {
				LCD_Cursor(j);
				LCD_WriteData(statement[temp]);
				++temp;
			}
		break;
	}
	return state;
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	//_task struct declared in scheduler.h
	static _task task1, task2; //only one SM
	_task *tasks[] = {&task1, &task2}; //task array with one task
	const unsigned short numTasks = sizeof(tasks)/sizeof(_task*); //number of arrays divided by number of tasks?
	
	task1.state = indexStart;
	task1.period = 500;
	task1.elapsedTime = task1.period;
	task1.TickFct = &MoveCursor;
	
	task2.state = display;
	task2.period = 500;
	task2.elapsedTime = task1.period;
	task2.TickFct = &Display;
	
	TimerSet(500);
	TimerOn();
	
	LCD_init();
	LCD_ClearScreen();
	
	unsigned char i;
    /* Replace with your application code */
    while (1) 
    {
		for (i = 0; i < numTasks; ++i) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 500;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
	return 0;
}




	
