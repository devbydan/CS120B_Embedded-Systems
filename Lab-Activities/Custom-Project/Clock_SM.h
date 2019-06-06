/*
 * Modifier -> Dan Murphy ; dmurp006@ucr.edu
 * Course   -> CSEE120B - Intro to Embedded Systems
 * Project  -> Custom Project - Digital Alarm Clock
 * Purpose  -> Contains the logical code for the
 *			   current time and alarm time to be
 *			   displayed using the Nokia 5110 LCD.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File     -> Clock_SM.h (Clock State Machine)
 */

#ifndef __CLOCK_SM_H__ /* Wrap __CLOCK_SM_H__ within the scope */
#define __CLOCK_SM_H__ /* If __CLOCK_SM_H__ is not defined, wrap within the scope */

// Headers ------------
#include "io.c"
#include "Timer.h"
#include "Nokia_5110.h"
#include "Nokia_5110_chars.h"
#include "PWM.h"
#include "ADC.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * Author   -> Dan Murphy ; dmurp006@ucr.edu
 * Function -> unsigned char clock_tick()
 * Purpose  -> Contains the logical code for the 
 *			  current time and alarm time to be
 *			  displayed using the Nokia 5110 LCD.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Receives -> NONE (no parameter needed)
 * Returns  -> NONE (all output displayed)
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

unsigned char clock_tick()
{
		unsigned char on_off = 0x00;
		
		/* Nokia 5110 -> Prime LCD Display */
		nokia_lcd_init();
		nokia_lcd_clear();
		
		/* Timer -> Force timer on and provide period */
		TimerOn();
		TimerSet(1000);
		
	// Infinite Loop ---	
	while(1)
	{
		
			// -----------------------------------------
			//		  >>> Display Clock Time <<<
			// -----------------------------------------
			nokia_lcd_clear();
			nokia_lcd_write_string("Current Time:", 1);
			nokia_lcd_set_cursor(0, 10);
			nokia_lcd_write_char( (clock_hours/10) + '0', 1);
			nokia_lcd_write_char( (clock_hours%10) + '0', 1);
			nokia_lcd_write_string(":", 1);
			nokia_lcd_write_char( (clock_minutes/10) + '0', 1);
			nokia_lcd_write_char( (clock_minutes%10) + '0', 1);
			nokia_lcd_write_string(":", 1);
			nokia_lcd_write_char( (clock_seconds/10) + '0', 1);
			nokia_lcd_write_char( (clock_seconds%10) + '0', 1);
			nokia_lcd_write_string('10', 1);
			
			// -----------------------------------------
			//		  >>> Display Alarm Time <<<
			// -----------------------------------------
			
			nokia_lcd_set_cursor(0, 25);
			nokia_lcd_write_string("Alarm Time:", 1);
			nokia_lcd_set_cursor(0, 35);
			nokia_lcd_write_char( (alarm_hours/10) + '0', 1);
			nokia_lcd_write_char( (alarm_hours%10) + '0', 1);
			nokia_lcd_write_string(":", 1);
			nokia_lcd_write_char( (alarm_minutes/10) + '0', 1);
			nokia_lcd_write_char( (alarm_minutes%10) + '0', 1);
			nokia_lcd_write_string(":", 1);
			nokia_lcd_write_char( (alarm_seconds/10) + '0', 1);
			nokia_lcd_write_char( (alarm_seconds%10) + '0', 1);
			nokia_lcd_render();
		
		// Clock Functionality 24-HOUR Format ------------------------------------------
		if(clock_seconds >= 60) { clock_seconds = 0; clock_minutes++; } clock_seconds++;
	
		if(clock_minutes >= 60) { clock_minutes = 0; clock_hours++;   }
		
		if(clock_hours > 23)    { clock_hours = 0; clock_seconds = 0; }
			
		ADMUX = 0x00;// Force reset MUX
		
			/* Range of Joystick ADC found through trial and error --- */
			if((ADC) > 600)
			{
				/* Reset current time to initialized time for testing purposes */
				alarm_flag = 0;
				clock_seconds = 30;
				clock_minutes = 15;
				clock_hours = 10;
				
				/* Turn off the (blue) LED Matrix */
				PORTC = ~0xFF;
				PORTD = 0xFF;
				
			}// End of if statement -> Joystick ADC ---
			
			
			/*
			 * IF clock hours, minutes and seconds match alarm EXACTLY
			 * 
			 * Note: Clock seconds -1 adjusts for time delay when alarm is going off
			 */
		if(   (clock_hours      == alarm_hours)   &&
		      (clock_minutes    == alarm_minutes) &&
		    ( (clock_seconds-1) == alarm_seconds) )
		    { 
				alarm_flag = 1;
				
				/* >>> ERROR <<<
				 * This function does not work when called
				 * Not enough memory can be allocated for actions performed
				 * LED_Matrix_Tick(state_of_LED_Matrix);  
				 * We can either directly manipulate the PORTS or use the flag
				 *
				 * Un-comment these two commands for these ports for quick testing
 				 * PORTC = 0xFF;
 				 * PORTD = ~0xFF;
				 */

			}
				
			if(alarm_flag == 1)
			{
				if(on_off < 1)
				{
					PORTC = 0xFF;
					PORTD = ~0xFF;
					set_PWM(500);
					
				}// End of if state_of_LED_Matrixment ---
				else
				{
					PORTC = ~0xFF;
					PORTD = 0xFF;
					set_PWM(0);
					
				}// End of else state_of_LED_Matrixment ---
				
				on_off = (on_off == 1) ? 0 : on_off + 1;
				
			}// End of parent if state_of_LED_Matrixment ---
		
		
		// ---------------------
		
			while(!TimerFlag);
			TimerFlag = 0;


	
	}// End of while 1 infinite loop ---
	

			

}// End of clock tick function ---

#endif /* !__CLOCK_SM_H__ */
