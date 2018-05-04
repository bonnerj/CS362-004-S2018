#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    //ASCII characters 32(' ') to 126('~') decimal
    int random = rand() % 94;
    char x = ' ' + random; 
    return x;
}

char *inputString()
{
    int i;
    
    char letters[42] = {'\0', 'a', 'b', 'c', 'd', 'f', 'g', 'h', 'i', 'j',
			'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 'e', 's', 'e', 't',
			'u', 'v', 'w', 'x', 'y', 'z', 'R', 'E', 'S', 'E', 'T', ',',
			' '};

    //variable to hold the string returned, cleared out
    static char x[6];
    memset(x, 0, 6);

    //ensures each char only used once
    int usedLetters[42];
    for (i=0; i<42; i++) {
	usedLetters[i] = 0;
    }
    
    //finds a random index in array of char
    int index = rand() % 42;

    //fills the string with the random characters
    for (i = 0; i < 6; i++) {
	while(usedLetters[index])
            index = rand() % 42;
	char letter = letters[index];
	x[i] = letter;
	usedLetters[index] = 1; 
    }


    return x;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    //printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error\n\n ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
