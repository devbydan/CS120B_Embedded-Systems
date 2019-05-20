/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 11, Exercise 1 (Part 1)
 * Purpose    -> Modify the keypad code to be in an
 *				 SM task. Then, modify the keypad SM
 *				 to utilize the simple task scheduler
 *				 format (refer to PES Chp 7).
 * 
 *				 All code from here on out should use
 *				 the task scheduler. 
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab11_Part1.c
 */
// Necessary headers for Atmel Studio 7.0
#include <avr/io.h>
#include <avr/interrupt.h>

// Headers ------------
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"

// Global variables -------------
unsigned char temp_swap_B = 0x00;

// Enumerated instance ---
enum KeypadStates{keypad_output} keypad_state;
	
int main(void)
{
	// Registers ------------
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	
	// Period for the array_of_tasks
	unsigned long int keypad_calc = 20;

	//Calculating GCD
	unsigned long int temp_GCD = 10;

	//Greatest common divisor for all array_of_tasks or smallest time unit for array_of_tasks.
	unsigned long int GCD = temp_GCD;

	//Recalculate GCD periods for scheduler
	unsigned long int keypad_period = (keypad_calc/GCD);

	//Declare an array of array_of_tasks
	static task static_task;
	task *array_of_tasks[] = { &static_task };
	const unsigned short number_of_tasks = sizeof(array_of_tasks)/sizeof(task*);

	// Task 1
	static_task.keypad_state = 0;			     // Task initial keypad_state
	static_task.period = keypad_period;			 // Task Period
	static_task.elapsedTime = keypad_period;	 // Task current elapsed time
	static_task.TickFct = &keypadTick;		     // Function pointer for the tick

	// Function calls ---
	TimerSet(GCD);
	TimerOn();

	// Infinite loop ---
	while(1)
	{
		/*
		 * FOR the amount of tasks
		 *	IF the positions are equal between the elapsed time and period
		 *		- Set the next keypad state for the current task
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

	return 0;
	
}/* End of main test harness --- */

int keypadTick(int keypad_state)
{
	unsigned char key_pushed = GetKeypadKey();
	
	/* Transitional switch statement is trivial here, not used */
	
	switch(keypad_state)/* Executional switch statement --- */
	{
		case keypad_output:
		switch (key_pushed)
		{
			case '\0':
			
				temp_swap_B = 0x1F;
			
			break; //!
			
			case '1':
			
				temp_swap_B = 0x01;
			
			break; //!
			
			case '2':
			
				temp_swap_B = 0x02;
			
			break; //!
			
			case '3':
			
				temp_swap_B = 0x03;
			
			break; //!
			
			case '4':
			
				temp_swap_B = 0x04;
			
			break; //!
			
			case '5':
			
				temp_swap_B = 0x05;
			
			break; //!
			
			case '6':
			
				temp_swap_B = 0x06;
			
			break; //!
			
			case '7':
			
				temp_swap_B = 0x07;
			
			break; //!
			
			case '8':
			
				temp_swap_B = 0x08;
			
			break; //!
			
			case '9':
			
				temp_swap_B = 0x09;
			
			break; //!
			
			case 'A':
			
				temp_swap_B = 0x0A;
			
			break; //!
			
			case 'B':
			
				temp_swap_B = 0x0B;
			
			break; //!
			
			case 'C':
			
				temp_swap_B = 0x0C;
			
			break; //!
			
			case 'D':
			
				temp_swap_B = 0x0D;
			
			break; //!
			
			case '*':
			
				temp_swap_B = 0x0E;
			
			break; //!
			
			case '0':
			
				temp_swap_B = 0x00;
			
			break; //!
			
			case '#':
			
				temp_swap_B = 0x0F;
			
			break; //!
			
			default:
			
				temp_swap_B = 0x1B; // Should never occur, but here for edge cases
			
			break; //! -> Middle LED off.
			
		}/* End of child executional switch statement --- */
		
		// Update variables ---------
		keypad_state = keypad_output;
		PORTB = temp_swap_B;
		
		break; //! Force break out of parent switch
		
	}/* End of parent switch statement --- */
	
	return keypad_state;
	
}/* End of keypadTick function --- */
