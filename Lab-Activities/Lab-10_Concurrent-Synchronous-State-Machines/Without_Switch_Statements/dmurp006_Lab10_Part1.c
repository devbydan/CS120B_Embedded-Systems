/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 10, Exercise 1 (Part 1)
 * Purpose    -> Connect LEDs to PB0, PB1, PB2,
 *				 and PB3. In one state machine
 *				 (Three LEDs), output to a shared
 *				 variable (threeLEDs) the following
 *				 behavior:
 *	
 *				 Set only bit 0 to 1, then only bit 1,
 *				 then only bit 2 in sequence for 1 second
 *				 each.
 * 
 *				 In a second state machine (Blinking LED),
 *				 output to a shared variable (bilnkingLED)
 *				 the following behavior:
 *	
 *				 set bit 3 to 1 for 1 second, then 0 for
 *				 1 second.
 *
 *				 In a third state machine (Combine LEDs),
 *				 combine both shared variables and output
 *				 to the PORTB.
 *
 *				 Note: only one SM is writing to outputs.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab10_Part1.c
 */

// Necessary headers for Atmel Studio 7.0
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
	TCNT1 = 0;		//Initialize avr counter

	_avr_timer_cntcurr = _avr_timer_M; // TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts ---
	SREG |= 0x80; // 0x80: 1000000

}// End of TimerOn() /////////////////////////////////////

void TimerOff()
{
	TCCR1B = 0x00;
} // bit3bit1bit0=000: timer off

void TimerISR()
{
	TimerFlag = 1;
} // Initialize flag to 1

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

unsigned char threeLEDs = 0x01;
unsigned char temp_3LEDs;
volatile unsigned char flag;

int main(void)
{
	// Registers ------------
	DDRA = 0x00; PINA = 0xFF;  // input for A  ---
	DDRC = 0xFF; PORTC = 0x00; // output for C ---
	DDRD = 0xFF; PORTD = 0x00; // output for D ---
	
	// Function calls ---
	TimerSet(1000); // Sets timer to tick every second (Period = 1 second)
	TimerOn();		// Forces timer to turn on
	
	// Infinite loop ---
    while (1) //ONE Mealy state machine so no need for switch statements
    {
		// blinkLED FINITE STATE MACHINE

		if ((threeLEDs & 0x08) == (0x08)) { threeLEDs -= 0x08; }

		else							  { threeLEDs += 0x08; }

		//end of blinkLED FSM --------------------------------------------
		

		// threeLEDs FINITE STATE MACHINE

		temp_3LEDs = threeLEDs;// used for storage of the bit 3
		
		if ((threeLEDs & 0x04) == (0x04)) { threeLEDs = (threeLEDs & 0x08) + 0x01; }
		
		// last 4 bits are in error state. does not matter
		else { threeLEDs = (threeLEDs << 1) | (temp_3LEDs & 0x08); } 

		// end of threeLEDs FSM ------------------------------------------
		
		PORTC = threeLEDs & 0x0F;// Update PORTC ---
		
		while(!TimerFlag){/* Wait 1 sec */}
		TimerFlag = 0;
			
	}// End of while(1) infinite loop ---

}// End of main ---

