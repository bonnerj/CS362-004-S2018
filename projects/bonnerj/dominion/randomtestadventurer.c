/******************************************************************************
 * Method Under Test: adventurer
 * Input Parameters: int currentPlayer, struct gameState *state, int handPos
 * Parameters affected:  deck, hand, played cards, coins
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
int totalDeckTest = 0;
int coinsTest = 0;
int returnTest = 0;
int playedTest = 0;

int  assertTrue(int a, int b, int test) {
    if (a != b)
    {
        allPassed = 0;
        if (test == 1) 
            handTest++;
        else if (test == 2)
            totalDeckTest++;
        else if (test == 3)
	    coinsTest++;
	else if (test == 4)
	    returnTest++;
	else if (test == 5)
	    playedTest++;
        return 0;
    }
    else 
	return 1;
}

void printFailedState(int p, int numTreasures, struct gameState *origTest) {

	printf("GAMESTATE THAT FAILED: \n");
	printf("deckCount:%d, handCount:%d, discardCount:%d, playedCount:%d, deckTreasures:%d\n", 
               origTest->deckCount[p], origTest->handCount[p], 
               origTest->discardCount[p], origTest->playedCardCount, numTreasures);
}

void randomTest(int p, int handPos, int t, struct gameState *origTest) {
    struct gameState postTest;
    memcpy (&postTest, origTest, sizeof(struct gameState));

    int bonus=0; //not used in adventurer
    int before, after, expected;

    int retVal = cardEffect(adventurer, -1, -1, -1, &postTest, handPos, &bonus);

    //testing hand count (+treasures drawn -1 adventurer played)
    before = origTest->handCount[p];
    if (t>1)
	expected = before + 1;
    else if (t==1)
        expected = before;
    else
	expected = before - 1; 
    after = postTest.handCount[p];
    if(!assertTrue(expected, after, 1)){
//	printf("HAND COUNT ");
//      printFailedState(p, t, origTest);
//      printf("BEFORE: %d, EXPECTED:  %d, AFTER: %d\n", before, expected, after);
    }

    //testing played cards (should be exactly 1 - adventurer)
    before = origTest->playedCardCount;
    after = postTest.playedCardCount;
    if(!assertTrue(before+1, after, 5)){
//	printf("TOTAL CARD COUNT ");
//      printFailedState(p, t, origTest);
//      printf("BEFORE: %d, AFTER: %d\n", before, after);
    }

    //testing total cards in deck after cards drawn 
    before = origTest->deckCount[p] + origTest->discardCount[p] + origTest->handCount[p]
	     + origTest->playedCardCount;
    after = postTest.deckCount[p] + postTest.discardCount[p] + postTest.handCount[p] + 
	    + postTest.playedCardCount;
    if(!assertTrue(before, after, 2)){
//	printf("TOTAL CARD COUNT ");
//      printFailedState(p, t, origTest);
//      printf("BEFORE: %d, AFTER: %d\n", before, after);
    }
    
    //testing correct number of coins added for player
    before = origTest->coins;
    if (t>1)
	expected = before + 2;
    else if (t==1)
        expected = before + 1;
    else
	expected = before; 
    after = postTest.coins;
    if(!assertTrue(expected, after, 3)){ 
//	printf("COIN COUNT ");
//      printFailedState(p, t, origTest);
//      printf("BEFORE: %d, AFTER: %d\n", before, after);
    }

    //testing return value = 0
    if(!assertTrue(retVal, 0, 4)) {
    //    printFailedState(p, t, origTest);
    }

}

int main () {
    
    int i, j, k, player, handPos, rand, index;
    struct gameState G;
    int treasures;
    int c[10]= {adventurer, council_room, feast, gardens, mine,
                remodel, smithy, village, baron, great_hall};

    SelectStream(2);
    PutSeed(-3);
    printf("\n********RANDOM TESTING ADVENTURER********\n");   
    for( i = 0; i < 250; i++) {
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
        G.hand[player][handPos] = adventurer;
        G.coins = floor(Random() * 2000);

        treasures = 0;
	for (k = 0; k<G.deckCount[player]; k++){
	    if (k != handPos) {
                rand = floor(Random() * 10);
	        if (rand < 5) {
	            G.deck[player][k] = copper;
		    treasures++;
               }
                else {
		    index = floor(Random() * 9);
                    G.deck[player][k] = c[index];
		}
            }
        } 

        randomTest(player, handPos, treasures, &G);
    }
     
    if (allPassed) { 
        printf("    ***All tests have passed.***\n");
    }
    else {
        printf("    ***Failed Tests***\n");
        printf("        Total Deck Count Test: %d failed\n", totalDeckTest);
        printf("        Hand Count Test: %d failed\n", handTest);
        printf("        Played Count Test: %d failed\n", playedTest);
        printf("        Coin Test: %d failed\n", coinsTest);
        printf("        Return Test: %d failed\n", returnTest);
    }
    return 0;
}

