/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 8, Exercise 3 (Part 3)
 * Author     -> Dan Murphy
 * Purpose    -> Design a system where an LED is
 *				 illuminated if enough light is
 *				 detected from the photo resistor.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab8_Part3.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

/* This global MAX value only works through trial and error due to ever-changing light setting */
const unsigned char MAX = 0xBB; 

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
		/*
		 * IF ADC >= MAX/2 { PORTB = 0xFF; }
		 * ELSE			   { PORTB = 0x00; }
		 * IF ADC >= MAX/2 { PORTD = 0x03; }
		 * ELSE			   { PORTB = 0x00; }
		 */
		
		PORTB = ADC >= MAX/2 ? 0xFF : 0x00;
		PORTD = ADC >= MAX/2 ? 0x03 : 0x00;
		
	}// End of while ---
	
}// End of main ---
