/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 10, Exercise 5 (Part 5)
 * Purpose    -> Buttons are connected to PA0 and PA1.
 *				 Output PORTB drives a bank of 4 LEDs.
 *				 Pressing PA0 increments a binary number
 *				 displayed on the bank of LEDs
 *				 (stopping at 9).
 * 
 *				 Pressing PA1 decrements the binary number
 *				 (stopping at 0).
 *	
 *				 If both buttons are depressed (even if not
 *				 initially simultaneously), the display resets
 *				 to 0.
 * 
 *				 If a button is held, then the display continues
 *				 to increment (or decrement) at a rate of once
 *				 per second.
 *
 *				 However, if the button is held for 3 seconds,
 *				 the incrementing/decrementing occurs once per
 *				 400 ms.
 * 
 *				 Use synchronous state machines captured in C.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab10_Part5.c
 */

 // Necessary headers for Atmel Studio 7.0
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms SM_SM_Tick_First_Cycles

/*
 * Variable Clarification ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ===================== TCCR1B ====================
 * AVR timer/counter controller register TCCR1
 *
 * bit2bit1bit0=011: pre-scaler /64 ; 00001011: 0x0B
 * SO, 8 MHz clock or 8,000,000 /64 = 125,000 SM_SM_Tick_First_Cycles/s
 * Thus, TCNT1 register will count at 125,000 SM_SM_Tick_First_Cycles/s
 * ====================================================
 *
 * ======================= OCR1A ======================
 * AVR output compare register OCR1A.
 *
 * We want a 1 ms SM_SM_Tick_First_Cycle. 0.001 s * 125,000 SM_SM_Tick_First_Cycles/s = 125
 * So when TCNT1 register equals 125,
 * 1 ms has passed. Thus, we compare to 125.
 * AVR timer interrupt mask register
 * ====================================================
 *
 */

 /*
  *  ******************* Function Breakdown ********************
  * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  * void TimerSet(unsigned char M) -- set the timer to SM_SM_Tick_First_Cycle
  *									 every M milliseconds
  * void TimerOn() -- inititialize and start the timer
  * void TimerOff() -- stop the timer
  * void TimerISR() -- called automatically when the timer SM_SM_Tick_First_Cycles,
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

// Set TimerISR() to SM_SM_Tick_First_Cycle every M ms
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}// End of TimerSet //////////////////////////////////////

unsigned char threeLEDs = 0x09;
unsigned char temp_3LED;
volatile unsigned char flag;
unsigned long long int count;
unsigned long long int frequency = 2;

/* TEST HARNESS --- */
int main(void)
{
	// Registers -------------
	DDRA = 0x00; PORTA = 0xFF; // input for A  ---
	DDRC = 0xFF; PORTC = 0x01; // output for C ---
	DDRD = 0xFF; PORTD = 0x00; // output for D ---
	
	TimerSet(200); // Sets timer to SM_Tick_First_Cycle every 200 milliseconds (Period = 2 seconds)
	TimerOn();	   // Forces the timer to turn on
	
	// Infinite loop ---
    while (1) //ONE Mealy state machine so no need for switch statements
    {
		PORTC = temp_3LED;// Priming the loop

		     if ((~PINA & 0x01)) { PORTC = (++temp_3LED); }
		
		else if ((~PINA & 0x02)) { PORTC = (--temp_3LED); }
		
		while ((~PINA & 0xFF) && (count < 15)) { SM_Tick_First_Cycle(); }
			
		while (~PINA & 0xFF) { SM_Tick_Second_Cycle(); } 
			
		count = 0;// Reset the count once it maxes out
	}
}

void SM_Tick_First_Cycle()
{
	count++;// Increment the count by 1 every time the function is called

	if(count % 5 == 0)
	{
		PORTC = temp_3LED;// Update PORTC

		     if ((~PINA & 0x01)) { PORTC = ++temp_3LED; }

		else if ((~PINA & 0x02)) { PORTC = temp_3LED--; }

	}
			
	while(!TimerFlag){ /* Wait for 1 sec */ }
	TimerFlag = 0;

}// End of state machine's first cycle tick function ---

void SM_Tick_Second_Cycle()
{
	count++;// Increment the count by 1 every time the function is called

	if(count % 2 == 0)
	{
		PORTC = temp_3LED;// Update PORTC

		     if ((~PINA & 0x01)) { PORTC = temp_3LED++; }

		else if ((~PINA & 0x02)) { PORTC = temp_3LED--; }
	}
	
	while(!TimerFlag){ /* Wait for 1 sec */ }
	TimerFlag = 0;

}// End of state machine's first cycle tick function ---