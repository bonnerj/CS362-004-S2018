#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int allPassed=1;
int failedTests=0;

void assertTrue(int a, int b) {
        if (a == b)
                printf("	PASSED TEST: ");
        else
        {
                printf("	FAILED TEST: ");
                allPassed=0;
                failedTests += 1;
        }
}

int main() {
	int numPlayers = 2;
	int randomSeed = 25;
	int score=0;
	int player=0;
	int i;

	int k[10]= {adventurer, council_room, feast, gardens, mine,
                        remodel, smithy, village, baron, great_hall};

    int expected=0;

    
    struct gameState test;

    initializeGame(numPlayers, k, randomSeed, &test);

    //reset hand and deck for testing
	for (i = 0; i < test.handCount[player]; i++)
	{
		test.hand[player][i] = -1;
	}

	test.handCount[player] = 0;

	for (i = 0; i < test.deckCount[player]; i++)
	{
		test.deck[player][i] = -1;
	}

	test.deckCount[player] = 0;

	printf("TESTING WITH EMPTY HAND, DISCARD, AND DECK\n"); 
    score=scoreFor(player, &test);
    assert(expected == score);
    printf("Expected return value=%d  Actual value=%d\n", expected, score);

    return 0;
}
