/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 11, Exercise 2 (Part 2)
 * Purpose    -> Use the LCD code, along with a button
 *				 and/or time delay to display the message
 *				 "CS120B is Legend... wait for it DARY!"
 * 
 *				 The string will not fit on the display all
 *				 at once, so you will need to come up with
 *				 some way to paginate or scroll the text.
 *
 * Note: If your LCD is exceptionally dim, adjust the resistance
 *		 provided by the potentiometer connected to Pin #3.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab11_Part2.c
 */

// Necessary headers for Atmel Studio 7.0
#include <avr/io.h>
#include <avr/interrupt.h>

// Headers -----------
#include "timer.h"
#include "io.c"
#include "keypad.h"
#include "scheduler.h"



/* TEST HARNESS --- */
int main()
{
	// Registers -------------
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	
	// Period for the array_of_tasks
	unsigned long int keypad_calc = 200;

	//Calculating GCD
	unsigned long int temp_GCD = 1;

	//Greatest common divisor for all array_of_tasks or smallest time unit for array_of_tasks.
	unsigned long int GCD = temp_GCD;

	//Recalculate GCD periods for scheduler
	unsigned long int keypad_tick_period = keypad_calc;

	//Declare an array of array_of_tasks
	static task static_task;
	task *array_of_tasks[] = { &static_task};
	const unsigned short number_of_tasks = sizeof(array_of_tasks)/sizeof(task*);

	// Task 1
	static_task.keypad_state = 0;					// Task initial keypad_state.
	static_task.period = keypad_tick_period;		// Task Period.
	static_task.elapsedTime = keypad_tick_period;	// Task current elapsed time.
	static_task.TickFct = &keypadTick;				// Function pointer for the tick.


	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	LCD_init();
	
	unsigned short position; // Scheduler for-loop iterator
	
	// Infinite loop ---
	while(1)
	{
		/*
		 * FOR the amount of array_of_tasks
		 *	IF the positions are equal between the elapsed time and period
		 *		- Set the next keypad keypad_state for the current task
		 *		- Reset the total time passed for the next tick
		 * Increment by 1
		 */
		for ( unsigned short position = 0; position < number_of_tasks; position++ ) {
			// Task is ready to tick
			if ( array_of_tasks[position]->elapsedTime == array_of_tasks[position]->period )
			{
				 array_of_tasks[position]->keypad_state = array_of_tasks[position]->TickFct(array_of_tasks[position]->keypad_state);
				 array_of_tasks[position]->elapsedTime = 0;
			}
			array_of_tasks[position]->elapsedTime += 1;
		}
		
		while(!TimerFlag) { /* Wait 1 sec */ };
		TimerFlag = 0;
		
	}/* End of while(1) infinite loop --- */

	return 0;// Should not reach this point, but necessary
	
}/* End of test harness --- */