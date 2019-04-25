/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 4, Exercise 1 (Part 1)
 * Purpose    -> PB0 and PB1 each connect to an
 *               LED, and PB0's LED is initially
 *               on.
 *
 *               Pressing a button connected to
 *               PA0 turns off PB0's LED and turns
 *               on PB1's LED, staying that way
 *               after button release.
 *
 *               Pressing the button again turns
 *               off PB1's LED and turns on PB0's
 *               LED.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab4_Part1.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---
		


// Enumerator ----------------------------------------------------------------
enum state_of_button { LED_init, first_LED_PB0, second_LED_PB1 } button_state;

// Mutator -----------------
void illuminate(char button);


// Test harness ---
void main()
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	// Variables -----------
	unsigned char button;		// Holds button data
	unsigned char temp_swap_B;	// Swap variable for PORTB
	button_state = LED_init;	// Initializes the LED to begin illumination

	// Infinite Loop ---
	while(1)
	{
		button = PINA & 0x01;	// Gets LSB
		
		// FUNCTION CALL @@@ =====================================
		illuminate(button);			// Calls illuminate function ---
		//PORTB = temp_swap_B;	// Stores updated value into PORTB

	}// End of while(1) ---

}// End of main test harness ---


// Added functions ------------------------------------------------

void illuminate(char button)
{

	switch(button_state)
	{

	// ______________________________________________________
		case LED_init: // Initial state
		button_state = first_LED_PB0; break;

	// ______________________________________________________
		case first_LED_PB0:

		     if (button)  { button_state = second_LED_PB1; }

		else if (!button) { button_state = first_LED_PB0; }

			 break;// !

	// ______________________________________________________
		case second_LED_PB1:

		     if (button)  { button_state = first_LED_PB0; }

		else if (!button) { button_state = second_LED_PB1; }

			 break;// !

// ______________________________________________________

		default: // Defaults to init state as requested

		button_state = LED_init; break;

	} // End of switch statement -----------------------



	// Execution of button_state ----------------
	switch(button_state)
	{
		
	// ______________________________________________________
		case first_LED_PB0:// Illuminates first light
		
		PORTB = 0x01; break;


	// ______________________________________________________
		case second_LED_PB1:// Illuminates second light

		PORTB = 0x02; break;


	// ______________________________________________________
		default:// Illuminates first light by default
		
		PORTB = 0x01; break;

	}// End of switch execution statement ---

}// End of illuminate function



