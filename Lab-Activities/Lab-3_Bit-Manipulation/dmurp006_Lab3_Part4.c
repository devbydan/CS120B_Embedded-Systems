/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 3, Exercise 4 (Part 4)
 * Purpose    -> Make the upper nibble of PINA
 *               map it to the lower nibble of
 *               PORTB. Do the same thing to
 *				 PORTC.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab3_Part4.c
 */ 


int main(void)
{
	// Registers ---
	
	
	// Variables ---
	unsigned char temp_swap_B = 0x00;
	unsigned char temp_swap_C = 0x00;

/*
 * Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4.
 * That is to say,  take the upper nibble of PINA and map it to the lower
 * nibble of PORTB, likewise take the lower nibble of PINA and map it to the
 * upper nibble of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).
*/

    while (1) 
    {
		temp_swap_B = (PINA & 0xF0) >> 4;// Shifts the bits 4 to the right
		temp_swap_C = (PINA & 0x0F) << 4;// Shifts the bits 4 to the left
		
		PORTB = temp_swap_B;// Stores the updated, bit-shifted value into PORTB
		PORTC = temp_swap_C;// Stores the updated, bit-shifted value into PORTC
		
    }// End of while(1) ---

}// End of main ---


