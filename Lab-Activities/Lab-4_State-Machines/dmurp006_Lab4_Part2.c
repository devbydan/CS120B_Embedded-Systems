/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 4, Exercise 2 (Part 2)
 * Purpose    -> Buttons are connected to PA0
 *               and PA1.
 *
 *               Output for PORTC is initially 7.
 *               Pressing PA0 increments PORTC once
 *               (stopping at 9).
 *
 *               Pressing PA1 decrements PORTC once
 *               (stopping at 0).
 *
 *               If both buttons are depressed (even
 *               if not initially simultaneously),
 *               PORTC resets to 0.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab4_Part2.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

// Enumerator ------------------------------------------------------------
enum button_state_of_button{init, increment, decrement, wait} button_state;

// Mutator ---------------------------------------------------------------------------------
void illuminate();

// Test harness ---
int main(void)
{

	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	// Variables -------
	unsigned char temp_swap_C = 0x07; // Overwrite data in temp_swap_C
	button_state = init;	 // Initialize the button_state to init state 
	
	PORTC = temp_swap_C;

	// Infinite Loop ---
	while (1)
	{

		// FUNCTION CALL @@@
		illuminate();
		//PORTC = temp_swap_C;

	}// End of while(1) ---

}// End of main ---

void illuminate()
{
	char first_button  = ~PINA & 0x01;
	char second_button = ~PINA & 0x02;
	unsigned char temp_swap_C = 0x07;


	switch(button_state)
	{
		// ____________________________________
		case init:

		     if(!first_button && !second_button) { button_state = wait; }

		else if(first_button && !second_button)  { button_state = increment; }

		else if(!first_button && second_button)  { button_state = decrement; }

		else { button_state = init; }

		break;// !


		// ____________________________________
		case increment:

		     if(!first_button && !second_button) { button_state = init; }
				 
		else if(first_button && second_button)   { button_state = 0x00; }
		
		else { button_state = init; }

		break;// !


		// ____________________________________
		case decrement:

		     if(!first_button && !second_button) { button_state = init; }

		else if(first_button && second_button)   { button_state = 0x00; }

		else { button_state = init; }

		break;// !

		// ____________________________________
		case wait:

			 if(first_button && second_button)  { button_state = 0x00; }

		else if(first_button && !second_button) { button_state = increment; }

		else if(!first_button && second_button) { button_state = decrement; }

		else { button_state = init; }

		break;// !
		

	}// End of switch --------------------------------------------------------


	// Button execution ------------------------
	switch(button_state)
	{
		// ____________________________________
		case init:

		temp_swap_C = init; break;

		// ____________________________________
		case increment:

		if(temp_swap_C < 9) { PORTC += 0x01; }

		break;// !


		// ____________________________________
		case decrement:

		if(temp_swap_C >= 1) { PORTC -= 0x01; }

		break;// !


		// ____________________________________
		case wait:
			PORTC = 0x00; break;
			
			
		default:

		PORTC = 0x00; break;



	}// End of execution switch statement -------------------------------------



}// End of illuminate ---





