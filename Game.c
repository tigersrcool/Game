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
			player = player + 1;
			if (player==0x88|player == 0xC8){
				player = player - 1;
			}
			break;

		case BIT2:
			player = player - 1;
			if (player == 0x7F|player == 0xBF){
				player = player + 1;
			}
			break;

		case BIT3:
			player &= ~ROW_MASK;
			break;

		case BIT4:
			player |= ROW_MASK;
			break;
	}
	return player;
}char

didPlayerWin(unsigned char player){
	return player == 0xC7;
}


