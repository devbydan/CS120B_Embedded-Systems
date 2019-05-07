/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 9, Exercise 3 (Part 3)
 * Purpose    -> Using the ATmega1284’s built in PWM functionality,
 *				 design a system where a short, five-second melody,
 *				 is played when a button is pressed.
 * 
 *				 NOTE: The melody must be somewhat complex
 *				       (scaling from C to B is NOT complex).
 * 
 * Criteria:
 * When the button is pressed, the melody should play until completion
 * Pressing the button again in the middle of the melody should do nothing
 * If the button is pressed and held, when the melody finishes, it should not repeat until the button is released and pressed again
 * 
 * Hints:
 * One approach is to use three arrays. One array holds the sequence of notes for the melody.
 * Another array holds the times that each note is held.
 * The final array holds the down times between adjacent notes.
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab9_Part3.c
 */ 

/* Libraries needed for Atmel Studio 7.0 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/*
 * GLOBAL VARIABLE INFORMATION ---
 * 0.954 Hz is lowest frequency possible with this function
 * 31250 Hz is the highest frequency possible with this function
 * This is based ON_state settings in PWM_on()
 * Passing in 0 as the frequency will stop the speaker from generating sound
 */
static double SMALLEST_FREQUENCY = 0.954;
static double LARGEST_FREQUENCY = 31250;

/* Musical notes --- */
#define C4 = 261.63
#define D4 = 293.66
#define E4 = 329.63
#define F4 = 349.23
#define G4 = 392.00
#define A4 = 440.00
#define B4 = 493.88
#define C5 = 523.25

/* TEST HARNESS --- */
int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF; // A input initialized to 0xFF
	DDRB = 0xFF; PORTB = 0x00; // B output initialized to 0x00

	// Function calls ---
	TimerSet(150);
	TimerOn();
	PWM_on();
	
	button_state = OFF_state;// Force initializes button_state to be OFF_state prior to infinite loop
	
	// Infinite loop ---
	while(1)
	{
		SM_Tick();
		while(!TimerFlag){ /* Wait 1 sec */ }
		TimerFlag = 0;
	}
	
}/* End of main --- */

/* Function Documentation =-=-=-=-=-=-=-=-=-=-=-=

> PWM_on(): Enables the ATmega1284’s PWM functionality.

> PWM_off(): Disables the ATmega1284’s PWM functionality.

> set_PWM(double frequency): Sets the frequency output ON_state
	 OC3A (Output Compare pin). OC3A is pin PB6 ON_state the micro-controller.
	 The function uses the passed in frequency to determine what the value
	 of OCR3A should be so the correct frequency will be output ON_state PB6.
	 The equation below shows how that output frequency is calculated,
	 and how the equation in set_PWM was derived.
*/

void set_PWM(double frequency)
{
	
	/* --- static double current_frequency variable explanation ---
	 *
	 * > Keeps track of the currently set frequency
	 *
	 * > Will only update the registers when the frequency changes, otherwise allows
	 *   music to play_notes uninterrupted.
	 *
	 */
	
	/*
	 * IF frequency is not equal to the current frequency
	 *		- Stop the timer
	 * ELSE
	 *		- Timer continues
	 * =====
	 * IF frequency is less than 0.954
	 *		- Prevent overflowing, using pre-scalar 64
	 * ELSE IF frequency is greater than 31250
	 *		- Prevent underflowing, using pre-scalar 64
	 * ELSE
	 *		- Set OCR3A to desired frequency
	 * =====
	 * Reset the counter
	 * Update current frequency post logical button_statements
	 */
	
	static double current_frequency; 
	if (frequency != current_frequency)
	{
		if (!frequency) { TCCR0B &= 0x08; }
		else            { TCCR0B |= 0x03; } 

		if (frequency < SMALLEST_FREQUENCY) { OCR0A = 0xFFFF; }

		if (frequency > LARGEST_FREQUENCY)  { OCR0A = 0x0000; }
		
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0;
		current_frequency = frequency;
		
	}/* End of if button_statement --- */

}/* End of set_PWM --- */

void PWM_on()
{
	/*
	 *  OLD //////////////////////////////////////////////////
	 * 	//TCCR0A = (1 << COM0A0 | 1 << WGM00);
	 *  //TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	 *
	 * UPDATE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 * In order to avoid unplugging it while programming the board, changed the PWM output from PB6 to PB3.
	 * However, PB3 is connected to Timer0 which is only an 8 bit counter.
	 * So when attempting to set a value like 31250 to OCR0A (8 bits), it overflowed and did not work properly.
	 * If you leave it ON_state PB6 and use Timer3 (16 bit) then it will work properly.
	 */
	
	TCCR0A = (1 << COM0A0) | (1 << WGM02) | (1 << WGM00);
	set_PWM(0);

}/* End of PWM_on --- */

void PWM_off()
{
	/* FORCES OFF --- */
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}/* End of PWM_off --- */


/* Priming the switch statements --- */
enum States { init_state, OFF_state, play_notes, wait_state } button_state;
unsigned char button = 0x00;
const double melody[40] = {493.88, 523.25,329.63, 329.63, 329.63, 329.63,493.88, 523.25,329.63, 329.63, 329.63, 329.63,349.23, 349.23, 349.23, 349.23, 329.63, 329.63, 329.63, 329.63,493.88, 523.25,329.63, 329.63, 329.63, 329.63,493.88, 523.25,329.63, 329.63, 329.63, 329.63,349.23, 349.23, 349.23, 349.23, 329.63, 329.63, 329.63, 329.63};
unsigned char index = 0x00;

void SM_Tick()
{
	button = ~PINA & 0x01;
	switch(button_state)
	{
		case init_state:
		
		button_state = OFF_state;
		
		break; //!
		
		case OFF_state:
		
			if(button) { button_state = play_notes; index = 0; }
		
			else	   { button_state= OFF_state;              } 
		
		break; //!
		
		case play_notes:
		
			if(index < 41) { button_state = play_notes; }
		
			else		   { button_state = wait_state; }
		
		break; //!
		
		case wait_state:
		
			if(button) { button_state = wait_state;	}
		
			else	   { button_state= OFF_state;   } 
		
		break; //!
		
		default:
		
		break; //!
		
	}/* End of transitional switch statement --- */
	
	switch(button_state)
	{ 
		
		case init_state:
		
		/* Initializing ... */
		
		break; //!
		
		case OFF_state:
		
			set_PWM(0);
		
		break; //!
		
		case play_notes:
		
			set_PWM(melody[index]); 
			
			++index;
		
		break; //!
		
		case wait_state:
		
			set_PWM(0);
		
		break; //!
		
		default:
		
		/* Don't do anything here --- */
		
		break; //!
		
	}/* End of executional switch statement --- */
	
}/* End of SM_Tick function --- */



