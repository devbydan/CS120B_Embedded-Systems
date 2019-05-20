/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 11, Exercise 5 (Part 5)
 * Purpose    -> Using both rows of the LCD display,
 *				 design a game where a player controlled
 *				 character avoids oncoming obstacles.
 *				 Three buttons are used to operate the game.
 * 
 * Criteria:
 * > Use the cursor as the player controlled character.
 * > Choose a character like ‘#’, ‘*’, etc. to represent the
 *   obstacles.
 * > One button is used to pause/start the game.
 * > Two buttons are used to control the player character.
 *	 One button moves the player to the top row. The other
 *   button moves the player to the bottom row.
 * > A character position change should happen immediately after
 *   pressing the button.
 * > Minimum requirement is to have one obstacle on the top row
 *   and one obstacle on the bottom row. You may add more if you are
 *   feeling up to the challenge.
 * > Choose a reasonable movement speed for the obstacles (100ms or more).
 * > If an obstacle collides with the player, the game is paused, and a
 *   “game over” message is displayed. The game is restarted when the pause
 *   button is pressed.
 *
 * Hints:
 * > Due to the noticeable refresh rate observed when using LCD_DisplayString,
 *   instead use the combination of LCD_Cursor and LCD_WriteData to keep noticeable
 *   refreshing to a minimum.
 * > LCD cursor positions range between 1 and 32 (NOT 0 and 31).
 * > As always, dividing the design into multiple, smaller synchSMs can result in a
 *   cleaner, simpler design.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab11_Part5.c
 */

 // Necessary headers for Atmel Studio 7.0
#include <avr/io.h>
#include <avr/interrupt.h>

// Headers --------
#include "timer.h"
#include "button.h"

// Enumerated instances ---
enum ControllerStates { controller_start, controller_wait, controller_increment, controller_decrement, controller_reset} controller_state;
enum ButtonStates     { button_set, button_start } button_state;




/* TEST HARNESS --- */
int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 0s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	// Variables --------
	total_time_passed = 1;
	
	// State initialization -----------
	controller_state = controller_start;
	button_state = button_start;
	set_period = timer_period;

	// Function calls ---
	TimerSet(timer_period);
	TimerOn();

	// Infinite Loop ---
	while (1)
	{
		if (total_time_passed >= set_period) { countIterations(); total_time_passed = 0; }
		setButton();
		while (!TimerFlag){ /* Wait 1 sec */ }
		TimerFlag = 0;
		
		// Update variables ---
		total_time_passed += timer_period;
		current += timer_period;
	}
	}/* End of test harness --- */

