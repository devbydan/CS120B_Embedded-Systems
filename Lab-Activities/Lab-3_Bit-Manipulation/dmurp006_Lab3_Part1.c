/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 3, Exercise 1 (Part 1)
 * Purpose    -> Count the number of 1's on
 *               ports A & B and output that
 *               number on port C.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab3_Part1.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---

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
	DDRA = 0x00; PORTA = 0xFF;	// Enabling port A pins to receive inputs
	DDRB = 0x00; PORTB = 0xFF;	// Enabling port B pins to receive inputs
	DDRC = 0xFF; PORTC = 0x00;	// Enabling port C pins to receive outputs

	// Variables -----------------------------
	unsigned char count_of_marked_bits = 0x00;	// Counts iterations to keep track of bits that are  
	unsigned char marked_bits_on_A = 0x00;		// Holds the marked bits in PINA set
	unsigned char marked_bits_on_B = 0x00;		// Holds the marked bits in PINA set
	
	// Infinite Loop ---
	while (1)
	{
		marked_bits_on_A = PINA;	// Stores marked_bits_on_A from PIN A set
		marked_bits_on_B = PINB;	// Stores marked_bits_on_B from PIN B set
		count_of_marked_bits = 0;	// Initializing count_of_marked_bits to 0
		    
		   /*
			* LOOP DOCUMENTATION
			* 
			* For the 8 bits in PIN A and PIN B
			* Increment one position at a time
			* Call the GetBit function to determine if the bits are marked
			*  -> Add the amount of times they're marked and add them to the count
			*
			*/
			
			for(unsigned char position = 0; position < 8; position++)
			{
				if ( GetBit(marked_bits_on_A, position) ) { count_of_marked_bits++; }
				if ( GetBit(marked_bits_on_B, position) ) { count_of_marked_bits++; }	
			}// End of for loop ---
		    
		PORTC = count_of_marked_bits;	// Send the count of marked bits to PORTC (which is the desired objective)
	    
	}// End of while(1) ---

}// End of main ---

