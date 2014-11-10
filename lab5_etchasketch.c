//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab5.c
// Date:	Fall 2014
// Purp:	Demo the decoding of an IR packet
//-----------------------------------------------------------------
#include <msp430g2553.h>
#include "lab5.h"

int8	newIrPacket = FALSE;
int32	packetBits;
int8	packetIndex = 0;

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void main(void) {

	initMSP430();				// Setup MSP to process IR and buttons
	initNokia();
	clearDisplay();
	x=4;		y=4;
	color= BLACK;
	drawBlock(y,x, color);

	while(1) {

		if (packetIndex >= 31) {	//Got a new packet!
			packetIndex = 0;

			//Check if color is changed by middle button
			if(packetBits == BIG_MIDDLE){
				color = (color+1)%2;
			}

			//Check if up button is pressed
			if (packetBits == BIG_UP) {
				if (y>=1) y=y-1;
				drawBlock(y,x,color);
			}

			//Check if down button is pressed
			else if (packetBits == BIG_DOWN) {
				if (y<=6) y=y+1;
				drawBlock(y,x,color);
			}

			//Check if left button is pressed
			else if (packetBits == BIG_LEFT) {
				if (x>=1) x=x-1;
				drawBlock(y,x,color);
			}

			//Check if right button is pressed
			else if (RIGHT_BUTTON == 0) {
				if (x<=10) x=x+1;
				drawBlock(y,x,color);
			}

		}	// End got a new packet
	} // End While Loop


} // end main

// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LEDs
	P1OUT &= ~(BIT0 | BIT6);			// And turn the LEDs off

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
	case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
		pulseDuration = TAR;
		packetIndex++;
		if(minLogic0Pulse < pulseDuration && pulseDuration < maxLogic0Pulse){	//If the bit is 0
			packetBits = packetBits << 1;
			packetBits += 0;
		}
		else if(minLogic1Pulse < pulseDuration && pulseDuration < maxLogic1Pulse){	//If the bit is 1
			packetBits = packetBits << 1;
			packetBits += 1;
		}
		// Otherwise Ignore the bit

		LOW_2_HIGH; 				// Setup pin interrupr on positive edge
		break;

	case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
		TAR = 0x0000;						// time measurements are based at time 0
		HIGH_2_LOW; 						// Setup pin interrupr on positive edge
		break;
	} // end switch

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {

	TACTL &= ~TAIFG;
}


//-------------------------------------------------------------------------------
//	Name:		Dr. Chris Coulston
//	Term:		Fall 2014
//	MCU:		MSP430G2553
//	Lecture:	22
//	Date:		16 October 2014
//	Note:		Demonstration of how to combine C and assembly lanugage.
//-------------------------------------------------------------------------------
#include <msp430g2553.h>

extern void init();
extern void initNokia();
extern void clearDisplay();
extern void drawBlock(unsigned char row, unsigned char col, unsigned char color);

#define		TRUE			1
#define		FALSE			0
#define		BLACK			1
#define		WHITE			0
#define		UP_BUTTON		(P2IN & BIT5)
#define		DOWN_BUTTON		(P2IN & BIT4)
#define		AUX_BUTTON		(P2IN & BIT3)
#define		LEFT_BUTTON		(P2IN & BIT2)
#define		RIGHT_BUTTON	(P2IN & BIT1)


unsigned char	x, y, button_press, color;

void main() {


	// === Initialize system ================================================
	IFG1=0; /* clear interrupt flag1 */
	WDTCTL=WDTPW+WDTHOLD; /* stop WD */
	button_press = FALSE;


	init();
	initNokia();
	clearDisplay();
	x=4;		y=4;
	color= BLACK;
	drawBlock(y,x, color);

	while(1) {

		//Check if color is changed by aux button
		if (AUX_BUTTON == 0) {
			while(AUX_BUTTON == 0);
			color = (color+1)%2;
		}

		//Clear screen by pressing up and aux
		if(UP_BUTTON == 0 && AUX_BUTTON == 0){
			while(UP_BUTTON == 0 && AUX_BUTTON == 0);
			clearDisplay();
		}

		if (UP_BUTTON == 0) {
			while(UP_BUTTON == 0);
			if (y>=1) y=y-1;
			button_press = TRUE;
		} else if (DOWN_BUTTON == 0) {
			while(DOWN_BUTTON == 0);
			if (y<=6) y=y+1;
			button_press = TRUE;
		} else if (LEFT_BUTTON == 0) {
			while(LEFT_BUTTON == 0);
			if (x>=1) x=x-1;
			button_press = TRUE;
		} else if (RIGHT_BUTTON == 0) {
			while(RIGHT_BUTTON == 0);
			if (x<=10) x=x+1;
			button_press = TRUE;
		}


		if (button_press) {
			button_press = FALSE;
			drawBlock(y,x,color);
		}
	}
}
