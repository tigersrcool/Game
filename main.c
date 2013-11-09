#include <msp430.h> 
#include "Game.h"
#include "buttons/button.h"
#include "LCD_Driver/LCD_Driver.h"	//used C2C Bentley's library
/*
 * main.c
 */

unsigned char position;
char* Loser1 = "Game";
char* Loser2 = "Over";
char* Winner1 = "You";
char* Winner2 = "Win!!!";
char flag = 0;
int direction = 0;
int playing = 1;
int ENDGAME = 0;
char buttons[] = { BIT1, BIT2, BIT3, BIT4 };
char pressedButton = 0;
char WINNER = 0;
int start = 1;

void init_timer();
void init_buttons();
void init_LCD();
void testAndRespondToButtonPush(char buttonToTest);

int main(void) {
	WDTCTL = (WDTPW | WDTHOLD);

	init_timer();
	init_buttons();
	init_LCD();
	__enable_interrupt();

	position = initPlayer();
	printPlayer(position);

	while (1) {
		while (playing) {

//looked at C2C Busho's code to get an idea of how I could handle the buttons
//			if (isP1ButtonPressed(BIT1)) {
//				direction = BIT1;
//				position = movePlayer(position, direction);	//had to look at C2C Busho's code to realize movePlayer() needed to equal position
//				flag = 0;
//			}

//			if (isP1ButtonPressed(BIT2)) {
//				direction = BIT2;
//				position = movePlayer(position, direction);
//				flag = 0;
//			}

//			if (isP1ButtonPressed(BIT3)) {
//				direction = BIT3;
//				position = movePlayer(position, direction);
//				flag = 0;
//			}

//			if (isP1ButtonPressed(BIT4)) {
//				direction = BIT4;
//				position = movePlayer(position, direction);
//				flag = 0;
//			}

//			WINNER = didPlayerWin(position);
			while (WINNER) {
				LCDclear();
				writeString(Winner1);
				setCursorLine2();
				writeString(Winner2);
				waitForP1ButtonRelease(BIT1);
				waitForP1ButtonRelease(BIT4);
				pressedButton = pollP1Buttons(buttons, 4);	//checks reset
				while (pressedButton) {
					LCDclear();
					position = initPlayer();
					printPlayer(position);
					pressedButton = 0;
					WINNER = 0;
				}
			}

			while (ENDGAME) {
				LCDclear();
				writeString(Loser1);
				setCursorLine2();
				writeString(Loser2);
				pressedButton = pollP1Buttons(buttons, 4);	//checks reset
				while (pressedButton) {
					LCDclear();
					position = initPlayer();
					printPlayer(position);
					pressedButton = 0;
					ENDGAME = 0;
				}
			}
		}
	}
	return 0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;            // clear interrupt flag
	flag = flag + 1;
	if (flag == 4) {				//checks 2 sec time limit
		ENDGAME = 1;//Decided to put it in here after looking at C2C Taormina's code
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void) {
	testAndRespondToButtonPush(BIT1);
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT3);
	testAndRespondToButtonPush(BIT4);
}

void testAndRespondToButtonPush(char buttonToTest) {
	if (buttonToTest & P1IFG) {
		if (buttonToTest & P1IES) {
			clearPlayer(position);
			position = movePlayer(position, buttonToTest);
			printPlayer(position);
			WINNER = didPlayerWin(position);
			flag = 0;
			TACTL |= TACLR;
		} else {
			debounce();
		}
		P1IES ^= buttonToTest;
		P1IFG &= ~buttonToTest;
	}
}

void init_timer() {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	TACTL &= ~(MC1 | MC0);      // stop timer
	TACTL |= TACLR;             // clear TAR
	TACTL |= TASSEL1;   		// configure for SMCLK - what's the frequency (roughly)?
	TACTL |= ID1 | ID0; 		// divide clock by 8 - what's the frequency of interrupt?
	TACTL &= ~TAIFG;            // clear interrupt flag
	TACTL |= MC1;               // set count mode to continuous
	TACTL |= TAIE;              // enable interrupt
}

void init_buttons() {
	configureP1PinAsButton(BIT1 | BIT2 | BIT3 | BIT4);
	P1IES |= BIT1|BIT2|BIT3|BIT4;		//Looked at C2C Taormina's code to realize I needed to do these last three lines to make the interrupt work.
	P1IFG &= ~(BIT1|BIT2|BIT3|BIT4);
	P1IE |= BIT1|BIT2|BIT3|BIT4;
}

void init_LCD() {
	initSPI();
	initLCD();
	LCDclear();
}

