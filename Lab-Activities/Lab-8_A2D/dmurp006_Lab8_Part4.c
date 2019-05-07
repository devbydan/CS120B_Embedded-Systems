/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 8, Exercise 4 (Part 4)
 * Author     -> Dan Murphy
 * Purpose    -> Design a system, using a bank of
 *				 eight LEDs, where the number of LEDs
 *				 illuminated is a representation of
 *				 how much light is detected.
 *
 *				 For example, when more light is
 *				 detected, more LEDs are illuminated.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab8_Part4.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

// Global Var ---
const unsigned short MAX = 0xC9;

void ADC_init()
{
	/*
	 * ADEN: setting this bit enables analog-to-digital conversion.
	 * ADSC: setting this bit starts the first conversion.
	 * ADATE: setting this bit enables auto-triggering. Since we are
	 *        in Free Running Mode, a new conversion will trigger whenever
	 *        the previous conversion completes.
	 */
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);

}// End of ADC_init ---

int main(void)
{
	// Registers ------------
	DDRA = 0xFE; PORTA = 0x01;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	// Function call ---
	ADC_init();
	
	// Local variables ------------------
	unsigned short AnalogToDigital = 0x00;
	unsigned short temp_swap_B = 0x00;
	
	// Infinite Loop ---
	while (1)
	{
		temp_swap_B = 0x00;
		AnalogToDigital = ADC;
		if      (AnalogToDigital <= (MAX / 0x08))       { temp_swap_B = 0x01; }
		else if (AnalogToDigital <= (2 * (MAX / 0x08))) { temp_swap_B = 0x02; }
		else if (AnalogToDigital <= (3 * (MAX / 0x08))) { temp_swap_B = 0x04; }
		else if (AnalogToDigital <= (4 * (MAX / 0x08))) { temp_swap_B = 0x08; }
		else if (AnalogToDigital <= (5 * (MAX / 0x08))) { temp_swap_B = 0x10; }
		else if (AnalogToDigital <= (6 * (MAX / 0x08))) { temp_swap_B = 0x20; }
		else if (AnalogToDigital <= (7 * (MAX / 0x08))) { temp_swap_B = 0x40; }
		else if (AnalogToDigital <= MAX)  				{ temp_swap_B = 0x80; }
		else											{ temp_swap_B = 0xFF; }
		
		PORTB = temp_swap_B;// Stores the new value in PORTB
		
	}// End of while ---

}// End of main ---
