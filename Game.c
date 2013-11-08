/*
 * Game.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C15Kirsten.Larson
 */

#include <msp430.h>
#include "Game.h"
#include "buttons/button.h"
#include "LCD_Driver/LCD_Driver.h"

unsigned char initPlayer(){
	return 0x80;
}

void printPlayer(unsigned char player){
	writeCommandByte(player);
	writeDataByte('*');
}

void clearPlayer(unsigned char player){
	writeCommandByte(player);
	writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction){
	switch (direction) {
		case RIGHT:
			clearPlayer(player);
			player = player + 1;
			if (player==0x88){
				player = 0xC0;
			}
			printPlayer(player);
			waitForP1ButtonRelease(BIT1);
			break;

		case LEFT:
			player = player - 1;
			break;
	}
	return player;
}char

didPlayerWin(unsigned char player){
	return player == 0xC7;
}


