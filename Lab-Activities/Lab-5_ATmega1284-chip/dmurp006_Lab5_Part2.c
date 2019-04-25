/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 5, Exercise 2 (Part 2)
 * Purpose    -> Warns user if fuel gauge is
 *               below a certain point which 
 *               shows a low-fuel light.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab5_Part2.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

// Enumerator ------------------------------------------------------------
enum button_state_of_button{init, increment, increment_wait, decrement, decrement_wait, wait, reset_wait} button_state;

// Mutator ---------------------------------------------------------------------------------
unsigned char illuminate(char temp_swap_B);

// Test harness ---
int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	// Variables -------
	char temp_swap_B = 0x00; // Overwrite data in temp_swap_B
	button_state = init;	 // Initialize the button_state to init state 

	// Infinite Loop ---
	while (1)
	{
		// FUNCTION CALL @@@
		temp_swap_B = illuminate(temp_swap_B);
		PORTB = temp_swap_B;

	}// End of while(1) ---

}// End of main ---

unsigned char illuminate(char temp_swap_B)
{
	unsigned char first_button  = ~PINA & 0x01;
	unsigned char second_button = ~PINA & 0x02;


	switch(button_state)
	{
		// ____________________________________
		case init:
			if(first_button)	   { button_state = increment_wait; }
			
			else if(second_button) { button_state = decrement_wait; }
			
			else { button_state = init; }	
					
			break;// !


		// ____________________________________
		case increment:
			button_state = init;
			
			break;// !
			
		// ____________________________________
		case increment_wait:
			if(first_button && second_button) { button_state = reset_wait; }
			
			else if (first_button)			  { button_state = increment_wait; }
			
			else { button_state = increment; }
				
			break;// !


		// ____________________________________
		case decrement:
			button_state = init;
			break;// !
		
		// ____________________________________
		case decrement_wait:
			if(first_button && second_button) { button_state = reset_wait; }
			else if (second_button) { button_state = decrement_wait; }
			else { button_state = decrement; }
			break;// !

		// ____________________________________
		case reset_wait:
			if(first_button || second_button) { button_state = reset_wait; }
			else { button_state = init; }
			break;// !

		// ____________________________________
		default:
			button_state = init;
			break;// !


	}// End of switch --------------------------------------------------------


	// Button execution ------------------------
	switch(button_state)
	{

		// ____________________________________
		case increment:
			if (temp_swap_B < 0x09) return temp_swap_B = temp_swap_B + 0x01;
			else { return temp_swap_B; } 
			break;// !

		// ____________________________________
		case decrement:
			if(temp_swap_B > 0) { return temp_swap_B = temp_swap_B - 1; }
			else { return temp_swap_B; } 
			break;// !
		
		// ____________________________________
		case reset_wait:
			return 0x00;
			break;// !

		// ____________________________________
		default:

			return temp_swap_B = temp_swap_B; break;



	}// End of execution switch statement -------------------------------------



}// End of illuminate ---





