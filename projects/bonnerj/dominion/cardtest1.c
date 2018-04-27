/******************************************************************************
*	From Wikipedia: http://wiki.dominionstrategy.com/index.php/Adventurer
*	Description of Adventurer Card: "Reveal cards from your deck until you 
*	reveal 2 Treasure cards.  Put those Treasure cards into your hand and
*	discard the other revealed cards.  "
*	FAQs/Rule Clarifications: 
*	If you run out of cards after  shuffling and still only have one Treasure, 
*	you just get that one treasure.
*	If you have to shuffle during, then do so.  Don't shuffle in the revealed
*	cards as these do not go to the Discard until finished revealing cards."

*	From Code:
	void adventurerCard(int drawntreasure, struct gameState *state, int handPos, 
	        int z, int currentPlayer)
	{    
	    int temphand[MAX_HAND];

	    while(drawntreasure<2){
	        //if the deck is empty we need to shuffle discard and add to deck
		if (state->deckCount[currentPlayer] < 1){
		  shuffle(currentPlayer, state);
		}
		drawCard(currentPlayer, state);
	        //top card of hand is most recently drawn card.
		int cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];
		if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
		  drawntreasure++;
		else{
		  temphand[z]=cardDrawn;
	          //this should just remove the top card (the most recently drawn one).  
		  state->handCount[currentPlayer]--;
		  z++;
	        }
	    }
	    while(z>=0){
	        // discard all cards in play that have been drawn
	        state->discard[currentPlayer][state->discardCount[currentPlayer]++]
	            =temphand[z-1]; 
		z=z-1;
	      }
	}
* 	Notes: 
	drawntreasure=0, z=0 (counter for tempHand) - set from within cardeffect()
  	currentPlayer - state->whoseTurn is initialized to 0 when game is initialized 
  	and set from within cardeffect()
*****************************************************************************/

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
	int i;
	//for initializing game
	int numPlayers = 2;
	int randomSeed = 25;
	int player1=0;
	int k[10]= {adventurer, council_room, feast, gardens, mine,
                remodel, smithy, village, baron, great_hall};
    struct gameState origTest, postTest;

    //for cardEffect()
    //choice1, choice2, choice3, and bonus are NOT used in adventurer
    int choice1=-1;
	int choice2=-1;
	int choice3=-1;
	int bonus=0;

	int handPos=0;

    int expected=0;
    int before=0;
    int after=0;
    //starting value when game is initialized
    int totalCards=0;

    initializeGame(numPlayers, k, randomSeed, &origTest);

    printf("*************TESTING Adventurer Card *************\n\n"); 

    printf("Round One: TESTING CARD WITH 5 COPPERS IN DECK AND ONLY ADVENTURER IN HAND\n");

    //start by clearing out the deck to start fresh
    clearDeck(&origTest, player1);

	for (i = 0; i < 5; i++)
	{
		origTest.deck[player1][i] = copper;
		origTest.deckCount[player1]++;
		origTest.coins++;
	}

	origTest.hand[player1][0] = adventurer;
	origTest.handCount[player1]++;

	totalCards=6;

    postTest = origTest;

    cardEffect(adventurer, choice1, choice2, choice3, &postTest, handPos, &bonus);

    printf("	Test #1: Testing Correct Number of Cards Gained in Player 1's Hand\n");
    //should be 5
    before=origTest.handCount[player1];
	printf("	Cards in hand before playing adventurer: %d\n", before);
	after=postTest.handCount[player1];
	//should be 6; 5 to start +2 treasures and -1 adventurer played
	expected=before + 2 - 1;
	assertTrue(after, expected);
	printf("	Cards in hand after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards Discarded by Player 1\n");
    //should be 0
    before=origTest.discardCount[player1];
	printf("	Cards in discard before playing adventurer: %d\n", before);
	after=postTest.discardCount[player1];
	//should be 0
	expected= before;
	assertTrue(after, expected);
	printf("	Cards in discard after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards Left in Player 1's deck\n");
    //should be 5
    before=origTest.deckCount[player1];
	printf("	Cards in deck before playing adventurer: %d\n", before);
	after=postTest.deckCount[player1];
	//deck = 5 - 2 = 3
	expected= 3;
	assertTrue(after, expected);
	printf("	Cards in deck after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #4: Testing Correct Number of Cards Played by Player 1\n");
    //should be 0
    before=origTest.playedCardCount;
	printf("	Cards in played cards before playing adventurer: %d\n", before);
	after=postTest.playedCardCount;
	//should be 1 - just adventurer
	expected= 1;
	assertTrue(after, expected);
	printf("	Cards in played cards after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #5: Testing Correct Number of Total Cards For Player 1\n");
	expected=totalCards;
	printf("	Total cards before playing adventurer: %d\n", before);
	after=postTest.handCount[player1] + postTest.deckCount[player1]
				+ postTest.discardCount[player1] + postTest.playedCardCount;
	assertTrue(after, expected);
	printf("	Total cards after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #6: Testing Updated Coins\n");
	before=origTest.coins;
	printf("	Total coins before playing adventurer: %d\n", before);
	after=postTest.coins;
	expected=origTest.coins + 2;
	assertTrue(after, expected);
	printf("	Total coins after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);


	printf("Round Two: TESTING CARD WITH 1 TREASURES IN DECK AND ONLY ADVENTURER IN HAND\n");

	//start by clearing out the deck to start fresh
    clearDeck(&origTest, player1);


	origTest.deck[player1][0] = copper;
	origTest.deckCount[player1]++;

	origTest.hand[player1][0] = adventurer;
	origTest.handCount[player1]++;

	totalCards=2;

    postTest = origTest;

    cardEffect(adventurer, choice1, choice2, choice3, &postTest, handPos, &bonus);

	printf("	Test #1: Testing Correct Number of Cards Gained in Player 1's Hand\n");
    //should be 1
    before=origTest.handCount[player1];
	printf("	Cards in hand before playing adventurer: %d\n", before);
	after=postTest.handCount[player1];
	//should be 1; 1 to start +1 treasures and -1 adventurer played
	expected=before + 1 - 1;
	assertTrue(after, expected);
	printf("	Cards in hand after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards Discarded by Player 1\n");
    //should be 0
    before=origTest.discardCount[player1];
	printf("	Cards in discard before playing adventurer: %d\n", before);
	after=postTest.discardCount[player1];
	//no cards discarded
	expected=before;
	assertTrue(after, expected);
	printf("	Cards in discard after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards Left in Player 1's deck\n");
    //should be 1
    before=origTest.deckCount[player1];
	printf("	Cards in deck before playing adventurer: %d\n", before);
	after=postTest.deckCount[player1];
	//deck = 1 - 1 = 0
	expected= 0;
	assertTrue(after, expected);
	printf("	Cards in deck after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);
	
	printf("	Test #4: Testing Correct Number of Cards Played by Player 1\n");
    //should be 0
    before=origTest.playedCardCount;
	printf("	Cards in played cards before playing adventurer: %d\n", before);
	after=postTest.playedCardCount;
	//should be 1 - just adventurer
	expected= 1;
	assertTrue(after, expected);
	printf("	Cards in played cards after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);


	printf("	Test #5: Testing Correct Number of Total Cards For Player 1\n");
	expected=totalCards;
	printf("	Total cards before playing adventurer: %d\n", before);
	after=postTest.handCount[player1] + postTest.deckCount[player1]
				+ postTest.discardCount[player1] + postTest.playedCardCount;
	assertTrue(after, expected);
	printf("	Total cards after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #6: Testing Updated Coins\n");
	before=origTest.coins;
	printf("	Total coins before playing adventurer: %d\n", before);
	after=postTest.coins;
	expected=origTest.coins + 1;
	assertTrue(after, expected);
	printf("	Total coins after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);



printf("Round Three: TESTING CARD WITH ONLY ADVENTURER IN DECK (0 TREASURES) ");
printf( "AND ONLY ADVENTURER IN HAND\n");

	//start by clearing out the deck to start fresh
    clearDeck(&origTest, player1);

	origTest.deck[player1][0] = adventurer;
	origTest.deckCount[player1]++;


	origTest.hand[player1][0] = adventurer;
	origTest.handCount[player1]++;

	totalCards=2;

    postTest = origTest;

    cardEffect(adventurer, choice1, choice2, choice3, &postTest, handPos, &bonus);

	printf("	Test #1: Testing Correct Number of Cards Gained in Player 1's Hand\n");
    //should be 1
    before=origTest.handCount[player1];
	printf("	Cards in hand before playing adventurer: %d\n", before);
	after=postTest.handCount[player1];
	//should be 0; 1 to start +0 treasures and -1 adventurer played
	expected=before + 0 - 1;
	assertTrue(after, expected);
	printf("	Cards in hand after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards Discarded by Player 1\n");
    //should be 0
    before=origTest.discardCount[player1];
	printf("	Cards in discard before playing adventurer: %d\n", before);
	after=postTest.discardCount[player1];
	//only card (adventurer) in hand should be discarded at the end
	expected= 1;
	assertTrue(after, expected);
	printf("	Cards in discard after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards Left in Player 1's deck\n");
    //should be 5
    before=origTest.deckCount[player1];
	printf("	Cards in deck before playing adventurer: %d\n", before);
	after=postTest.deckCount[player1];
	//deck = 1 - 1 = 0
	expected= 0;
	assertTrue(after, expected);
	printf("	Cards in deck after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #4: Testing Correct Number of Cards Played by Player 1\n");
    //should be 0
    before=origTest.playedCardCount;
	printf("	Cards in played cards before playing adventurer: %d\n", before);
	after=postTest.playedCardCount;
	//should be 1 - just adventurer
	expected= 1;
	assertTrue(after, expected);
	printf("	Cards in played cards after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #5: Testing Correct Number of Total Cards For Player 1\n");
	expected=totalCards;
	printf("	Total cards before playing adventurer: %d\n", before);
	after=postTest.handCount[player1] + postTest.deckCount[player1]
				+ postTest.discardCount[player1] + postTest.playedCardCount;
	assertTrue(after, expected);
	printf("	Total cards after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	printf("	Test #6: Testing Updated Coins\n");
	before=origTest.coins;
	printf("	Total coins before playing adventurer: %d\n", before);
	after=postTest.coins;
	expected=origTest.coins + 0;
	assertTrue(after, expected);
	printf("	Total coins after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n", expected, after);

	if(allPassed)
                printf("ALL TESTS PASSED!\n");
    	else
                printf("%d TESTS HAVE FAILED\n", failedTests);


	return 0;
}
