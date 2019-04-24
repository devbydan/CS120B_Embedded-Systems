
/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 2, Exercise 4 (Part 4)
 * Purpose    -> An amusement park ride has 3 seats.
 *               Each seat has an 8-bit sensor
 *               connected to ports A, B & C measuring
 *               from 0-255kg.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab2_Part4.c
 */

#include <avr/io.h> // Includes the required header for Atmel Studio 7 ---


int main(void)
{
    // Registers -------------
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0x00;
    DDRC = 0x00; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    // Variables ---------------------
    unsigned char temp_port_A  = 0x00;
    unsigned char temp_port_B  = 0x00;
    unsigned char temp_port_C  = 0x00;
    unsigned char temp_port_D  = 0x00;
    unsigned char total_weight = 0x00;
    
    // Infinite Loop ---
    while (1)
    {
        temp_port_A = PINA;
        temp_port_B = PINB;
        temp_port_C = PINC;
        temp_port_D = 0x00;// Not yet needed, initializing to zero
        total_weight = temp_port_A + temp_port_B + temp_port_C;

        
        // Set PD0 to 1 if the cart's total passenger weight exceeds 140"kg"
        if(temp_port_A+temp_port_B+temp_port_C>= 140) { temp_port_D = temp_port_D | 0x01; }
        
        // Set port PD1 to 1 if the difference between A and C exceeds 80"kg"
        if(abs(temp_port_A - temp_port_C) > 80) { temp_port_D = temp_port_D | 0x02; }
        
        /*
         * Can you also devise a way to inform the operator
         * of the approx weight with remaining bits on D?
         * Sure! Why not
         */
        
        total_weight = total_weight + ((temp_port_D) >> 2);// Bit-shift by two places
        
        PORTD = temp_port_D;                      // Stores the value from temp_port_D into PORTD
        
    }// End of while(1) ---
    
    return 0;// Default return statement ---

}// End of main ---
