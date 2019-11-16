
/*      Author: An Pho
 *  Partner(s) Name: Van Truong
 *      Lab Section:
 *      Assignment: Lab #11  Exercise #1
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */

#include <avr/io.h>
//#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>

// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;
//--------End Task scheduler data structure-----------------------------------

//--------User defined FSMs---------------------------------------------------

enum SM_1 {output} state1;

unsigned char tempB;
int Key_pad(int state){
	unsigned char x;	
	x = GetKeypadKey();
	switch(state1){
		case output:
			switch (x) {
				case '\0': tempB = 0x1F; break; // All 5 LEDs on
				case '1': tempB = 0x01; break; // hex equivalent
				case '2': tempB = 0x02; break;
				case '3': tempB = 0x03; break;
				case '4': tempB = 0x04; break;
				case '5': tempB = 0x05; break;
				case '6': tempB = 0x06; break;
				case '7': tempB = 0x07; break;
				case '8': tempB = 0x08; break;
				case '9': tempB = 0x09; break;
				case 'A': tempB = 0x0A; break;
				case 'B': tempB = 0x0B; break;
				case 'C': tempB = 0x0C; break;
				case 'D': tempB = 0x0D; break;
				case '*': tempB = 0x0E; break;
				case '0': tempB = 0x00; break;
				case '#': tempB = 0x0F; break;
				default: tempB = 0x1B; break; // Should never occur. Middle LED off.
			}
		state = output;
		PORTB = tempB;
		break;
	}
	return state;
}

// --------END User defined FSMs-----------------------------------------------

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s 
	unsigned long int keyperiod = 50; //Period for the key pad
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task1
	task1.state = output;
	task1.period = keyperiod;
	task1.elapsedTime = keyperiod;
	task1.TickFct = &Key_pad;
	//
	
	TimerSet(keyperiod);
	TimerOn();
	unsigned short i;
	while (1) 
    {
		//Scheduler Code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += keyperiod;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

			
