#include <msp430.h> 
#include "Game.h"
#include "buttons/button.h"
#include "LCD_Driver/LCD_Driver.h"	//used C2C Bentley's library
/*
 * main.c
 */

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

void init_timer();
void init_buttons();
void init_LCD();

int main(void) {
	WDTCTL = (WDTPW | WDTHOLD);

	init_timer();
	init_buttons();
	init_LCD();
	__enable_interrupt();

	unsigned char position = initPlayer();
	printPlayer(position);

	while (1) {

		while (playing) {

//looked at C2C Busho's code to get an idea of how I could handle the buttons
			if (isP1ButtonPressed(BIT1)) {
				direction = RIGHT;
				position = movePlayer(position, RIGHT);	//had to look at C2C Busho's code to realize movePlayer() needed to equal position
				flag = 0;
			}

			if (isP1ButtonPressed(BIT2)) {
				direction = LEFT;
				position = movePlayer(position, LEFT);
				flag = 0;
			}

			if (isP1ButtonPressed(BIT3)) {
				direction = UP;
				position = movePlayer(position, UP);
				flag = 0;
			}

			if (isP1ButtonPressed(BIT4)) {
				direction = DOWN;
				position = movePlayer(position, DOWN);
				flag = 0;
			}

			WINNER = didPlayerWin(position);
			while (WINNER) {
				LCDclear();
				writeString(Winner1);
				setCursorLine2();
				writeString(Winner2);
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

void init_timer() {
WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
TACTL &= ~(MC1 | MC0);      // stop timer
TACTL |= TACLR;             // clear TAR
TACTL |= TASSEL1;   // configure for SMCLK - what's the frequency (roughly)?
TACTL |= ID1 | ID0; // divide clock by 8 - what's the frequency of interrupt?
TACTL &= ~TAIFG;            // clear interrupt flag
TACTL |= MC1;               // set count mode to continuous
TACTL |= TAIE;              // enable interrupt
}

void init_buttons() {
configureP1PinAsButton(BIT1 | BIT2 | BIT3 | BIT4);
}

void init_LCD() {
initSPI();
initLCD();
LCDclear();
}

