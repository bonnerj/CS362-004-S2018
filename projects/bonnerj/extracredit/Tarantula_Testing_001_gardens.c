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
                printf("	PASSED TEST  \n");
        else
        {
                printf("	FAILED TEST  \n");
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

    //reset hand and deck for testing - discard starts empty
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

    //adds garden to hand
	test.discard[ player ][ test.discardCount[player] ] = gardens;
    test.discardCount[player]++;

    //adds 10 more cards to hand so that gardens will add to score + 1
    for (i = 1; i < 10; i++) {
        test.discard[player][i] = adventurer;
        test.disccardCount[player]++;
    }

    //printf("Deck count = %d", test.deckCount[player]);
    score=scoreFor(player, &test);
    expected += 1;
    assertTrue(expected, score);

    return 0;
}
