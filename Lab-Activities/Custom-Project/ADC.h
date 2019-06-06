/*
 * Modifier -> Dan Murphy ; dmurp006@ucr.edu
 * Course   -> CSEE120B - Intro to Embedded Systems
 * Project  -> Custom Project - Digital Alarm Clock
 * Purpose  -> Contains the Analog to Digital code
 *			   for the digital alarm clock using the
 *			   Nokia 5110 LCD, PWM and Alarm SM's
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * File     -> ADC.h (ADC Template)
 */

#ifndef __ADC_H__ /* Wrap __ADC_H__ within the scope */
#define __ADC_H__ /* If __ADC_H__ is not defined, wrap within the scope */

#include "Clock_SM.h"

void ADC_init()
{
	/*
	 * ADEN: setting this bit enables analog-to-digital conversion.
	 * ADSC: setting this bit starts the first conversion.
	 * ADATE: setting this bit enables auto-triggering. Since we are
	 *        in Free Running Mode, a new conversion will trigger whenever
	 *        the previous conversion completes.
	 */
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	

}// End of ADC_init ---

#endif /* !__ADC_H__ */
