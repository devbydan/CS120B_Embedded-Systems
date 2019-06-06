/*
 * Modifier -> Dan Murphy ; dmurp006@ucr.edu
 * Course   -> CSEE120B - Intro to Embedded Systems
 * Project  -> Custom Project - Digital Alarm Clock
 * Purpose  -> Holds functionality of PWM.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File     -> PWM.h (Pulse Width Module header)
 */

#ifndef __PWM_H__ /* Wrap __TIMER_H__ within the scope */
#define __PWM_H__ /* If __TIMER_H__ is not defined, wrap within the scope */

/* Libraries needed for Atmel Studio 7.0 */
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * GLOBAL VARIABLE INFORMATION ---
 * 0.954 Hz is lowest frequency possible with this function
 * 31250 Hz is the highest frequency possible with this function
 * This is based on settings in PWM_on()
 * Passing in 0 as the frequency will stop the speaker from generating sound
 */
static double SMALLEST_FREQUENCY = 0.954;
static double LARGEST_FREQUENCY = 31250;


/* Function Documentation =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
> PWM_on():  Enables the ATmega1284’s PWM functionality.

> PWM_off(): Disables the ATmega1284’s PWM functionality.

> set_PWM(double frequency): Sets the frequency output on
	 OC3A (Output Compare pin). OC3A is pin PB6 on the micro-controller.
	 The function uses the passed in frequency to determine what the value
	 of OCR3A should be so the correct frequency will be output on PB6.
	 The equation below shows how that output frequency is calculated,
	 and how the equation in set_PWM was derived.
   =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=
*/

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
	 * If you leave it on PB6 and use Timer3 (16 bit) then it will work properly.
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

#endif /* !__PWM_H__ */
