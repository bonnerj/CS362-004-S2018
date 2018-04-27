//Scoring for Victory cards comes from: http://wiki.dominionstrategy.com/index.php/Basic_cards

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

//Each gardens card adds a point for every 10 cards in player's the entire deck 
//(hand+discard+deck), rounding down according to rules of the game
int gardensAdds(struct gameState *state, int player) {
	int adds = state->handCount[player] + state->deckCount[player] 
						+ state->discardCount[player];
	//printf("		Total Deck Count = %d\n", adds);
	adds = adds/10;
	//printf("		Count/10 = %d\n", adds);
	return adds;
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


	printf("*************TESTING scoreFor *************\n\n"); 

    printf("Round One: TESTING SCORE WITH GAME JUST INITIALIZED\n");
    //Each player should start with 7 coppers and 3 estates; score should be 3
    score=scoreFor(player, &test);
	assertTrue(3, score);
	printf("Expected return value=3   Actual value=%d\n", score);

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

	printf("\nRound Two: TESTING WITH EMPTY HAND, DISCARD, AND DECK\n"); 
    score=scoreFor(player, &test);
    assertTrue(expected, score);
    printf("Expected return value=%d  Actual value=%d\n", expected, score);

	int victories[5]= {estate, duchy, province, great_hall, gardens};
	const char *cardName[] = {"estate", "duchy", "province", "great_hall", "gardens"};
    int adds[5]= {1, 3, 6, 1, gardensAdds(&test, player)};

	printf("\nRound Three: TESTING VICTORY CARDS IN HAND ONE AT A TIME\n");

    for (i=0; i<5; i++)
    {
    	expected = 0;
    	printf("	TEST #%d: Testing score after adding one %s worth %d to hand\n", 
    			i+1, cardName[i], adds[i]);
    	test.hand[ player ][ test.handCount[player] ] = victories[i];
    	test.handCount[player]++;
    	score=scoreFor(player, &test);
    	expected += adds[i];
    	assertTrue(expected, score);
    	printf("Expected return value=%d   Actual value=%d\n", expected, score);
    	//Removing card to test one at a time
    	printf("	Removing %s...\n", cardName[i]);
    	test.handCount[player]--;
    	test.hand[ player ][ test.handCount[player] ] = -1;
    }

    //adding 10 cards worth 0 victory points to test that gardens will add +1  
    for (i=0; i<10; i++)
    {
    	test.hand[ player ][ test.handCount[player] ] = k[1];
    	test.handCount[player]++;
    }
 

    printf("	TEST #7: Testing score after adding 10 adventurer cards and one gardens card to hand.\n"); 
    test.hand[ player ][ test.handCount[player] ] = victories[4];
    test.handCount[player]++;
    score=scoreFor(player, &test);
    expected = gardensAdds(&test, player);
    assertTrue(expected, score);
    printf("Expected return value=%d   Actual value=%d\n", expected, score);

    printf("Removing all cards from hand...\n");
	for (i = 0; i < test.handCount[player]; i++)
	{
		test.hand[player][i] = -1;
	}

	test.handCount[player] = 0;


	printf("\nRound Four: TESTING VICTORY CARDS IN DISCARD\n");

    for (i=0; i<5; i++)
    {
    	expected = 0;
    	printf("	TEST #%d: Testing score after adding one %s worth %d to discard\n", 
    			i+1, cardName[i], adds[i]);
    	test.discard[ player ][ test.discardCount[player] ] = victories[i];
    	test.discardCount[player]++;
    	score=scoreFor(player, &test);
    	expected += adds[i];
    	assertTrue(expected, score);
    	printf("	Expected return value=%d   Actual value=%d\n", expected, score);
    	//removing card to test one at a time
    	printf("	Removing %s...\n", cardName[i]);
    	test.discard[ player ][ test.discardCount[player] ] = -1;
    	test.discardCount[player]=0;
    }

    printf("\n	TEST #7: Testing score after adding 10 adventurer cards and one gardens card to hand.\n");
    //adding 10 cards worth 0 victory points to test that gardens will add +1  
    printf("	Adding 10 gardens cards...\n");
    for (i=0; i<10; i++)
    {
    	test.discard[ player ][ test.discardCount[player] ] = k[1];
    	test.discardCount[player]++;
    }

    test.discard[ player ][ test.handCount[player] ] = gardens;
    test.discardCount[player]++;
    score=scoreFor(player, &test);
	expected = gardensAdds(&test, player);
    assertTrue(expected, score);
    printf("	Expected return value=%d   Actual value=%d\n", expected, score);



     //reset hand for next test
    printf("Removing all cards from discard...\n");
	for (i = 0; i < test.discardCount[player]; i++)
	{
		test.discard[player][i] = -1;
	}

	test.discardCount[player] = 0;



	printf("\nRound Five: TESTING VICTORY CARDS IN DECK\n");

    for (i=0; i<5; i++)
    {
		expected = 0;
    	printf("	TEST #%d: Testing score after adding one %s worth %d to deck\n", 
    			i+1, cardName[i], adds[i]);
    	test.deck[ player ][ test.deckCount[player] ] = victories[i];
    	test.deckCount[player]++;
    	score=scoreFor(player, &test);
    	expected += adds[i];
    	assertTrue(expected, score);
    	printf("	Expected return value=%d   Actual value=%d\n", expected, score);
    	//removing card to test one at a time
    	printf("	Removing %s...\n", cardName[i]);
    	test.deck[ player ][ test.deckCount[player] ] = -1;
    	test.deckCount[player]=0;
    }

    //adding 10 cards worth 0 victory points to test that gardens will add +1  
    for (i=0; i<10; i++)
    {
    	test.deck[ player ][ test.deckCount[player] ] = k[1];
    	test.deckCount[player]++;
    }

    printf("	TEST #7: Testing score after adding 10 adventurer cards and one gardens card to deck.\n"); 
    test.deck[ player ][ test.deckCount[player] ] = gardens;
    test.deckCount[player]++;
    score=scoreFor(player, &test);
    expected = gardensAdds(&test, player);
    assertTrue(expected, score);
    printf("	Expected return value=%d   Actual value=%d\n", expected, score);

      //reset hand for next test
    printf("Removing all cards from deck...\n");
	for (i = 0; i < test.deckCount[player]; i++)
	{
		test.deck[player][i] = -1;
	}

	test.deckCount[player] = 0;

	printf("\nRound Six: TESTING CURSE CARD\n");

		printf("	TEST #1: Testing score after adding curse to hand with no other cards.\n");
		test.hand[ player ][ test.handCount[player] ] = curse;
   	 	test.handCount[player]++;
    	score=scoreFor(player, &test);
    	expected = -1;
    	assertTrue(expected, score);
    	printf("	Expected return value=%d   Actual value=%d\n", expected, score);
    	test.handCount[player]--;
    	test.hand[ player ][ test.handCount[player] ] = -1;


		printf("	TEST #2: Testing score after adding curse to hand when player has one point.\n");
		score=scoreFor(player, &test);
		expected = 0;
		assertTrue(expected, score);
    	printf(" 	Score before adding any cards = %d\n", score);
    	printf("			Expected return value=%d\n", expected);
		test.hand[ player ][ test.handCount[player] ] = estate;
   	 	test.handCount[player]++;
   	 	score=scoreFor(player, &test);
		expected = 1;
		assertTrue(expected, score);
   	 	printf(" 	Score after adding one estate card = %d\n", score);
   	 	printf("			Expected return value=%d\n", expected);
   	 	test.hand[ player ][ test.handCount[player] ] = curse;
   	 	test.handCount[player]++;
   	 	score=scoreFor(player, &test);
		expected = 0;
		assertTrue(expected, score);
		printf(" 	Score after adding curse card = %d\n", score);
    	printf("			Expected return value=%d\n", expected);
    	printf("	Removing curse from hand...\n");
    	test.handCount[player]--;
    	test.hand[ player ][ test.handCount[player] ] = -1;


    	printf("	TEST #3: Testing score after adding curse to discard when player has one point.\n");
    	score=scoreFor(player, &test);
		expected = 1;
		assertTrue(expected, score);
    	printf(" 	Score before adding curse card = %d\n", score);
    	printf("			Expected return value=%d\n", expected);
   	 	test.discard[ player ][ test.discardCount[player] ] = curse;
   	 	test.discardCount[player]++;
   	 	score=scoreFor(player, &test);
		expected = 0;
		assertTrue(expected, score);
		printf(" 	Score after adding curse card = %d\n", score);
    	printf("			Expected return value=%d\n", expected);
    	printf("	Removing curse from discard...\n");
    	test.discardCount[player]--;
    	test.discard[ player ][ test.discardCount[player] ] = -1;


    	printf("	TEST #4: Testing score after adding curse to deck when player has one point.\n");
    	score=scoreFor(player, &test);
		expected = 1;
		assertTrue(expected, score);
    	printf(" 	Score before adding curse card = %d\n", score);
    	printf("			Expected return value=%d\n", expected);
   	 	test.deck[ player ][ test.deckCount[player] ] = curse;
   	 	test.deckCount[player]++;
   	 	score=scoreFor(player, &test);
		expected = 0;
		assertTrue(expected, score);
		printf(" 	Score after adding curse card = %d\n", score);
    	printf("			Expected return value=%d\n", expected);

 	if(allPassed)
                printf("ALL TESTS PASSED!\n");
    	else
                printf("%d TESTS HAVE FAILED\n", failedTests);


    return 0;
}

