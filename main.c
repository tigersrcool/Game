#include <msp430.h> 
#include "Game.h"
#include "buttons/button.h"
#include "LCD_Driver/LCD_Driver.h"	//used C2C Bentley's library
/*
 * main.c
 */

char* Loser1 = "Game";
char* Loser2 = "Over";
char flag = 0;
int direction = 0;
int playing = 1;
int ENDGAME = 0;
char buttons[] = {BIT1, BIT2, BIT3, BIT4};
char pressedButton = 0;

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



			if (isP1ButtonPressed(BIT1)) {
				direction = RIGHT;
				position = movePlayer(position, RIGHT);
				flag = 0;
			}

			if (isP1ButtonPressed(BIT2)) {
				direction = LEFT;
				movePlayer(position, LEFT);
				flag = 0;
			}

			if (isP1ButtonPressed(BIT3)) {
				direction = UP;
				movePlayer(position, UP);
				flag = 0;
			}

			if (isP1ButtonPressed(BIT4)) {
				direction = DOWN;
				movePlayer(position, DOWN);
				flag = 0;
			}
			while (ENDGAME){
				LCDclear();
				writeString(Loser1);
				setCursorLine2();
				writeString(Loser2);
		        pressedButton = pollP1Buttons(buttons, 4);	//checks reset
		        while (pressedButton){
		        	LCDclear();
		        	position = initPlayer();
		        	printPlayer(position);
		        	pressedButton = 0;
		        	ENDGAME = 0;
		        }
			}

		}
		/*
		 * while (game is on)
		 * {
		 *                 check if button is pushed (you don't want to block here, so don't poll!)
		 *                 if button is pushed:
		 *                         clear current player marker
		 *                         update player position based on direction
		 *                         print new player
		 *                         clear two second timer
		 *                         wait for button release (you can poll here)
		 * }
		 *
		 * print game over or you won, depending on game result
		 *
		 * wait for button press to begin new game (you can poll here)
		 * wait for release before starting again
		 */
	}
	return 0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;            // clear interrupt flag
	flag = flag + 1;
	if (flag==4){				//checks 2 sec time limit
		ENDGAME = 1;			//Decided to put it in here after looking at C2C Taormina's code
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
}

void init_LCD() {
	initSPI();
	initLCD();
	LCDclear();
}

