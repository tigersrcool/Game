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

#define ROW_MASK 0x40

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
		case BIT1:
			clearPlayer(player);
			player = player + 1;
			if (player==0x88|player == 0xC8){
				player = player - 1;
			}
			printPlayer(player);
			waitForP1ButtonRelease(BIT1);
			break;

		case BIT2:
			clearPlayer(player);
			player = player - 1;
			if (player == 0x7F|player == 0xBF){
				player = player + 1;
			}
			printPlayer(player);
			waitForP1ButtonRelease(BIT2);
			break;

		case BIT3:
			clearPlayer(player);
			player = player - 0x40;
			if (player==0x40|player==0x41|player==0x42|player==0x43|player==0x44|player==0x45|player==0x46|player==0x47){
				player = player + 0x40;
			}
			printPlayer(player);
			waitForP1ButtonRelease(BIT3);
			break;

		case BIT4:
			clearPlayer(player);
			player |= ROW_MASK;
			printPlayer(player);
			waitForP1ButtonRelease(BIT4);
			break;
	}
	return player;
}char

didPlayerWin(unsigned char player){
	return player == 0xC7;
}


