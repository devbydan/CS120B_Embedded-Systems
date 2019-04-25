/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 4, Exercise 4 (Part 4)
 * Purpose    -> Simulate a keypad lock on a door.
 *               Unlock combo = '#' -> 'Y'
 *				 *** Is able to lock from the
 *				     inside // built off of
 *					 Lab4_Part3
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab4_Part4.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

// Enumerator ----------------------------------------------------------------
enum state_of_lock {init, pound_pressed, Y_pressed, door_locked, door_unlocked, lock_door_with_combo} lock_state;

// Mutator -------------------------------
char unlock_the_door(unsigned char lock_input);

// Test harness ---
int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	// Variables --------------------
	unsigned char lock_input = 0x00;

	// Infinite Loop ---
	while (1)
	{
		// FUNCTION CALL @@@
		unlock_the_door(lock_input);

	}// End of while(1) ---

}// End of main ---

/*
 * A household has a digital combination deadbolt lock system
 * on the doorway. The system has buttons on a keypad. Button 'X'
 * connects to PA0, 'Y' to PA1, and '#' to PA2. Pressing and releasing
 * '#', then pressing 'Y', should unlock the door by setting PB0 to 1.

 * Any other sequence fails to unlock. Pressing a button from inside the
 * house (PA7) locks the door (PB0=0). For debugging purposes, give each
 * state a number, and always write the current state to PORTC
 * (consider using the enum state variable).

 * Also, be sure to check that only one button is pressed at a time. 
 */

char unlock_the_door(unsigned char lock_input)
{	
	lock_input = PINA & 0x87;
	
	switch(lock_state)
	{
		// ____________________________________
		case init:
			lock_state = door_locked; break;
		
		// ____________________________________
		case pound_pressed:
			if( lock_input == 0x02) { lock_state = Y_pressed; }
			else { door_locked; }
			break;
			
		// ____________________________________
		case Y_pressed:
			if(lock_input == 0x80) { lock_state = door_locked; }
			else { Y_pressed; }
			break;
			
		// ____________________________________
		case door_unlocked:
			     if(lock_input == 0x04) { lock_state = lock_door_with_combo; }
			else if(lock_input == 0x80) { door_locked; }
			else { door_locked; }
			break;
			
		// ____________________________________
		case door_locked:
			if(lock_input == 0x04) { lock_state = pound_pressed; }
			else { door_locked; }
			break;
			
		// ____________________________________
		case lock_door_with_combo:
			     if(lock_input == 0x80) { lock_state = door_locked; }
			else if(lock_input == 0x02) { lock_state = door_locked; }
			else { door_unlocked; }
			break;
			
		// ____________________________________
		default:
			lock_state = door_locked;
			break;
		
	}// End of transitional switch statement ---
	
	switch(lock_state)
	{
		// ____________________________________
		case door_unlocked:
			PORTB = 0x01; break;
			
		// ____________________________________
		default:
			PORTB = 0x00; break;
		
	}// End of executional switch statement ---
	
}// End of unlock_the_door function ---
