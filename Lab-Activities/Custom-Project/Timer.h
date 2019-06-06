/*
 * Modifier -> Dan Murphy ; dmurp006@ucr.edu
 * Course   -> CSEE120B - Intro to Embedded Systems
 * Project  -> Custom Project - Digital Alarm Clock
 * Purpose  -> Contains the test harness for the
 *			  digital alarm clock using Nokia 5110 
 *			  LCD, PWM and Alarm SM's
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File     -> Timer.h (Timer Template)
 */


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef __TIMER_H__ /* Wrap __TIMER_H__ within the scope */
#define __TIMER_H__ /* If __TIMER_H__ is not defined, wrap within the scope */

void TimerOn()
{
	// Variables -------------------------------------------------------------
	TCCR1B = 0x0B;  // bit3 = 0: CTC mode (clear timer on compare)
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	TIMSK1 = 0x02;  // bit1: OCIE1A -- enables compare match interrupt
	TCNT1=0;		// Initialize avr counter
	
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

#endif /* !__TIMER_H__ */
