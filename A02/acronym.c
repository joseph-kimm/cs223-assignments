/*
* acronym.c
*
* Author: Joseph Kim
* Version: February 1, 2023
*
* This program receives a phrase from the user and creates an
* acronym of the phrase.
*
*
*/

#include <stdio.h>
#include <string.h>

int main() {

  // asks and receives a phrase from the user
  printf("Enter a phrase: ");
  char phrase[1024];
  scanf("%[^\n]%*c", phrase);

  int n = strlen(phrase); // length of string received
  char acronym[1024]; // string to store acronym
  int j = 0; // index for string acronym

  // adds the first letter in acronym if it is a capital letter
  if (phrase[0] > 64 && phrase[0] < 91) {
    acronym[j] = phrase[0];
    j++;
  }

  // checks if the character is space and if it is followed by a capital letter
  // if so, adds the capital letter in the acronym
  for (int i = 1; i < n; i++) {
    if (phrase[i] == 32 && (phrase[i+1] > 64 && phrase[i+1] < 91)) {
      acronym[j] = phrase[i+1];
      j++;
    }
  }

  // adds null character to end the acronym string
  acronym[j] = '\0';

  printf("Your acronym is %s\n", acronym);
  return 0;
}

