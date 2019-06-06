/*
 * Modifier -> Dan Murphy ; dmurp006@ucr.edu
 * Course   -> CSEE120B - Intro to Embedded Systems
 * Project  -> Custom Project - Digital Alarm Clock
 * Purpose  -> Contains the State Machine for the
 *			   LED Matrix, used to flash in unison 
 *			   with the alarm clock sounding off.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File     -> LED_Matrix.h (LED Matrix SM Template)
 */

#ifndef __LED_MATRIX_H__ /* Wrap __LED_MATRIX_H__ within the scope */
#define __LED_MATRIX_H__ /* If __LED_MATRIX_H__ is not defined, wrap within the scope */


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * Modifier -> Dan Murphy ; dmurp006@ucr.edu
 * Function -> int LED_Matrix_Tick( ... )
 * Purpose  -> ACCESSOR function which uses a double
 *			   switch statement method which separates
 *			   the state of the alarm into two state
 *			   machines.
 *
 *			   1) Transitional switch statement: grabs
 *				  states instantiated from the declared
 *				  enumerated type of AlarmStates and ONLY
 *				  PERMITS TRANSITIONS upon conditions met
 *				  for each respective state.
 *
 *			   2) Executional switch statement: directly
 *				  manipulates the states from its preceding
 *				  transitional switch statement state machine
 *				  and ONLY PERMITS EXECUTION (i.e. actions to
 *				  be performed) upon receiving information from
 *			      the transitional switch statement.
 *
 *			   In essence, the first switch statement ONLY
 *			   serves as a transition to different states.
 *			   The second switch statement ONLY serves as an
 *			   execution of [specified] actions given the
 *			   conditions are met through the if-else statements
 *			   provided.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Receives -> int state_of_LED_Matrix
 * Returns  -> int state_of_LED_Matrix
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
enum LEDMatrixStates {LED_Matrix_Display};
	int state_of_LED_Matrix;

	int LED_Matrix_Tick(int state_of_LED_Matrix)
	{
		// Switch statement variables -----------
		static unsigned char row_value = 0x01;		// Sets the pattern displayed on columns
		static unsigned char column_value = 0x7F;	// Grounds column to display pattern
		
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  TRANSITIONAL	TRANSITIONAL	TRANSITIONAL	TRANSITIONAL	TRANSITIONAL
//		TRANSITIONAL	TRANSITIONAL	TRANSITIONAL	TRANSITIONAL	TRANSITIONAL
//			TRANSITIONAL	TRANSITIONAL	TRANSITIONAL	TRANSITIONAL	TRANSITIONAL
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		switch (state_of_LED_Matrix)
		{
			case LED_Matrix_Display: 
			
				/* Display */
			
			break; // !
			
			default: /* Safety net to ensure state transitions to display */
			
				state_of_LED_Matrix = LED_Matrix_Display;
			
			break; // !
			
		}// End of transitional switch statement ---
		
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  EXECUTIONAL		EXECUTIONAL		EXECUTIONAL		EXECUTIONAL		EXECUTIONAL
//		EXECUTIONAL		EXECUTIONAL		EXECUTIONAL		EXECUTIONAL		EXECUTIONAL
//			EXECUTIONAL		EXECUTIONAL		EXECUTIONAL		EXECUTIONAL		EXECUTIONAL
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		switch (state_of_LED_Matrix)
		{
			/*
			 * IF illuminated LED in bottom right corner . . .
			 * IF column == row display
			 *		Display far left column and top row	
			 * ELSE IF far right column was last to display (grounded)
			 *		Reset column display to far left
			 *		Bit shift down illuminated LED by one row
			 * ELSE (base case)
			 *		Bit shift displayed column one to the right
			 */
			case LED_Matrix_Display:
			
			if (column_value == 0xFE && row_value == 0x80)
			{
				column_value = 0x7F; 
				row_value = 0x01;	 
			}

			else if (column_value == 0xFE)
			{
				column_value = 0x7F;
				row_value = row_value << 1;
			}

			else { column_value = (column_value >> 1) | 0x80; }
				
			break; // !
			
			default:
			
			/* Something broke if you're here debugging --- */
			
			break; // !
			
		}// End of executional switch statement ---
		
		PORTD = row_value;			// PORTD displays row pattern
		PORTC = column_value;		// PORTC displays column pattern
		
		return state_of_LED_Matrix; // Return the current state ---
	
	};// End of LED Matrix tick function ---

#endif /* !__LED_MATRIX_H__ */
