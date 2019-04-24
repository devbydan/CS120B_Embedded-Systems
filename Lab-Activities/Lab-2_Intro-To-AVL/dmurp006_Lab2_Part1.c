/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 2, Exercise 1 (Part 1)
 * Purpose    -> Illuminate an LED connected
 *               to PB0 if the garage door
 *               is open.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab2_Part1.c
 */ 

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---


int main(void)
{
    // Registers -------------
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    
	// Variables --------------
	unsigned char light_sensor = 0x00;
	unsigned char garage_door_sensor = 0x00;

    // Infinite Loop ---
	while(1)
	{
        // Overwriting variables with specific pins ---
		light_sensor = PINA & 0x01;
		garage_door_sensor = PINA & 0x02;
		
        /*
         * IF-ELSE documentation
         *
         * garage_door_sensor -> PA0 (1: door is open; 0: door is closed)
         * light_sensor -> PA1 (1: light is sensed; 0: light is not sensed)
         */
        
        // Determining if light needs to be turned on or off ---
		if( (light_sensor == 0x00) && (garage_door_sensor == 0x01) ) { PORTB = 0x01; }
		else                                                         { PORTB = 0x00; }
	
    }// End of while(1) ---
    
	return 0; // Return statement ---

}// End of main ---

