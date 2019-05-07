/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 9, Exercise 2 (Part 2)
 * Purpose    -> Using the ATmega1284’s PWM functionality,
 *				 design a system where the notes:
 *				 C4, D, E, F, G, A, B, and C5, from the table
 *				 at the top of the lab, can be generated on the
 *				 speaker by scaling up or down the eight note scale.
 * 
 *				 Three buttons are used to control the system.
 *				 One button toggles sound on/off.
 *				 The other two buttons scale up, or down, the eight note scale.
 * 
 * Criteria:
 * The system should scale up/down one note per button press.
 * When scaling down, the system should not scale below a C.
 * When scaling up, the system should not scale above a C.
 * Hints:
 * Breaking the system into multiple synchSMs could make this part easier.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab9_Part2.c
 */ 

/* Libraries needed for Atmel Studio 7.0 */
#include <avr/io.h>
#include <avr/interrupt.h>

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

/* TEST HARNESS --- */
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	PWM_on(); // Function call ---
	
	button_state = OFF_state; // Initializes button_state to OFF_state ---
	
	// Infinite loop ---
	while(1)
	{
		button = ~PINA & 0x07;
		SM_Tick();
	}

	}/* End of main --- */

void set_PWM(double frequency)
{
	
	/* --- static double current_frequency variable explanation ---
	 *
	 * > Keeps track of the currently set frequency
	 *
	 * > Will only update the registers when the frequency changes, otherwise allows
	 *   music to play uninterrupted.
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
enum States { OFF_state, ON_state, force_ON, force_OFF, play_note, increment_note_on_scale, wait_to_increment, decrement_note_on_scale, wait_to_decrement } button_state;
unsigned char button = 0x00;
const double notes[8] = {C4, D4, E4, F4, G4, A4, B4, C5}; 
unsigned char index = 0x00;

void SM_Tick()
{

	switch(button_state)
	{
		case OFF_state:
		
			if(button == 0x01) { button_state = force_ON;  }
				
			else               { button_state = OFF_state; }
				
		break; //!
		
		case force_OFF:
		
			if(button == 0x01) { button_state = force_OFF; }
		
			else               { button_state = OFF_state; } 
			
		break; //!
		
		case force_ON:
		
			if(button == 0x01) { button_state = force_ON; } 
			
			else			   { button_state = ON_state; } 
			
		break; //!
		
		case ON_state:
		
		button_state = play_note;
		
		break; //!
		
		case play_note:
		
		if     (button == 0x01) { button_state = force_OFF;         } 
			
		else if(button == 0x02) { button_state = wait_to_increment; } 
		
		else if(button == 0x04) { button_state = wait_to_decrement; } 
		
		else					{ button_state = play_note;         } 
		
		break; //!
		
		case increment_note_on_scale:
		
			button_state = play_note;
		
		break; //!
		
		case wait_to_increment:
		
			if(button == 0x02) { button_state = wait_to_increment;       } 
		
			else			   { button_state = increment_note_on_scale; } 
		
		break; //!
		
		case decrement_note_on_scale:
		
			button_state = play_note;
		
		break; //!
		
		case wait_to_decrement:
		
			if(button == 0x04) { button_state = wait_to_decrement;       } 
		
			else			   { button_state = decrement_note_on_scale; } 
		
		break; //!
		
		default:
		
		/* Don't do anything here --- */
		
		break; //!
	
	}/* End of transitional switch statement --- */
	
	
	switch(button_state)
	{
		case OFF_state:
		
		PWM_off();
		
		break; //!
		
		case force_ON:
		
		/* Force turn on */
		
		break; //!
		
		case force_OFF:
		
		/* Force turn off */
		
		break; //!
		
		case ON_state:
		
			PWM_on();
		
			index = 0x00;
		
		break; //!
		
		case play_note:
		
			set_PWM(notes[index]);
		
		break; //!
		
		case increment_note_on_scale:
		
			if(index < 7) { ++index; } 
		
		break; //!
		
		case wait_to_increment:
		
		/* Wait :-) */
		
		break; //!
		
		case decrement_note_on_scale:
		
			if(index > 0) { --index; }
		
		break; //!
		
		case wait_to_decrement:
		
		/* Wait :-) */
		
		break; //!
		
		default:
		
		/* Don't do anything here */
		
		break; //!
		
	}/* End of executional switch statement --- */
	
}/* End of SM_Tick function --- */
