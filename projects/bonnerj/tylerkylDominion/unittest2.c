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
	else {
		printf("FAILED TEST: ");
		allPassed=0;
		failedTests += 1;
	}
}

int main() {
	int c;
	char buf[BUFSIZ];

	int cards[27] = {curse, estate, duchy, province, copper, silver, gold, 
			adventurer, council_room, feast, gardens, mine, remodel, 
			smithy, village, baron, great_hall, minion, steward, 
			tribute, ambassador, cutpurse, embargo, outpost, salvager, 
			sea_hag, treasure_map};

	const char *cardName[] = {"curse", "estate", "duchy", "province", 
				 "copper", "silver", "gold", "adventurer", 
				 "council_room", "feast", "gardens", "mine", 
				 "remodel", "smithy", "village", "baron", 
				 "great_hall", "minion", "steward", "tribute", 
				 "ambassador", "cutpurse", "embargo", "outpost", 
				 "salvager", "sea_hag", "treasure_map"};

	int actualCost[27] = {0, 2, 5, 8, 0, 3, 6, 6, 5, 4, 4, 5, 4, 4, 3, 4, 
			     3, 5, 3, 5, 3, 4, 2, 5, 4, 4, 4};

	printf("*************TESTING getCost*************\n\n");

	int i;
	for (i = 0; i<27; i++)
	{
		printf("TEST #%d: Testing cost of %s", i+1, cardName[i]);
		printf("%s\n", buf);
		c=getCost(cards[i]);
		assertTrue(c, actualCost[i]);
		printf("Expected return value=%d    Actual value=%d\n", actualCost[i], c);
	}


	printf("TEST #28:  Testing cost of nonexistant card\n");
	int random = random;
	c=getCost(random);
	assertTrue(c, -1);
	printf("Expected return value=-1    Actual value=%d\n", c);

	if(allPassed)
		printf("ALL TESTS PASSED!\n");
	else
		printf("%d TESTS HAVE FAILED\n", failedTests);
	return 0;
}
