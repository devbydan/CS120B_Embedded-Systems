# UCR EE/CS 120B 
## Lab 5: Using the ATMega1284 microcontroller (Hardware light display) (2 Days)
Breadboard
Review electronics, use of breadboards, and safety when working with electronics. 

Before touching any of the sensitive circuit components, please make sure to discharge static electricity off of yourself. To properly discharge static electricity, touch a grounded metal component (i.e. metal on the lab machines). The discharge of static electricity onto a component is known as Electrostatic discharge (ESD). ESD can damage electronic components so take caution when handling sensitive electronic equipment.

Note: Always use anti-static bags and tubes as needed.

The Lab’s Google Doc Collection has such helpful files such as the AVR FAQ, AVRISP user guide, ATmega1284 datasheet, and more. Some of these contain troubleshooting sections, and it is a good idea to familiarize yourself with the datasheet for the ATmega1284.

On a breadboard, the holes along a red line (also called a rail or bus) are connected internally, likewise, the holes along a blue line are also connected internally. Red is for power, blue/black for negative (ground). For the rest of the breadboard, the only internal connections are among the holes in a 5-hole group in a row.  



Note: You may use a graphical resistor calculator or reference a chart if you are unfamiliar with resistors. Resistors are always measured in Ohms (Ω).

The resistor is needed to limit current flow through the LED; the LED datasheet indicates the maximum current that should flow through, typically around 10 mA - 20 mA. It also indicates the typical voltage drop that will occur across the LED; the remaining voltage drop V will occur across the resistor, so the current that will flow can be computed using V = IR (e.g., if the battery outputs 6V and the LED voltage drop is 4V, the remaining 2V will occur across the resistor. If R is 200Ω, then current will be 2V / 200Ω = 10 mA). 

Example: (resistance calculation for the diagram above)
VS: direct current voltage source
VD: voltage required to turn on LED
VR: voltage across the resistor in series with LED
I  : maximum current flowing through LED for correct operation
VS = 6 volts (V), VD = 4 volts (V), I = 10 milliamps (mA)
VR = VS - VD
R = VR/I
R = (6V - 2V) / 10mA
R = 200Ω

Let's keep the LED and resistor there as an indicator of when the board is powered.  
Basic board wiring for power
If you are using a battery pack through the regulator, follow this walkthrough first.

Important Note: If at any time while the power supply is connected you smell burning, or feel excessive heat coming from a component disconnect your power source immediately and check your wiring! Do not touch any components, until they cool down. If this happens you possibly have miswired the board and are burning components. They may still work afterwards but they also may be toast! Be sure to consider this when debugging.
Add the microcontroller
Before you begin ensure the board is NOT powered and you have discharged any static 
electricity from your person. 

First you will want to determine the orientation of the microcontroller (sometimes abbreviated μC). There is a notch in the top of the microcontroller and pin 1 is marked with a dot or arrow as pictured below. 




Carefully insert the microcontroller chip onto the breadboard as shown below. Place the top pin in row 21 to simplify determining the chip's pin numbers, such that pin 1 (upper left of chip) is in row 21, pin 2 in 22, pin 3 in 23, etc. The chip's horizontal pin spacing is slightly wider than the board, so place one side partly in, then angle the other side's pins in carefully; once all pins are in holes, gently press downwards until the chip snaps into place. If you have to remove the chip for any reason, use the chip extractor tool -- Video demo-ing chip insertion/extraction.

Next, we will add wires for both VCC and ground (gnd) to both sides of the microcontroller per the pinout above. Note that VCC and gnd connect to specific pin numbers on each side as shown -- be careful to note that VCC is Aabove ground on the left, but ground is above AVCC (which is VCC; see datasheet) on the right. 

VCC (red rail) connects to both pin 10 and 30. Ground (blue rail) connects to pin 11 and 31. Add the (optional) capacitors across VCC and ground, to smooth out the 5V being supplied to the microcontroller (reducing spikes or dips). This ceramic capacitor doesn't have a positive or negative side, so don't worry about its orientation (polarity). Confirm your setup with the pictures below.



Connecting the programmer
If you don’t have a header chip from IEEE follow this walkthrough. Otherwise you should be able to plug-in the header so VCC and GND line up (on the left hand side of the chip shown above). Once you have connected the programmer, you will need to make sure that the VM recognizes it. In the lower right corner of the VM there is an image of a USB. Clicking that will bring up a menu with the USB options (typically mouse and keyboard and the programmer if it is connected). Click on the programmer to make sure that the it is connected to the VM. 

