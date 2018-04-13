#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);
void adventurerCard(int drawntreasure, struct gameState *state, int handPos, int z, int currentPlayer);
void smithyCard(int currentPlayer, struct gameState *state, int handPos);
void baronCard(int choice1, int currentPlayer, struct gameState *state);
void council_roomCard(int currentPlayer, struct gameState *state, int handPos);
void sea_hagCard(int currentPlayer, struct gameState *state);


#endif
