/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 5, Exercise 1 (Part 1)
 * Purpose    -> Create a fuel-level sensor
 *               which can set PA3...PA0
 *               to their respective values of
 *				 0-15 (i.e. empty to full).
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab5_Part1.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

/*
 * A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty)
 * and 15 (full). A series of LEDs connected to PC5..PC0 should light to
 * graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights.
 * If the level is 3 or 4, PC5 and PC4 light. Level 5-6 lights PC5..PC3.
 * 7-9 lights PC5..PC2. 10-12 lights PC5..PC1. 13-15 lights PC5..PC0.
 * 
 * Also, PC6 connects to a "Low fuel" icon, which should light if the level
 * is 4 or less.
 * 
 * (The example below shows the display for a fuel level of 3).   
 */

int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF;	// Enable the PORTA pin's to receive inputs
	DDRC = 0xFF; PORTC = 0x00;	// Enable the PORTB pin's to receive outputs
	
	// Variables -------------------
	unsigned char fuel_level = 0x00;	// Declare temporary variable fuel_level to hold PINA[3] to PINA[0]'s bits
	unsigned char fuel_gauge = 0x00;	// Declare temporary variable to hold PINB[5] to PINB[0] bits to display fuel level
	
	// Infinite Loop ---
	while (1)
	{
		fuel_level = PINA & 0x0F;						// Initialize PINA to the first 4 bits of PINA
		
		     if(fuel_level >= 13) { fuel_gauge = 0x3F;} // Assigned value of 0011 1111
		
		else if(fuel_level >= 10) { fuel_gauge = 0x3E;} // Assigned value of 0011 1110
		
		else if(fuel_level >= 7)  { fuel_gauge = 0x3C;} // Assigned value of 0011 1100
		
		else if(fuel_level >= 5)  { fuel_gauge = 0x38;} // Assigned value of 0011 1000
		
		else if(fuel_level >= 3)  { fuel_gauge = 0x30;} // Assigned value of 0011 0000
		
		else if(fuel_level >= 1)  { fuel_gauge = 0x20;} // Assigned value of 0010 0000
		
		// Default via fall-through [else] case -------------------------------------------------------
		else {fuel_gauge = 0x00;}					   // Assigned value of 000000000 aka its over.
			
		PORTC = fuel_gauge;
		
		if (fuel_level < 5)		{ PORTC = (fuel_gauge | 0x40); }	// Bit-wise OR
		else					{ PORTC = fuel_gauge; }				// Default via fall-through [else] case
		
	}// End of while(1) ---

}// End of main ---




