/*
 * Author  -> Dan Murphy ; dmurp006@ucr.edu
 * Course  -> CSEE120B - Intro to Embedded Systems
 * Project -> Custom Project - Digital Alarm Clock
 * Purpose -> Contains the test harness for the
 *			  digital alarm clock using Nokia 5110 
 *			  LCD, PWM and Clock SM's
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File    -> main.c (test harness)
 */


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// Libraries ---
#include <avr/io.h>			// AVR IO header
#include <util/delay.h>		// AVR delay header
#include <avr/interrupt.h>	// AVR interrupt header

// Headers -----
#include "Clock_SM.h"


// Function Prototypes ----
unsigned char clock_tick();
int LED_Matrix_Tick();
void ADC_init();

// ======================== //
// >>> Global Variables <<< //
// ======================== //

// >>> Clock Variables <<< // 
unsigned char clock_seconds = 30;
unsigned char clock_minutes = 15;
unsigned char clock_hours = 10;

// >>> Alarm Variables <<< // 
unsigned char alarm_seconds = 40;
unsigned char alarm_minutes = 15;
unsigned char alarm_hours = 10;
unsigned char alarm_flag = 0;

// >>> ADC Variables <<< // 
unsigned char AnalogToDigital = 0x00;
#define ADC_0 0

// ======================== //

/* TEST HARNESS --- */
int main(void)
{
	PWM_on();// For PWM to turn on for speaker integration ---
	
	// Registers ------------------
	DDRD = 0xFF; // PORTD as output
	DDRC = 0xFF; // PORTC as output
	DDRB = 0xFF; // PORTB as output
	
	// Infinite Loop ---
	while(1)
	{
		/* ADC -> Joystick */
		ADC_init();
	
		/* Nokia 5110 -> Prime LCD Display */
		nokia_lcd_init();
		nokia_lcd_clear();
		
		/* Nokia 5110 -> Display Current & Alarm Time */
		clock_tick();
			
	}// End of infinite while(1) loop ---
	
} // End of test harness for digital alarm clock -> using LCD, PWM, Nokia 5110





