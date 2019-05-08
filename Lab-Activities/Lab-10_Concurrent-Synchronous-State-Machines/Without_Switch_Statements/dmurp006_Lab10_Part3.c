/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 10, Exercise 3 (Part 3)
 * Purpose    -> To the previous exercise's implementation,
 *				 connect your speaker's red wire to PB4 and
 *				 black wire to ground.
 *
 *				 Add a third task that toggles PB4 on for
 *				 2 ms and off for 2 ms as long as a switch
 *				 on PA2 is in the on position. 
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab10_Part3.c
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

unsigned char threeLEDs = 0x09;
unsigned char temp_3LED;
volatile unsigned char flag;
unsigned long long int count;

int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF; // input for A  ---
	DDRC = 0xFF; PORTC = 0x00; // output for C ---
	DDRD = 0xFF; PORTD = 0x00; // output for D ---
	
	// Function calls ---
	TimerSet(2); // Sets timer to tick every 2 milliseconds (Period = 0.002 seconds)
	TimerOn();   // Forces timer to turn on
	
	// Infinite loop ---
    while (1) //ONE Mealy state machine so no need for switch statements
    {
		// blinkLEDs FINITE STATE MACHINE 

		/* > numbers which come after the modulus operator are modifiable
		 * > original numbers were % 1000 and % 300 but the pitch was
		 *	 not as distinguishable at this range.
		 */
		if(count % 500 == 0)
		{
			if ((threeLEDs & 0x08) == (0x08)) { threeLEDs -= 0x08; }

			else							  { threeLEDs += 0x08; }
		}

		// End of blinkLEDs FSM ----------------------------------------------
			
		// threeLEDs FINITE STATE MACHINE

		temp_3LED = threeLEDs;// used for storage of the bit 3
		
		if(count % 150 == 0)
		{
			if( (threeLEDs & 0x04) == (0x04) )//threeLED's FMS
				threeLEDs = (threeLEDs & 0x08) + 0x01;
			else
				threeLEDs =  ( (threeLEDs << 1) | (temp_3LED & 0x08) ) & 0x0F;// end of threeLED's FMS
		}

		// End of threeLEDs FSM -----------------------------------------------
		
		// speaker FINITE STATE MACHINE

		// plays when a button is pressed
		if( (~PINA & 0x01) == (0x01) )
		{
			// set the speaker to chnnge every two cycles
			if (count % 2 == 0) { threeLEDs = (threeLEDs) | (0x80); }

			else				{ threeLEDs = (threeLEDs & 0x0F); }
		}
			
		// End of speaker FSM -----------------------------------------------

		PORTC = threeLEDs & 0x8F; // Sets the output

		while(!TimerFlag){ /* Wait for 1 sec */ }
		TimerFlag = 0;
		
		count++;// Increment the count by 1
		
	}// End of while(1) infinite loop ---

}// End of main ---

