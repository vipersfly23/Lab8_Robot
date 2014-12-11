#include <msp430.h>
#include "start5.h"

int16 rightValue;
int16 leftValue;
int16 centerValue;

int8 leftFlagFar = 0;
int8 leftFlagNear = 0;
int8 rightFlag = 0;
int8 centerFlag = 0;
int8 leftFlag = 0;

void main(void) {

	initMSP430();				// Setup MSP to process IR and buttons

	while (1) {

		if (centerFlag && leftFlag) {
			motorControl(NORMAL, REVERSE);
		} else if (leftFlagNear) {
			motorControl(NORMAL, SLOW);
		} else if (leftFlagFar) {
			motorControl(SLOW, NORMAL);

		}
	}
// end loop

} // end main

void initMSP430() {

	IFG1 = 0; 					// clear interrupt flag1
	WDTCTL = WDTPW + WDTHOLD; 		// stop WD
	BCSCTL1 = CALBC1_8MHZ;										// 8MHz clock
	DCOCTL = CALDCO_8MHZ;

	/*
	 * This sets the pins to run the motors
	 */
	P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
	P2SEL |= BIT2;	// P2.2 is associated with TA1CCTL1

	P2DIR |= BIT0;
	P2OUT |= BIT0;

	P2DIR |= BIT1;
	P2OUT &= ~BIT1;

	P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
	P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	P2DIR |= BIT5;
	P2OUT |= BIT5;

	P2DIR |= BIT3;
	P2OUT &= ~BIT3;

	/*
	 * this sets the timer
	 */
	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
	TA1CCR0 = 0x1388;						// set signal period

	TA1CCR1 = NORMALSPEED;						// LEFT MOTER
	TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

	TA1CCR2 = NORMALSPEED;					// RIGHT MOTOR
	TA1CCTL2 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

	P1DIR = BIT0 | BIT6;							// Set the red LED as output

	ADC10CTL0 = 0;
	ADC10CTL1 = ADC10DIV_3;				// Channel 4, ADC10CLK/4
	ADC10AE0 = BIT3 | BIT4 | BIT5;		 			// Make P1.3,P1.4, P1.5 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;// Vcc & Vss as reference

	TA0CCR0 = 0xFFFF;						// create a 16mS roll-over period
	TACTL &= ~TAIFG;	// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;// Use 1:8 presclar off MCLK and enable interrupts
	_enable_interrupt();

	_enable_interrupt();
}

// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR				// This is from the MSP430G2553.h file
__interrupt void timerOverflow(void) {
	static int8 currentSample = LEFT;

	if (currentSample == LEFT) {	//Starts collecting value from left side.
		selectInch(currentSample);
		rightValue = ADC10MEM;						// stores the right value
		if (rightValue > 550) {
			rightFlag = 1;
		} else {
			rightFlag = 0;
		}

		ADC10CTL0 |= ADC10SC;				// Start a conversion
		currentSample = CENTER;				//samples the center sensor next
	} else if (currentSample == CENTER) {
		selectInch(currentSample);			//selects center sensor
		leftValue = ADC10MEM;				//stores left value

		if (leftValue > 800) {//Checks to see how far the robot is from the wall
			leftFlagNear = 1;				//if it's too close
		} else {
			leftFlagNear = 0;
		}
		if (leftValue < 700) {				//if it's too far
			leftFlagFar = 1;
		} else {
			leftFlagFar = 0;
		}
		if (leftValue > 475) {//checks for turning, if left wall is near, the robot turns right.
			leftFlag = 1;
		} else {
			leftFlag = 0;
		}

		ADC10CTL0 |= ADC10SC;							// Start a conversion
		currentSample = RIGHT;			//selects right sensor
	} else {
		selectInch(currentSample);		//re-configure pins for right sensor
		centerValue = ADC10MEM;			//stores center value

		if (centerValue > 475) {		//checks center sensor
			centerFlag = 1;
		} else {
			centerFlag = 0;
		}

		ADC10CTL0 |= ADC10SC;				 // Start a conversion
		currentSample = LEFT;				//selects left sensor
	}

	TACTL &= ~TAIFG;
}

void motorControl(int8 leftMode, int8 rightMode) {
	if (leftMode == OFF) {
		P2OUT &= ~BIT0;		//	P2DIR &= ~BIT0 ; turn off enable
	} else if (leftMode == REVERSE) {
		P2OUT |= BIT0;		//DIR HIGH, MOD_3
		P2OUT |= BIT1;
		TA1CCTL1 = OUTMOD_3;
	} else {
		P2OUT |= BIT0;		//enable
		P2OUT &= ~BIT1;		//set ground
		TA1CCTL1 = OUTMOD_7;		//move forward
	}

	if (rightMode == OFF) {
		P2OUT &= ~BIT5;		//	P2DIR &= ~BIT0 ; turn off enable
	} else if (rightMode == REVERSE) {
		P2OUT |= BIT5;		//DIR HIGH, MOD3
		P2OUT |= BIT3;
		TA1CCTL2 = OUTMOD_3;
	} else {
		P2OUT |= BIT5;		//DIR LOW, MOD_7
		P2OUT &= ~BIT3;
		TA1CCTL2 = OUTMOD_7;

	}

	/*
	 * configures speed
	 */
	if (leftMode == FAST) {
		TA1CCR1 = FASTSPEED;						// LEFT MOTER
	}
	if (rightMode == FAST) {
		TA1CCR2 = FASTSPEED;					// RIGHT MOTOR
	}

	if (leftMode == NORMAL) {
		TA1CCR1 = NORMALSPEED;						// LEFT MOTER
	}
	if (rightMode == NORMAL) {
		TA1CCR2 = NORMALSPEED;					// RIGHT MOTOR
	}

	if (leftMode == SLOW) {
		TA1CCR1 = SLOWSPEED;
	}
	if (rightMode == SLOW) {
		TA1CCR2 = SLOWSPEED;
	}
}

void selectInch(int8 num) {
	if (num == LEFT) {
		ADC10CTL0 &= ~ENC;
		ADC10CTL1 = INCH_3;
		ADC10CTL0 |= ENC;
	} else if (num == CENTER) {
		ADC10CTL0 &= ~ENC;
		ADC10CTL1 = INCH_4;
		ADC10CTL0 |= ENC;
	} else {
		ADC10CTL0 &= ~ENC;
		ADC10CTL1 = INCH_5;
		ADC10CTL0 |= ENC;

	}
}

