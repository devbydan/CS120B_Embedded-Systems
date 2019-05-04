/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 7, Exercise 1 (Part 1)
 * Purpose    -> Buttons are connected to PA0 and
 *				 PA1.
 *				 
 *				 Output PORTC and PORTD drive the
 *				 LCD display, initially displaying
 *				 0.
 *				 
 *				 Pressing PA0 increments the display
 *				 (stopping at 9). Pressing PA1 
 *				 decrements the display (stopping 
 *				 at 0).
 *				 
 *				 If both buttons are depressed
 *				 (even if not initially simultaneously),
 *				 the display resets to 0.
 * 
 *				 If a button is held, then the display
 *				 continues to increment (or decrement)
 *				 at a rate of once per second.
 *	
 *				 Use a synchronous state machine
 *				 captured in C.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab7_Part1.c
 */ 

// Libraries
// ---------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c" /* Includes the io file necessary for the LCD to display with the timer */ 


volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;		  // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks


/*
 * Variable Clarification ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ===================== TCCR1B ====================
 * AVR timer/counter controller register TCCR1
 * 
 * bit2bit1bit0=011: pre-scaler /64 ; 00001011: 0x0B
 * SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
 * Thus, TCNT1 register will count at 125,000 ticks/s
 * ====================================================
 * 
 * ======================= OCR1A ======================
 * AVR output compare register OCR1A.
 * 
 * We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
 * So when TCNT1 register equals 125,
 * 1 ms has passed. Thus, we compare to 125.
 * AVR timer interrupt mask register
 * ====================================================
 * 
 */

/*
 *  ******************* Function Breakdown ********************
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * void TimerSet(unsigned char M) -- set the timer to tick
 *									 every M milliseconds
 * void TimerOn() -- inititialize and start the timer
 * void TimerOff() -- stop the timer
 * void TimerISR() -- called automatically when the timer ticks,
 *					  with contents filled by the user ONLY with
 *					  an instruction that sets the user-declared
 *					  global variable TimerFlag=1
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */


void TimerOn()
{
	// Variables -------------------------------------------------------------
	TCCR1B = 0x0B;  // bit3 = 0: CTC mode (clear timer on compare)
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	TIMSK1 = 0x02;  // bit1: OCIE1A -- enables compare match interrupt
	TCNT1=0;		//Initialize avr counter
	
	_avr_timer_cntcurr = _avr_timer_M; // TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts ---
	SREG |= 0x80; // 0x80: 1000000
	
}// End of TimerOn() /////////////////////////////////////

void TimerOff()
{ TCCR1B = 0x00; } // bit3bit1bit0=000: timer off

void TimerISR()
{ TimerFlag = 1; } // Initialize flag to 1

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	/*
	 * CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	 * 
	 * Count down to 0 rather than up to TOP (i.e. decrement count)
	 * 
	 * Results in a more efficient compare
	 * IF the count is 0
	 *		Call the ISR that the user uses
	 */
	
	_avr_timer_cntcurr--;
	
	if (_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}// End of ISR(TIMER1_COMPA_vect) ////////////////////////

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}// End of TimerSet //////////////////////////////////////


int main(void)
{
	// Registers
	// ---------
	DDRA = 0x00; PORTA = 0xFF; // input for A
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	// Variables
	// ---------
	unsigned char LCD_output_data = 0; // Initialize output data to 0
	
	// Timer function calls ---
	TimerSet(1000);// Sets to 1 sec intervals (timer)
	TimerOn();// Forces the timer to turn on
	
	// Initializes the LCD display ---
	LCD_init();
	
	// Prints the data on the display ---
	LCD_WriteData('0');

	// Infinite Loop ---
	while(1)
	{
		/* Sets to zero */
		if( (~PINA & 0x01) && (~PINA & 0x02) ) { LCD_output_data = 0; }
			
		/* Increment (by one) */
		else if( (~PINA & 0x01) && (LCD_output_data < 9) ) { LCD_output_data++; }
		
		/* Decrement (by one) */
		else if( (~PINA & 0x02) && (LCD_output_data > 0) ) { LCD_output_data--; }
		
		while(!TimerFlag) { }
		TimerFlag = 0;
		
		// LCD function calls ---
		LCD_Cursor(1);
		LCD_WriteData('0' + LCD_output_data);
	
	}// End of while(1) loop ---
	
}// End of main test harness ---