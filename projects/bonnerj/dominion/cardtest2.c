/******************************************************************************
*	From Wikipedia: http://wiki.dominionstrategy.com/index.php/Great_Hall
*	Card Description:  +1 Card, +1 Action
*	From Code:
*		case great_hall:
	      //+1 Card
	      drawCard(currentPlayer, state);
				
	      //+1 Actions
	      state->numActions++;
				
	      //discard card from hand
	      discardCard(handPos, currentPlayer, state, 0);
	      return 0;
*******************************************************************************/

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

void clearDeck(struct gameState *state, int player)
{
	int i;
 	//reset hand and deck for testing
	for (i = 0; i < state->handCount[player]; i++)
	{
		state->hand[player][i] = -1;
	}

	state->handCount[player] = 0;

	for (i = 0; i < state->deckCount[player]; i++)
	{
		state->deck[player][i] = -1;
	}

	state->deckCount[player] = 0;

	for (i = 0; i < state->discardCount[player]; i++)
	{
		state->discard[player][i] = -1;
	}

	state->discardCount[player] = 0;

	//reset coin count
	state->coins = 0;
}

int main() {

	//for initializing game
	int numPlayers = 2;
	int randomSeed = 25;
	int player1=0;
	int k[10]= {adventurer, council_room, feast, gardens, mine,
                remodel, smithy, village, baron, great_hall};
    struct gameState origTest, postTest;

    //for cardEffect()
    //choice1, choice2, choice3, and bonus are NOT used in great_hall
    int choice1=-1;
	int choice2=-1;
	int choice3=-1;
	int bonus=0;

	int handPos=0;

    int expected=0;
    int before=0;
    int after=0;
    
    initializeGame(numPlayers, k, randomSeed, &origTest);

    clearDeck(&origTest, player1);

    origTest.hand[player1][0] = great_hall;
	origTest.handCount[player1]++;


	origTest.deck[player1][0] = adventurer;
	origTest.deckCount[player1]++;


	postTest = origTest;

	cardEffect(great_hall, choice1, choice2, choice3, &postTest, handPos, &bonus);

	printf("*************TESTING Great Hall *************\n\n"); 

    printf("	Test #1: Testing Correct Number of Cards in Player 1's Hand\n");
    //should be 1
    before=origTest.handCount[player1];
	printf("	Cards in hand before playing Great Hall: %d\n", before);
	after=postTest.handCount[player1];
	//should be 1; 1 to start +1 drawn and -1 adventurer played
	expected=before + 1 - 1;
	assertTrue(after, expected);
	printf("	Cards in hand after playing Great Hall:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards in Player 1's Played Cards\n");
    //should be 0
    before=origTest.playedCardCount;
	printf("	Cards in discard before playing Great Hall: %d\n", before);
	after=postTest.playedCardCount;
	//should be 1 - great_hall played
	expected=before + 1;
	assertTrue(after, expected);
	printf("	Cards in discard after playing Great Hall:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards in Player 1's Deck\n");
    //should be 1
    before=origTest.deckCount[player1];
	printf("	Cards in deck before playing Great Hall: %d\n", before);
	after=postTest.deckCount[player1];
	//should be 0 - the only card in the deck was drawn
	expected=before - 1;
	assertTrue(after, expected);
	printf("	Cards in deck after playing Great Hall:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #4: Testing Correct Number of Actions added\n");
    //should be + 1
    before=origTest.numActions;
	printf("	Number of actions before playing Great Hall: %d\n", before);
	after=postTest.numActions;
	expected=origTest.numActions+1;
	assertTrue(after, expected);
	printf("	Number of actions after playing Great Hall:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);


	if(allPassed)
                printf("ALL TESTS PASSED!\n");
    	else
                printf("%d OF THE TESTS HAVE FAILED\n", failedTests);

    return 0;
}

