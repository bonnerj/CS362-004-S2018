/******************************************************************************
 * Method Under Test: greatHall
 * Input Parameters: int currentPlayer, struct gameState *state, int handPos, 
 * int numActions
 * Parameters affected:  deck, hand, played cards, numActions
 * *******************************************************************************/

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
int actionsTest = 0;
int returnTest = 0;

int  assertTrue(int a, int b, int test) {
    if (a != b)
    {
        allPassed = 0;
        if (test == 1) {
            handTest++;
        }
        else if (test == 2) {
            deckTest++;
        }
        else if (test == 3) {
            discardTest++;
        }
        else if (test == 4)
    	    playedTest++;
	else if (test == 5)
	    actionsTest++;
	else if (test == 6)
	    returnTest++;
        return 0;
    }
    else 
	return 1;
}

void printFailedState(int p, struct gameState *origTest) {

	printf("GAMESTATE THAT FAILED: \n");
	printf("deckCount:%d, handCount:%d, discardCount:%d, playedCount: %d, numactions:%d \n", 
               origTest->deckCount[p], origTest->handCount[p], 
               origTest->discardCount[p], origTest->playedCardCount, origTest->numActions);
}

void randomTest(int p, int handPos, struct gameState *origTest) {
    struct gameState postTest;
    memcpy (&postTest, origTest, sizeof(struct gameState));

    int bonus=0; //not used in great_hall
    int before, after, expected;

    int retVal = cardEffect(great_hall, -1, -1, -1, &postTest, handPos, &bonus);
    //testing no change in hand count (+1 drawn -1 great hall played)
    before = origTest->handCount[p];
    after = postTest.handCount[p];
    if(!assertTrue(before, after, 1)){
    //    printFailedState(p, origTest);
    }

    //testing 1 cards taken from deck (-1 drawn)
    before = origTest->deckCount[p];
    after = postTest.deckCount[p];
    if (before < 1) //0 cards to draw from (can't have negative cards)
        expected = before;
    else 
         expected = before - 1;
    if(!assertTrue(expected, after, 2)){
        printFailedState(p, origTest);
    }
    
    //testing discarded cards not changed (not until end of turn)
    before = origTest->discardCount[p];
    after = postTest.discardCount[p];
    if(!assertTrue(before, after, 3)){
        printFailedState(p, origTest);
    }

    //testing 1 card added to played cards
    before = origTest->playedCardCount;
    after = postTest.playedCardCount;
    if(!assertTrue(before+1, after, 4)){ 
    //    printFailedState(p, origTest);
    }

    //testing 1 action added for player
    before = origTest->numActions;
    after = postTest.numActions;
    if(!assertTrue(before+1, after, 5)){ 
    //    printFailedState(p, origTest);
    }

    //testing returns 0
    if(!assertTrue(0, retVal, 6)){
    //    printFailedState(p, origTest);
    }
}
    
int main () {
    
    int i, j, player, handPos, numAct;
    struct gameState G;
    
    SelectStream(2);
    PutSeed(-3);
   
    printf("\n********RANDOM TESTING GREAT HALL********\n");
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
        G.hand[player][handPos] = great_hall;
        numAct = floor(Random() * 1000);
        G.numActions = numAct;
        randomTest(player, handPos, &G);
    }
     
    if (allPassed) { 
        printf("    ***All tests have passed.***\n");
    }
    else {
        printf("    ***Failed Tests***\n");
        printf("        Deck Count Test: %d failed\n", deckTest);
        printf("        Hand Count Test: %d failed\n", handTest);
        printf("        Discard Count Test: %d failed\n", discardTest);
        printf("        Played Card Test: %d failed\n", playedTest);
        printf("        Action Added Test: %d failed\n", actionsTest);
	printf("	Returns 0 Test: %d failed\n", returnTest);
    }
    return 0;
}

