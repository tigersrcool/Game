#include <msp430.h> 
#include "buttons/button.h"
#include "LCD_Driver/LCD_Driver.h"

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	initSPI();
	initLCD();
	LCDclear();
	
	return 0;
}
