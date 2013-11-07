#include <msp430.h> 
#include "Game.h"
#include "buttons/button.h"
#include "LCD_Driver/LCD_Driver.h"	//used C2C Bentley's library
/*
 * main.c
 */

char* Loser1 = "Game";
char* Loser2 = "Over";
char position = 0;
char flag = 0;
char direction = 0;


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	TACTL &= ~(MC1 | MC0);        // stop timer
	TACTL |= TACLR;             // clear TAR
	TACTL |= TASSEL1;   // configure for SMCLK - what's the frequency (roughly)?
	TACTL |= ID1 | ID0; // divide clock by 8 - what's the frequency of interrupt?
	TACTL &= ~TAIFG;            // clear interrupt flag
	TACTL |= MC1;               // set count mode to continuous
	TACTL |= TAIE;              // enable interrupt
	__enable_interrupt();       // enable maskable interrupts

	initSPI();
	initLCD();
	LCDclear();
	configureP1PinAsButton(BIT1|BIT2|BIT3|BIT4);

	position = initPlayer();
	printPlayer(position);

	int gameover = 1;

	while (gameover) {
		if (isP1ButtonPressed(BIT1)){
			direction = RIGHT;
			movePlayer(position,RIGHT);
			TAR = 1;
			flag = 0;
		}
		if (isP1ButtonPressed(BIT2)){
			direction = LEFT;
			movePlayer(position,LEFT);
			TAR = 1;
			flag = 0;
		}
		if (isP1ButtonPressed(BIT3)){
			direction = UP;
			movePlayer(position,UP);
			TAR = 1;
			flag = 0;
		}
		if (isP1ButtonPressed(BIT4)){
			direction = DOWN;
			movePlayer(position,DOWN);
			TAR = 1;
			flag = 0;
		}
		if (flag >= 4) {
			LCDclear();
			writeString(Loser1);
			setCursorLine2();
			writeString(Loser2);
			gameover = 0;
		} else
			flag++;
	}
while(1){
}

return 0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {

TACTL &= ~TAIFG;            // clear interrupt flag
flag = 1;
}
