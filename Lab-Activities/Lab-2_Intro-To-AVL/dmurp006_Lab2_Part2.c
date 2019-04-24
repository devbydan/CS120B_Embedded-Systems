/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 2, Exercise 2 (Part 2)
 * Purpose    -> Simulate a four-car parking lot
 *               through Port A's pins 3 to 0.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab2_Part2.c
 */

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---


int main(void)
{
    // Registers -------------
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    
    // Variables -----------------------
    unsigned char count = 0x00 ;
    unsigned char parking_spot_1 = 0x00;
    unsigned char parking_spot_2 = 0x00;
	unsigned char parking_spot_3 = 0x00;
	unsigned char parking_spot_4 = 0x00;

	// Infinite Loop ---
    while(1)
    {
		parking_spot_1 = PINA & 0x01;
		parking_spot_2 = PINA & 0x02;
		parking_spot_3 = PINA & 0x04;
		parking_spot_4 = PINA & 0x08;
		
        /*
         * IF-ELSE documentation ---
         *
         * > 1 represents a car is parked in the space
         *   within the four-space parking lot
         *
         * > 0x01, 0x02, 0x04, 0x08 are bit representations
         */
        
        // Determining if parking spots are filled or open ---
		if(parking_spot_1 == 0x01) { counter0 = counter0 + 0x01; }
        
		if(parking_spot_2 == 0x02) { counter0 = counter0 + 0x01; }
		
        if(parking_spot_3 == 0x04) { counter0 = counter0 + 0x01; }
		
        if(parking_spot_4 == 0x08) { counter0 = counter0 + 0x01; }
		
		PORTC = count; // Store count's current iteration into PORTC
		
        count = 0;// Reset the counter for debugging reassurance purposes
        
    }// End of the while(1) ---
    
}// End of main ---