Note: If you accidentally click on the mouse or keyboard it might actually disconnect it so you can’t use it. If this happens just unplug and plug back in the peripheral to the computer and it should work again. 
A first program on the microcontroller chip
This program, from an earlier lab, just sets PB3..PB0 to 1. We'll connect an LED to PB0, so the program should turn on PB0's LED. 
Prepare the board for the upcoming program by adding an LED to PB0. 
Remove power from the board (note that the AVRISP external LED turns red). It's not necessary to unplug the AVRISP. 
Add a 330Ω resistor and LED in series between PB0 and ground as shown. Orient the LED properly, with the negative (short) leg plugged into ground. (The resistor limits the current flow, extending the LEDs life and keeping the microcontroller cooler). 

In Atmel Studio, create a new project named "lab_chip" for the ATmega1284 and write the following program (from an earlier lab): 
```c++
#include <avr/io.h>

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	while(1)
	{
		PORTB = 0x0F; // Writes port B's 8 pins with 00001111
	}
}
```

Select "Build -> Solution". 
(Previously, you would next select "Debug -> Continue" to simulate the program. But this time, we're going to download the program onto the ATmega1284 chip, a process called "programming" the chip. In the future, you may want to first run simulation to check or debug your program.). 
Select "Tools -> AVR Programming". Make sure the USB cable is connected. 

NOTE: If this is your first time connecting the AVRISP device, AVR Studio may tell you that you  need to update the device's firmware. Follow the on-screen instructions; if you are unsure, refer to our AVR FAQ - Firmware upgrade for the AVRISP. Do not skip this step.

In the AVR Programming popup window, select Tool as "AVRISP mkII", Device as "ATmega1284", and Interface as "ISP". Click "Apply". 



To check if the AVRISP is connecting properly to the chip, first add power back to the board. Then click Device ID "Read" and Target Voltage "Read" buttons -- the fields should fill in with values. If instead an error appears:
Make sure you powered the board. The chip must have power to communicate with the AVRISP. 
If you still have an error, a likely culprit is incorrect wiring from the AVRISP header to the chip. Double check the wiring carefully. 
Click on "Memories". To the far right of "Flash" (referring to the chip's flash memory, where the program will be stored), click on the "..." to open a file. Find the lab_chip.hex file for the project you built above. 

Click "Program". The AVRISP external LED should blink orange briefly, and then turn green again. (If not, make sure you still have power to the board). AND, your PB0 LED should illuminate. You can add more LEDs to PB1, PB2, and PB3 and those LEDs should illuminate. An LED connected to PB4 should not illuminate. 

A program using a button for input
This program, from an earlier lab, reads an input button and sets two LEDs to 01 (if not pressed) or 10 (if pressed). 
Prepare the board for the upcoming program. Be sure power is removed. Add an LED to PB1, and add a button connecting to ground on one end and to PA0 on the other. When pressed, the button forms a connection; else there is no connection.


Note: PA0 will be programmed in pull-up mode, meaning that when the pin has no input the program will read it as 1, and when the pin has a 0 input (ground) the program will read it as 0. Note how the above button provides PA0 with either no input (when not pressed, so read as 1) or with 0 (when pressed, so read as 0). Therefore, pressing the button causes PA0 to be read as 0, and releasing as 1. 

Create a new project named "lab_button" with the following program:

```c++
#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure PORTB as outputs, initialize to 0s
	unsigned char led = 0x00;	
unsigned char button = 0x00;
	while(1)
	{
		// if PA0 is 1, set PB1PB0=01, else =10
		// 1) Read inputs
		button = ~PINA & 0x01; // button is connected to A0
		// 2) Perform Computation
		if (button) { // True if button is pressed
			led = (led & 0xFC) | 0x01; // Sets B to bbbbbb01
					// (clear rightmost 2 bits, then set to 01)
		}
		else {
			led = (led & 0xFC) | 0x02; // Sets B to bbbbbb10
					// (clear rightmost 2 bits, then set to 10)
		}		
// 3) Write output 
PORTB = led;		
	}
}
```

Build the project and program the chip. (Don't forget to power the board). 
PB0's LED should be on and PB1's LED off. Press the button, and note that PB0's LED turns off and PB1's LED turns on. 

Note that PORTA is initialized to 0xFF -- this is essential for pull-up mode. Failing to initialize to 0xFF may result in strange errors from the port's read values being inconsistent. 


Using PORTC -- Disabling JTAG required
Add 8 LEDs (with resistors) to port C's pins and write a program that sets port C to 0x00 initially and to 0xFF while the PA0 button is pressed. Observe that, incorrectly, not all the LEDs light. The reason is that some pins on the ATmega1284 have multiple possible purposes, as indicated in the parentheses of the chip's pinout diagram and as described in the ATmega1284 datasheet:




"JTAG" is a standard serial interface for advanced chip testing, which we won't be using.  Fuses internal to the chip determine which purpose is active for each port. For PORTC, AVR Studio defaults to JTAG being active. If we wish to use port C, when configuring chip programming we must go to the Fuses section, deselect the JTAGEN box, and press Program -- check that the LEDs on PORTC now behave as desired. 
Also, when programming the microcontroller, there is an option to divide the clock speed (8Mhz) by 8. In later labs, we will be implementing our own timer and this option will interfere with that. So, make sure to uncheck the “CKDIV8” option in the fuses section to prevent future problems.






Pre-lab
Be sure that you have all needed supplies, ready for use. Read over the entire lab so you'll know what to expect. For credit you need to at least complete through section: "Prepare the AVR In-System Programming (ISP) header", so your header should be ready to plug into the board. You should also attempt to run the basic programs from above as well.
Exercises
Write C programs for the following exercises using Atmel Studio for an ATmega1284 following the PES standard technique. These are similar to previous exercises you have run in the simulator. For any behavior response caused by a button press, the response should occur almost immediately upon the press, not waiting for the button release (unless otherwise stated). Be sure to count each button press only once, no matter the duration the button is pressed. 


When completing these exercises, chances are that a few bugs will be encountered along the way. LEDs are a helpful component for debugging hardware.

LEDs can be used to check the input/output of individual pins. For example, If a specific pin is meant to be a certain value in order for your program to proceed, connect an LED to the pin to ensure the pin is the correct value. 

A helpful hint is to always keep an extra LED connected between ground and a long wire. Whenever you wish to check the value of a desired pin, just connect the other end of the long wire to the desired pin and observe the behavior of the LED.



It is also recommended that you review the AVR FAQ to help you with common debugging issues when working with hardware.



(From an earlier lab) A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). A series of LEDs connected to PB5..PB0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2, PB5 lights. If the level is 3 or 4, PB5 and PB4 light. 5-6 lights PB5..PB3. 7-9 lights PB5..PB2. 10-12 lights PB5..PB1. 13-15 lights PB5..PB0. Also, PB6 connects to a "Low fuel" icon, which should light if the level is 4 or less. Use buttons on PA3..PA0 and mimic the fuel-level sensor with presses. 



Video Demonstration: http://youthttp://www.youtube.com/watch?v=_6BjqDXpdVk&feature=youtu.behttp://www.youtube.com/watch?v=_6BjqDXpdVk&feature=youtu.beu.be/_6BjqDXpdVk

(From an earlier lab) Buttons are connected to PA0 and PA1. Output for PORTB is initially 0. Pressing PA0 increments PORTB (stopping at 9). Pressing PA1 decrements PORTB (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTB resets to 0. If a reset occurs, both buttons should be fully released before additional increments or decrements are allowed to happen. Use LEDs (and resistors) on PORTB. Use a state machine (not synchronous) captured in C. 

Note: Make sure that one button press causes only one increment or decrement respectively. Pressing and holding a button should NOT continually increment or decrement the counter.


Video Demonstration: http://youtu.be/a0cX_cjr5t0

(Challenge) Create your own festive lights display with 6 LEDs connected to port PB5..PB0, lighting in some attractive sequence. Pressing the button on PA0 changes the lights to the next configuration in the sequence.  Use a state machine (not synchronous) captured in C. 

Video Demonstration: http://youtu.be/ceOSKTxOP74

All students must submit the group’s .c source files according to instructions in the lab submission guidelines. Post any questions or problems you encounter to the wiki and discussion boards on iLearn.

Don’t forget to commit and push your code to Github before shutting down the VM!
