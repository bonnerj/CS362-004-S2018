/******************************************************************************
*	From Wikipedia: http://wiki.dominionstrategy.com/index.php/Adventurer
*	Card Description: "Reveal cards from your deck until you 
*	reveal 2 Treasure cards.  Put those Treasure cards into your hand and
*	discard the other revealed cards.  "
*	FAQs/Rule Clarifications: 
*	If you run out of cards after  shuffling and still only have one Treasure, 
*	you just get that one treasure.
*	If you have to shuffle during, then do so.  Don't shuffle in the revealed
*	cards as these do not go to the Discard until finished revealing cards."

* 	From Code:
*	void adventurerCard(int drawntreasure, struct gameState *state, int handPos, 
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
#include "interface.h"

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

void testSuite(struct gameState *origTest, struct gameState *postTest, int player1, int totalCards, int numTreasures, int nonTreasures)
{

    int expected=0;
    int before=0;
    int after=0;

    printf("	Test #1: Testing Correct Number of Cards Gained in Player 1's Hand\n");
    before=origTest->handCount[player1];
	printf("	Cards in hand before playing adventurer: %d\n", before);
	printHand(player1, origTest);
	after=postTest->handCount[player1];
	expected=before + numTreasures - 1;
	assertTrue(after, expected);
	printf("	Cards in hand after playing adventurer:\n");
	printHand(player1, postTest);
	printf("			Expected=%d 	Actual=%d\n\n", expected, after);

	printf("	Test #2: Testing Correct Number of Cards Discarded by Player 1\n");
    before=origTest->discardCount[player1];
	printf("	Cards in discard before playing adventurer: %d\n", before);
	printDiscard(player1, origTest);
	after=postTest->discardCount[player1];
	expected= before + nonTreasures;
	assertTrue(after, expected);
	printf("	Cards in discard after playing adventurer:\n");
	printDiscard(player1, postTest);
	printf("			Expected=%d 	Actual=%d\n\n", expected, after);

	printf("	Test #3: Testing Correct Number of Cards Left in Player 1's deck\n");
    before=origTest->deckCount[player1];
    printDeck(player1, origTest);
	printf("	Cards in deck before playing adventurer: %d\n", before);
	after=postTest->deckCount[player1];
	expected= before - numTreasures;
	assertTrue(after, expected);
	printf("	Cards in deck after playing adventurer:\n");
    printDeck(player1, postTest);
	printf("			Expected=%d 	Actual=%d\n\n", expected, after);

	printf("	Test #4: Testing Correct Number of Cards Played by Player 1\n");
    before=origTest->playedCardCount;
    printPlayed(player1, origTest);
	printf("	Cards in played cards before playing adventurer: %d\n", before);
	after=postTest->playedCardCount;
	expected= 1;
	assertTrue(after, expected);
	printf("	Cards in played cards after playing adventurer:\n");
	printPlayed(player1, postTest);
	printf("			Expected=%d 	Actual=%d\n\n", expected, after);

	printf("	Test #5: Testing Correct Number of Total Cards For Player 1\n");
	expected=totalCards;
	printf("	Total cards before playing adventurer: %d\n", before);
	after=postTest->handCount[player1] + postTest->deckCount[player1]
				+ postTest->discardCount[player1] + postTest->playedCardCount;
	assertTrue(after, expected);
	printf("	Total cards after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n\n", expected, after);

	printf("	Test #6: Testing Updated Coins\n");
	before=origTest->coins;
	printf("	Total coins before playing adventurer: %d\n", before);
	after=postTest->coins;
	expected=origTest->coins + numTreasures;
	assertTrue(after, expected);
	printf("	Total coins after playing adventurer:\n");
	printf("			Expected=%d 	Actual=%d\n\n", expected, after);
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

    testSuite(&origTest, &postTest, player1, totalCards, 2, 0);

    //should be 5 cards in hand to start; should end with 6 5 (+2 treasures and -1 adventurer played)
    //should be 0 cards in discard until turn ends (according to rules) 
    //should be 0 cards in played cards to start; should be 1 at end (adventurer)
    //should be total cards = 6 (none trashed)
    //should be 0 coins to start; should be +2 at end 


	printf("Round Two: TESTING CARD WITH ONLY 1 TREASURES IN DECK AND ONLY ADVENTURER IN HAND\n");

	//start by clearing out the deck to start fresh
    clearDeck(&origTest, player1);


	origTest.deck[player1][0] = copper;
	origTest.deckCount[player1]++;

	origTest.hand[player1][0] = adventurer;
	origTest.handCount[player1]++;

	totalCards=2;

    postTest = origTest;

    cardEffect(adventurer, choice1, choice2, choice3, &postTest, handPos, &bonus);

    testSuite(&origTest, &postTest, player1, totalCards, 1, 0);


    //should be 1 card in hand to start; should end in 1 (+1 treasure - 1 adventurer played)	
    //should be 0 cards in discard until turn ends (according to rules) 
    //should be 1 card in deck to start; should be 0 at end (-1 treasure card)
    //should be 0 cards in played cards to start; should be 1 at end (adventurer)
    //should be total cards = 2 (none trashed)
    //should be 0 coins to start; should be +1 at end 



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

    testSuite(&origTest, &postTest, player1, totalCards, 0, 1);


    //should be 1 card in hand to start; should end in 0 (+0 treasure - 1 adventurer played)	
    //should be 0 cards in discard until turn ends (according to rules) 
    //should be 1 card in deck to start; should be 1 at end 
    //should be 0 cards in played cards to start; should be 1 at end (adventurer)
    //should be total cars = 2 (none trashed)
    //should be 0 coins to start; should be 0 at end 


	if(allPassed)
                printf("ALL TESTS PASSED!\n");
    	else
                printf("%d TESTS HAVE FAILED\n", failedTests);


	return 0;
}