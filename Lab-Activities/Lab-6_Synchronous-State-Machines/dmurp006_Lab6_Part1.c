/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 6, Exercise 1 (Part 1)
 * Purpose    -> Create a synchSM to blink three
 *				 LEDs connected to PB0, PB1, and
 *				 PB2 in sequence, 1 second each.
 *	
 *				 Implement that synchSM in C using
 *				 the method defined in class.
 *	
 *				 In addition to demoing your program,
 *				 you will need to show that your code
 *				 adheres entirely to the method with
 *				 no variations.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab6_Part1.c
 */ 

// Libraries
// ---------
#include <avr/io.h>
#include <avr/interrupt.h>

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

unsigned char temp_light = 0x00;

/* 
 * >>> ENUMERATOR <<<
 * 
 * Only need two states here
 * Start    -> trigger the infinite looping light sequence
 * Next LED -> go to the next LED in the sequence; light 1, 2 or 3
 *
 */
enum states {start, next_LED} state; 

// StateMachine Tick function ==========================================
void SM_Tick()
{
	PORTB = temp_light;
	
	// Executional switch statement ---
	switch(state)
	{
// __________________________________________________________
		case start:
			
			temp_light = 0x01;
			
			state = next_LED;
		
		break;
// __________________________________________________________
		case next_LED:
			
			if(temp_light == 0x04) { temp_light = 0x01; }
			
			else { temp_light = temp_light << 1; }
			
			state = next_LED;
		
		break;

	}// End of switch ---
}// End of SM_Tick ///////////////////////////////////////////

	void main()
	{
		// Registers
		// ---------
		DDRB = 0xFF; // Set port B to output
		PORTB = 0x00; // Init port B to 0s
		
		// Function calls 
		// --------------
		TimerSet(1000);
		TimerOn();

		// Initialize state to force start ---
		state = start;
		
		// Infinite Loop ---
		while(1)
		{
			SM_Tick();
			while (!TimerFlag) {}	// Wait 1 sec
			TimerFlag = 0;
		}// End of while(1) ---
		
	}// End of main test harness ------------------------------------------------------
