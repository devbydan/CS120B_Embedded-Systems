/*
 * Name       -> Dan Murphy  ; dmurp006@ucr.edu
 * Lab Sec    -> 027
 * Assignment -> Lab 7, Exercise 2 (Part 2)
 * Purpose    -> Extend the earlier light game
 *				 to maintain a score on the LCD
 *				 display.
 *
 *				 The initial score is 5.
 *
 *				 Each time the user pressed the
 *				 button at the right time, the
 *				 score increments.
 * 
 *				 Each time the user fails, the
 *				 score decrements.
 *
 *				 When reaching 9, show victory
 *				 somehow.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * I acknowledge all content contained herein,
 * excluding template or example code, is my own
 * original work.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File       -> dmurp006_Lab7_Part2.c
 */ 

// Libraries
// ---------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c" /* Includes the io file necessary for the LCD to display with the timer */
#include "clock_timer.c" /* Includes the clock timer file */

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;		  // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define BUTTON (~PINA & 0x01)

enum States {first_light, second_light, third_light, end_wait, end, game_wait} light_state;

unsigned char light = 0x00;
unsigned char current_score = 0x05;
unsigned char previous_light_state = first_light;

void LCD_light_game();// Prototype ---

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

/* FUNCTION FOR THE GAME --- */
void LCD_light_game()
{
	switch(light_state)
	{
		case first_light:
		light = 0x01;
		if (light_state == BUTTON) { light_state = end_wait }
		else { second_light; }
		
		if(light_state == end_wait)
		{ if (current_score > 0x00) { current_score--; } }
		
		break;//!
		
		case second_light:
		light = 0x02;
		if(light_state == BUTTON) { light_state = end_wait; }
		else { light_state = third_light; }
		
		if (light_state == end_wait)
		{ if(current_score < 0x09) { current_score++; } }
		
		break;//!
		
		case third_light:
		light = 0x04;
		if(light_state == BUTTON) { light_state = end_wait; }
		else { first_light; }
		
		if(light_state == end_wait)
		{ if (current_score > 0x00) current_score--; } }
		
		break;//!
		
		case end_wait:
		if(light_state == BUTTON) { light_state = end_wait; }
		else { light_state = end; }
		
		break;//!
		
		case end:
		if(light_state == BUTTON) { light_state = game_wait; }
		else { light_state = end; }
		
		break;//!
		
		case game_wait:
		if(light_state == BUTTON) { light_state = game_wait; }
		else { light_state = first_light; }
		
		break;//!
		
	}// End of switch statement ---
	
	PORTB = light;
	if (current_score == 0x09) { PORTB = light + 0x08; }

	LCD_WriteData(current_score + '0');
	
	if      (current_score == 0x09) LCD_DisplayString(1, "WINNER *\^o^/*");
	else if (current_score == 0x00) LCD_DisplayString(1, "TRY AGAIN :(");
	else { LCD_init(); LCD_WriteData(current_score + '0'); }
	
}// End of LCD_light_game ----------------------

/* TEST HARNESS //////////////////////////////////////////////// */

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerOn();
	TimerSet(1000);
	LCD_init();

	while (1)
	{
		LCD_light_game();// Starts the game via function call
		while(!TimerFlag) { /* While not the flag */}
		TimerFlag = 0;		/* Resets flag --- */
		TimerReset();		/* Forces timer to reset */
		
	}// End of while(1) --- 

}// End of main ---


