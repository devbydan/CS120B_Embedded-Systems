/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 6, Exercise 2 (Part 2)
 * Purpose    -> Create a simple light game that
 *				 requires pressing a button on PA0
 *				 while the middle of the three LEDs
 *				 on PB0, PB1, and PB2 is lit.
 *
 *				 The LEDs light for 300 ms each in 
 *				 sequence.
 *	
 *				 When the button is pressed, the 
 *				 currently lit LED stays lit.
 *
 *				 Pressing the button again 
 *				 restarts the game.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab6_Part2.c
 */ 

// Libraries
// ---------
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
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

enum button_state_swap{lightMove, lightStatic} button_swap;

void main()
{
	// Registers
	// ---------
	DDRA = 0x00; PORTA = 0xFF; // set PORTA to input
	DDRB = 0xFF;			   // Set port B to output
	PORTB = 0x00;			   // Initialize PORTB to 0
	
	// Variables 
	// ---------
	int counter = 0;
	int buttonPress = 2;
	
	// Function calls 
	// --------------
	TimerSet(1000);
	TimerOn();

	
	// Infinite Loop ---
	while(1)
	{
		if( ~PINA & 0x01) { buttonPress++; }
		
		while( ~PINA & 0x01 ) { /* Pause !!! */ }
		
		// Executional switch statement ---------------------
		switch(button_swap)
		{
			case lightMove: // Light continually loops until button is pressed
			
				if(buttonPress % 2 == 0) { button_swap = lightStatic; break; }
			
				if (counter == 0) { counter++; PORTB = ~PORTB & 0x01; }
			
				else if (counter == 1) { counter++; PORTB = ~PORTB & 0x02; }
			
				else if (counter == 2) { counter++; PORTB = ~PORTB & 0x04; }
				
			break;// !
			
			case lightStatic: // Light pauses (becomes static) upon secondary press
			
				if(buttonPress % 2 == 1) { button_swap = lightMove; }
			
			break;// !
			
		}// End of switch //////////////
		
		
		
		while (!TimerFlag);	// Wait 1 second ---
		TimerFlag = 0;

	}// End of infinite while loop ---

}// End of main() test harness ------------------------------------------------------
