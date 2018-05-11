/******************************************************************************
Method Under Test: smithyCard
Input Parameters: int currentPlayer, struct gameState *state, int handPos
Parameters affected:  deck, hand, played cards
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "interface.h"


int allPassed = 1;
int handTest = 0;
int deckTest = 0;
int discardTest = 0;
int playedTest = 0;
int returnTest = 0;

int  assertTrue(int a, int b, int test) {
    if (a != b)
    {
        allPassed = 0;
	if (test == 1)
		handTest++;
	else if (test == 2)
		deckTest++;
	else if (test == 3) 
		discardTest++;
	else if (test == 4)
		playedTest++;	
	else if (test == 5)
		returnTest++;
        return 0;
    }
    return 1;
}

void printFailedState(int p, struct gameState *origTest) {
        printf("GAMESTATE THAT FAILED: \n");
        printf("deckCount:%d, handCount:%d, discardCount:%d, playedCount: %d\n",
               origTest->deckCount[p], origTest->handCount[p],
               origTest->discardCount[p], origTest->playedCardCount);
}

void randomTest(int p, int handPos, struct gameState *origTest) {
    struct gameState postTest;
    memcpy (&postTest, origTest, sizeof(struct gameState));

    int bonus=0;
    int before, after, expected;

    int retVal = cardEffect(smithy, -1, -1, -1, &postTest, handPos, &bonus);
    //testing 2 cards added to hand (+drawn -1smithy played)
    before = origTest->handCount[p];
    after = postTest.handCount[p];
    if(origTest->deckCount[p] > 2)  //at least 3 cards in deck to draw from
        expected = before + 2;
    else if (origTest->deckCount[p] == 2) //2 cards in deck to draw from
        expected = before + 1;
    else if (origTest->deckCount[p] < 2) //1 or 0 cards in deck
        expected = before;
    assertTrue(expected, after, 1); 

    //testing 3 cards taken from deck (-3 drawn)
    before = origTest->deckCount[p];
    after = postTest.deckCount[p];
    if(origTest->deckCount[p] > 2)  //at least 3 cards in deck to draw from
        expected = before - 3;
    else if (origTest->deckCount[p] == 2) //2 cards in deck to draw from
        expected = before - 2;
    else if (origTest->deckCount[p] == 1) //1 card in deck
        expected = before - 1;
    else //0 cards in deck
        expected = before; 
    assertTrue(expected, after, 2);

    //testing discarded cards not changed (not until end of turn)
    before = origTest->discardCount[p];
    after = postTest.discardCount[p];
    if(!assertTrue(before, after, 3)) {
    //    printf("FAILED DISCARD COUNT\n");
    //    printFailedState(p, origTest);
    }

    //testing 1 card added to played cards
    before = origTest->playedCardCount;
    after = postTest.playedCardCount;
    assertTrue(before+1, after, 4);

    //testing returns 0
    if(!assertTrue(0, retVal, 5)){
    //    printFailedState(p, origTest);
    }
} 


int main () {

    int i, j, player, handPos;
    struct gameState G;
    printf("\n********RANDOM TESTING SMITHY********\n");

    SelectStream(2);
    PutSeed(-3);

    for( i = 0; i<2000; i++) {
        for( j = 0; j < sizeof(struct gameState); j++) {
            ((char*)&G)[i] = floor(Random() * 256);
        }

        player = floor(Random() * MAX_PLAYERS);
        G.deckCount[player] = floor(Random() * MAX_DECK);
        G.discardCount[player] = floor(Random() * MAX_DECK);
        G.handCount[player] = floor(Random() * MAX_HAND);
        G.playedCardCount = floor(Random() * (MAX_DECK-1));
        G.whoseTurn = player;
	handPos = floor(Random() * G.handCount[player]);
	G.hand[player][handPos] = smithy;
	randomTest(player, handPos, &G);
    }
     
    if (allPassed) {
	printf("    ***All tests have passed.***\n");
    }
    else {
        printf("    ***Failed Tests***\n");
	printf("	Deck Count Test: %d failed\n", deckTest);
	printf("	Hand Count Test: %d failed\n", handTest);
	printf("	Discard Count Test: %d failed\n", discardTest);
	printf("	Played Card Test: %d failed\n", playedTest);
	printf("	Return 0 Test: %d failed\n", returnTest);
    }	
    return 0;
}
