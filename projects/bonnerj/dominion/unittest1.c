/*******************************************************************************
Description:  This unit test tests the  fullDeckCount() function in 
dominion.c

fullDeckCount() Function: Returns the count of a specific card in a 
specific player's discard, deck, and hand

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
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "interface.h"


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


	printf("*************TESTING fullDeckCount*************\n\n");

	printf("Current Full Deck: \n");
	printDeck(player, &test);
	printHand(player, &test);
	printDiscard(player, &test);

	printf("First test:  Testing Full Starting Deck With Zero Value\n");
	count=fullDeckCount(player, card, &test);
	assertTrue(count, 0);
	printf("Expected return value=0    Actual value=%d\n", count);

	printf("Second test:  Testing Full Starting Deck for Copper\n");
	count=fullDeckCount(player, copper, &test);
	assertTrue(count, 7);
	printf("Expected return value=7  Actual value=%d\n", count);

	printf("Third test:  Testing Full Starting Deck for Estate Card\n");
	count=fullDeckCount(player, estate, &test);
	assertTrue(count, 3);
	printf("Expected return value=3   Actual value=%d\n", count);

	printf("Fourth test:  Testing Full Deck for Card Not Present\n");
	count=fullDeckCount(player, adventurer, &test);
	assertTrue(count, 0);
	printf("Expected return value=0   Actual value=%d\n", count);

	printf("Fifth test:  Testing for same Card After Adding it to Deck\n");
	test.deck[ player ][ test.deckCount[player] ] = adventurer;
        test.deckCount[player]++;
	count=fullDeckCount(player, adventurer, &test);
	printf("New Deck: \n");
	printDeck(player, &test);
	assertTrue(count, 1);
	printf("Expected return value=1   Actual value=%d\n", count);

	printf("Sixth test: Testing after Card Added to Discard\n");
	test.discard[ player][test.discardCount[player] ] = smithy;
	test.discardCount[player]++;
	count=fullDeckCount(player, adventurer, &test);
	printf("New Discard: \n");
	printDiscard(player, &test);
	assertTrue(count, 1);
	printf("Expected return value=1   Actual value=%d\n", count);

    	return 0;
}

