/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 3, Exercise 5 (Part 5)
 * Author     -> Dan Murphy
 * Purpose    -> A car's passenger-seat weight sensor 
 *               outputs a 9-bit value (ranging from
 *               0 to 511) and connects to input
 *				 PD7..PD0PB0 on the micro-controller. 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab3_Part5.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---


int main(void)
{
	// Registers -------------
	DDRB = 0xFE; PORTB = 0x00;
	DDRD = 0x00; PORTD = 0xFF; 
	
	// Variables --------------------
	unsigned char temp_swap_B = 0x00;
	unsigned char temp_swap_D = 0x00;
	unsigned char airBagLight = 0x00;

	// Infinite Loop ---
	while (1)
	{

		temp_swap_B = PINB & 0x01;  // Overwrites temp_swap_B with PINB (bit-wise AND) 0x01
		temp_swap_D = PIND;			// Stores PIND into temp_swap_D

		/*
		 * A car's passenger-seat weight sensor outputs a 9-bit value
		 * (ranging from 0 to 511) and connects to input PD7..PD0PB0
		 * on the micro-controller. 

		 * If the weight is equal to or above 70 pounds, the airbag 
		 * should be enabled by setting PB1 to 1. If the weight is 
		 * above 5 but below 70, the airbag should be disabled and 
		 * an "Airbag disabled" icon should light by setting PB2 to 1. 

		 * (Neither B1 nor B2 should be set if the weight is 5 or less, 
		 * as there is no passenger). 
		 *
		 */

		     if ( (temp_swap_B == 1 && temp_swap_D >= 69)|| (temp_swap_D >= 70) )
				{ airBagLight = 0x02; }
			
		else if ( (temp_swap_B == 0 && temp_swap_D < 70 && temp_swap_D > 5) // [OR]
			   || (temp_swap_B == 1 && temp_swap_D >= 2) )
				{ airBagLight = 0x04; }
					
		// Default case ---------------
		else    { airBagLight = 0x00; }

		PORTB = temp_swap_B | airBagLight; // Stores updated bit-wise OR into PORTB

	}// End of while(1) ---

}// End of main ---
