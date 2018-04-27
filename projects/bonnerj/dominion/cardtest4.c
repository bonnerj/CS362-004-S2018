/******************************************************************************
*	From Wikipedia: http://wiki.dominionstrategy.com/index.php/Steward
*	Card Description:  "Choose one: +2 Cards or +2 Coins; or trash 2 cards
						from your hand."
*	Clarifications:	"If you pick trash 2 cards and only have one card in hand,
					 you trash that card"
					"If cards you trash have when-trashed effects, you choose 
					 which order to activate those effects"
*	From Code:
*		case steward:
		    if (choice1 == 1)
			{
			  //+2 cards
			  drawCard(currentPlayer, state);
			  drawCard(currentPlayer, state);
			}
		    else if (choice1 == 2)
			{
			  //+2 coins
			  state->coins = state->coins + 2;
			}
		    else
			{
			  //trash 2 cards in hand
			  discardCard(choice2, currentPlayer, state, 1);
			  discardCard(choice3, currentPlayer, state, 1);
			}
					
		    //discard card from hand
		    discardCard(handPos, currentPlayer, state, 0);
		    return 0;
******************************************************************************/

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

 //reset hand and deck for testing
void clearDeck(struct gameState *state, int player)
{
	int i;
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
	//counter
	int i;
	//for initializing game
	int numPlayers = 2;
	int randomSeed = 25;
	int player1=0;
	int k[10]= {steward, council_room, feast, gardens, mine,
                remodel, smithy, village, baron, great_hall};
    struct gameState origTest, postTest;

    //for cardEffect()
    //choice1, choice2, choice3, and bonus are NOT used in smithy
    int choice1=-1;
	int choice2=-1;
	int choice3=-1;
	int bonus=0;

	//putting the tested card in position 0 in hand
	int handPos=0;

	//variables to hold comparison values
    int expected=0;
    int before=0;
    int after=0;
    
    initializeGame(numPlayers, k, randomSeed, &origTest);

    //start with fresh slate in hand/deck/discard
    clearDeck(&origTest, player1);

    //add steward to hand
    origTest.hand[player1][0] = steward;
	origTest.handCount[player1]++;

	//add two cards to hand
	for (i=1; i<3; i++)
	{
		origTest.hand[player1][i] = adventurer;
		origTest.handCount[player1]++;
	}

	//add two cards to deck
	for (i=0; i<2; i++)
	{
		origTest.deck[player1][1] = adventurer;
		origTest.deckCount[player1]++;
	}
	
	//copies starting point for comparison
	postTest = origTest;


	printf("*************TESTING Steward Card *************\n\n"); 

	printf("Round One:  CHOICE TO DRAW 2 CARDS\n");
	choice1=1; //choose to draw 2 cards

	cardEffect(steward, choice1, choice2, choice3, &postTest, handPos, &bonus);

	printf("	Test #1: Testing Correct Number of Cards in Player 1's Hand\n");
    //should be 3 to start
    before=origTest.handCount[player1];
	printf("	Cards in hand before playing Steward: %d\n", before);
	after=postTest.handCount[player1];
	//should be 4; 3 to start +2 drawn and -1 smithy played
	expected=before + 2 - 1;
	assertTrue(after, expected);
	printf("	Cards in hand after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards in Player 1's Played Cards\n");
    //should be 0 to start
    before=origTest.playedCardCount;
	printf("	Cards in played cards before playing Steward: %d\n", before);
	after=postTest.playedCardCount;
	//should be 1 - smithy played
	expected=before + 1;
	assertTrue(after, expected);
	printf("	Cards in played cards after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards in Player 1's Deck\n");
    //should be 3 to start
    before=origTest.deckCount[player1];
	printf("	Cards in deck before playing Steward: %d\n", before);
	after=postTest.deckCount[player1];
	//should be 0 - drew 2 and started with 2 in deck
	expected=before - 2;
	assertTrue(after, expected);
	printf("	Cards in deck after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #4: Testing Coins Not Changed\n");
	before=origTest.coins;
	printf("	Cards in deck before playing Smithy: %d\n", before);
	after=postTest.coins;
	//should be 0 
	expected=before;
	assertTrue(after, expected);
	printf("	Coins after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("Round Two:  CHOICE TO GAIN 2 COINS\n");
	choice1=2; //choose to draw 2 cards

	//copies starting point for comparison
	postTest = origTest;

	cardEffect(steward, choice1, choice2, choice3, &postTest, handPos, &bonus);

	printf("	Test #1: Testing Correct Number of Cards in Player 1's Hand\n");
    //should be 3 to start
    before=origTest.handCount[player1];
	printf("	Cards in hand before playing Steward: %d\n", before);
	after=postTest.handCount[player1];
	//should be 2; 3 to start and -1 smithy played
	expected=before - 1;
	assertTrue(after, expected);
	printf("	Cards in hand after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards in Player 1's Played Cards\n");
    //should be 0 to start
    before=origTest.playedCardCount;
	printf("	Cards in played cards before playing Steward: %d\n", before);
	after=postTest.playedCardCount;
	//should be 1 - smithy played
	expected=before + 1;
	assertTrue(after, expected);
	printf("	Cards in played cards after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards in Player 1's Deck\n");
    //should be 2 to start
    before=origTest.deckCount[player1];
	printf("	Cards in deck before playing Steward: %d\n", before);
	after=postTest.deckCount[player1];
	//should be 2 - no change
	expected=before;
	assertTrue(after, expected);
	printf("	Cards in deck after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #4: Testing Coins + 2\n");
	before=origTest.coins;
	printf("	Cards in deck before playing Smithy: %d\n", before);
	after=postTest.coins;
	//should be 2
	expected=before + 2;
	assertTrue(after, expected);
	printf("	Coins after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);


	printf("Round Three:  CHOICE TO TRASH 2 CARDS FROM HAND\n");
	choice1=3; //choose to trash 2 cards from hand
	
	choice2=1; //trash 1st card from hand
	choice3=2; //trash 2nd card from hand


	//copies starting point for comparison
	postTest = origTest;

	cardEffect(steward, choice1, choice2, choice3, &postTest, handPos, &bonus);

	printf("	Test #1: Testing Correct Number of Cards in Player 1's Hand\n");
    //should be 3 to start
    before=origTest.handCount[player1];
	printf("	Cards in hand before playing Steward: %d\n", before);
	after=postTest.handCount[player1];
	//should be 0; 3 to start and -1 smithy played - 2 cards trashed
	expected=before - 1 - 2;
	assertTrue(after, expected);
	printf("	Cards in hand after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards in Player 1's Played Cards\n");
    //should be 0 to start
    before=origTest.playedCardCount;
	printf("	Cards in played cards before playing Steward: %d\n", before);
	after=postTest.playedCardCount;
	//should be 1 - smithy played, and other cards were TRASHED
	expected=before + 1;
	assertTrue(after, expected);
	printf("	Cards in played cards after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards in Player 1's Deck\n");
    //should be 2 to start
    before=origTest.deckCount[player1];
	printf("	Cards in deck before playing Steward: %d\n", before);
	after=postTest.deckCount[player1];
	//should be 2 - no change
	expected=before;
	assertTrue(after, expected);
	printf("	Cards in deck after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #4: Testing Coins Not Changed\n");
	before=origTest.coins;
	printf("	Cards in deck before playing Smithy: %d\n", before);
	after=postTest.coins;
	//should be 0 - no change
	expected=before;
	assertTrue(after, expected);
	printf("	Coins after playing Steward:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);


    if(allPassed)
                printf("ALL TESTS PASSED!\n");
    	else
                printf("%d OF THE TESTS HAVE FAILED\n", failedTests);

    return 0;
}

