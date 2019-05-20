/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 11, Exercise 4 (Part 4)
 * Purpose    -> Notice that you can visually see the
 *				 LCD refresh each character (display a
 *				 lengthy string then update to a different
 *				 lengthy string).
 *	
 *				 Design a system where a single character
 *				 is updated in the displayed string rather
 *				 than the entire string itself.
 * 
 *				 Use the functions provided in “io.c”.
 *
 *				 An example behavior would be to initially
 *				 display a lengthy string, such as “Congratulations!”.
 *				 The first keypad button pressed changes the first character
 *				 ‘C’ to the button pressed.
 *
 *				 The second keypad press changes the second character to the
 *				 second button pressed, etc. No refresh should be observable
 *				 during the character update.
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab11_Part4.c
 */

// Necessary headers for Atmel Studio 7.0
#include <avr/io.h>
#include <avr/interrupt.h>

// Headers -----------------
#include "timer.h"
#include "io.c"
#include "keypad.h"
#include "task_scheduler.h"

/* TEST HARNESS --- */
int main()
{
	// Registers -------------
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	
	// Period for the array_of_tasks
	unsigned long int keypadTick_calc = 200;

	//Calculating GCD
	unsigned long int temp_GCD = 1;

	//Greatest common divisor for all array_of_tasks or smallest time unit for array_of_tasks.
	unsigned long int GCD = temp_GCD;

	//Recalculate GCD periods for scheduler
	unsigned long int keypadTick_period = keypadTick_calc;

	//Declare an array of array_of_tasks
	static task static_task;
	task *array_of_tasks[] = { &static_task};
	const unsigned short number_of_tasks = ( sizeof(array_of_tasks) / sizeof(task*) );

	// Task 1
	static_task.keypad_state = 0;//Task initial keypad_state.
	static_task.period = keypadTick_period;//Task Period.
	static_task.elapsedTime = keypadTick_period;//Task current elapsed time.
	static_task.TickFct = &keypadTick;//Function pointer for the tick.


	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	LCD_init();
	LCD_DisplayString(1,"Congratulations!");

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
	
}/* End of test harness --- */

unsigned char temp_swap_B = 0x00;
unsigned char count = 1;

enum Keypadkeypad_states { keypad_output } keypad_keypad_state;

int keypadTick(int keypad_state)
{
	unsigned char x = GetKeypadKey();
	LCD_Cursor(count);
	switch(keypad_state) /* Executional switch statement --- */
	{
		case keypad_output:
			switch (x)
			{
				case '1':
				
				temp_swap_B = 0x01;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '2':
				
				temp_swap_B = 0x02;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '3':
				
				temp_swap_B = 0x03;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '4':
				
				temp_swap_B = 0x04;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '5':
				
				temp_swap_B = 0x05;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '6':
				
				temp_swap_B = 0x06;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '7':
				
				temp_swap_B = 0x07;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '8':
				
				temp_swap_B = 0x08;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '9':
				
				temp_swap_B = 0x09;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case 'A':
				
				temp_swap_B = 0x0A;
				
				LCD_WriteData(temp_swap_B + 0x37);
				
				++count; break; //!
				
				case 'B':
				
				temp_swap_B = 0x0B;
				
				LCD_WriteData(temp_swap_B + 0x37);
				
				++count;
				
				break; //!
				
				case 'C':
				
				temp_swap_B = 0x0C;
				
				LCD_WriteData(temp_swap_B + 0x37);
				
				++count;
				
				break; //!
				
				case 'D':
				
				temp_swap_B = 0x0D;
				
				LCD_WriteData(temp_swap_B + 0x37);
				
				++count;
				
				break; //!
				
				case '*':
				
				temp_swap_B = 0x0E;
				
				LCD_WriteData(temp_swap_B + 0x1C);
				
				++count;
				
				break; //!
				
				case '0':
				
				temp_swap_B = 0x00;
				
				LCD_WriteData(temp_swap_B + '0');
				
				++count;
				
				break; //!
				
				case '#':
				
				temp_swap_B = 0x0F;
				
				LCD_WriteData(temp_swap_B + 0x14);
				
				++count;
				
				break; //!
				
				default:
				
				/* Do nothing here --- */
				
				break; //!
			}/* End of executional switch statement --- */
			
		if(count==17) { count=1; } /* Resets counter ======================== */
			
		keypad_state = keypad_output;
		PORTB=temp_swap_B;
		
		break; //! Force break out ---
	}
	
	return keypad_state;
	
}/* End of keypadTick function --- */

