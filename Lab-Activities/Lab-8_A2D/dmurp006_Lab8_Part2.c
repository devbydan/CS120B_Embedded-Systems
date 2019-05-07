/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 8, Exercise 2 (Part 2)
 * Author     -> Dan Murphy
 * Purpose    -> Revise Part 1 by replacing the
 *				 potentiometer with a photo resistor
 *				 and 330 ohm resistor.
 *
 *				 Take note of the highest ADC value
 *				 displayed when the photo resistor
 *				 is exposed to light, and the lowest
 *				 ADC value displayed when the photo
 *				 resistor is deprived of all light.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab8_Part2.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

// Global Variable ---
unsigned short AnalogToDigital = 0x00;

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
	
	// Infinite Loop ---
	while (1)
	{
		AnalogToDigital = ADC;
		/* Assign directly to ports --- */
		PORTB = (char)AnalogToDigital;
		PORTD = (char)(AnalogToDigital >> 8);
		
	}// End of while ---
	
}// End of main ---
