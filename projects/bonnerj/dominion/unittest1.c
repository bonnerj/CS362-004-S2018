
/*
	Description:  This unit test tests the  fullDeckCount() function in 
	dominion.c

Returns the count of a specific card in a specific player's discard, deck,
and hand

int fullDeckCount(int player, int card, struct gameState *state) {
  int i;
  int count = 0;

  for (i = 0; i < state->deckCount[player]; i++)
    {
      if (state->deck[player][i] == card) count++;
    }

  for (i = 0; i < state->handCount[player]; i++)
    {
      if (state->hand[player][i] == card) count++;
    }

  for (i = 0; i < state->discardCount[player]; i++)
    {
      if (state->discard[player][i] == card) count++;
    }

  return count;
}

*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void assertTrue(int a, int b) {
	if (a == b)
		printf("PASSED TEST: ");
	else
		printf("FAILED TEST: ");
}

int main() {
	int numPlayers = 2;
	int randomSeed = 20;
	int player = 0;
	int card = 0;
	int count;


	int k[10] = {adventurer, council_room, feast, gardens, mine,
	       		remodel, smithy, village, baron, great_hall};
	

	struct gameState test;

	initializeGame(numPlayers, k, randomSeed, &test);


	printf("*************TESTING fullDeckCount*************");

	printf("First test:  Testing Returned Value With No Cards in Deck\n");
	//all decks are full of same card

	count=fullDeckCount(player, card, &test);
	//assertTrue(count, 0);
	//printf("Expected return value=0    Actual value=%d\n", count);

	//printf("Second test:  Testing value with Cards in Deck");

	printf("Expected return value=??   Actual value=%d\n", count);

	
	printf("Third test:  Testing value with Cards in Hand");
	printf("Expected return value=   Actual value=%d\n", count);


	printf("Fourth test:  Testing value with Cards in Discard");
	printf("Expected return value=   Actual value=%d\n", count);

    


}

