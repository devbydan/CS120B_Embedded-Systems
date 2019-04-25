/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 3, Exercise 3 (Part 3)
 * Purpose    -> Create a fuel-level sensor
 *               which can set PA3...PA0
 *               to their respective values of 
 *				 0-15 (i.e. empty to full).
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab3_Part3.c
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

// FUNCTION DECLARATIONS @@@@@

// Setter function ---
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)
	{ return (b ? x | (0x01 << k) : x & ~(0x01 << k)); }
		
// Getter function ---
unsigned char GetBit(unsigned char x, unsigned char k)
	{ return ((x & (0x01 << k)) != 0); }

// MAIN @@@@@

int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF;			// Enable the PORTA pin's to receive inputs
	DDRC = 0xFF; PORTC = 0x00;			// Enable the PORTB pin's to receive outputs
	
	// Variables -------------------
	unsigned char fuel_gauge = 0x00;	// Stores PINB[5] to PINB[0] bits to display fuel level
	unsigned char fuel_level = 0x00;	// Stores PINA[3] to PINA[0]'s bits
	unsigned char car_info = 0x00;		// Stores PINA[6] to PINA[4]'s bits

	// Infinite Loop ---	
	while (1)
	{
		// Initializations ------
		fuel_level = PINA & 0x0F;	// Overwrites & initializes PINA to the first 4 bits of PINA
		car_info = PINA & 0x70;		// Overwrites & initializes PINA to the PINA[6], PINA[5] & PINA[4]
									// Meanwhile, fuel_gauge is still initialized to 0x00 ---
		
		     if(fuel_level >= 13) { fuel_gauge = 0x3F;} // Assigned value of 0011 1111
		
		else if(fuel_level >= 10) { fuel_gauge = 0x3E;} // Assigned value of 0011 1110
		
		else if(fuel_level >= 7)  { fuel_gauge = 0x3C;} // Assigned value of 0011 1100
		
		else if(fuel_level >= 5)  { fuel_gauge = 0x38;} // Assigned value of 0011 1000
		
		else if(fuel_level >= 3)  { fuel_gauge = 0x30;} // Assigned value of 0011 0000
		
		else if(fuel_level >= 1)  { fuel_gauge = 0x20;} // Assigned value of 0010 0000
		
		// Default via fall-through [else] case --------------------------------
		else {fuel_gauge = 0x00;}						  // Assigned value of 000000000 aka its over. YOU ARE OUT OF GAS... YOU ARE FUCKED
		
		PORTC = fuel_gauge;// FROM ADAM WITH LOVE  <3
		
		if (fuel_level < 5)		{ PORTC = (fuel_gauge | 0x40); }
		else                    { PORTC = fuel_gauge; } // Default via fall-through [else] case

		/* Part 3 building off of Part 2 begins below --------------------------
		 *
		 * In addition to the above, PA4 is 1 if a key is in
		 * the ignition, PA5 is one if a driver is seated, and
		 * PA6 is 1 if the driver's seatbelt is fastened. PC7
		 * should light a "Fasten seatbelt" icon if a key is
		 * in the ignition, the driver is seated, but the belt
		 * is not fastened. 
		 *
		 */
		
		if( GetBit(car_info, 4) && GetBit(car_info, 5) && !GetBit(car_info, 6) )
		   { PORTC = PORTC + 0x80; }
		// Else, PORTC remains the same -- no need for code here

	}// End of while(1) ---
	
}// End of main ---



