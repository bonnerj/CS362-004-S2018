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
		printf("PASSED TEST: ");
	else
		printf("FAILED TEST: ");
}

int main() {
	int numPlayers = 2;
	int randomSeed = 25;
	int gameOver;

	int k[10] = {adventurer, council_room, feast, gardens, mine,
	       		remodel, smithy, village, baron, great_hall};
	

	struct gameState test;

	initializeGame(numPlayers, k, randomSeed, &test);

	printf("*************TESTING isGameOver *************\n\n"); 

	printf("First test:  Testing Return Value with Game Just Initialized\n");
	gameOver=isGameOver(&test);
	assertTrue(0, gameOver);
	printf("Expected return value=0   Actual value=%d\n", gameOver);


	printf("Second test:  Testing Return Value with Province Cards Empty\n");
	test.supplyCount[province]=0;
	gameOver=isGameOver(&test);
	assertTrue(1, gameOver);
	printf("Expected return value=1  Actual value=%d\n", gameOver);

	printf("Third test:  Testing Return Value with One Supply Pile at 0\n");
	test.supplyCount[province]=1;
	test.supplyCount[0] = 0;
	gameOver=isGameOver(&test);	
	assertTrue(0, gameOver);
	printf("Expected return value=0  Actual value=%d\n", gameOver);

	printf("Fourth test:  Testing Return Value with Two Supply Piles at 0\n");
	test.supplyCount[province]=1;
	test.supplyCount[0] = 0;
	test.supplyCount[1] = 0;
	gameOver=isGameOver(&test);	
	assertTrue(0, gameOver);
	printf("Expected return value=0  Actual value=%d\n", gameOver);

	printf("Fifth test:  Testing Return Value with Three Supply Piles at 0\n");
	test.supplyCount[province]=1;
	test.supplyCount[0] = 0;
	test.supplyCount[1] = 0;
	test.supplyCount[2] = 0;
	gameOver=isGameOver(&test);	
	assertTrue(1, gameOver);
	printf("Expected return value=0  Actual value=%d\n", gameOver);

	if(allPassed)
                printf("ALL TESTS PASSED!\n");
    	else
                printf("%d TESTS HAVE FAILED\n", failedTests);


    return 0;
}



