/*
* exclaim.c
*
* Author: Joseph Kim
* Version: February 1, 2023
*
* This program recieves a word and then turns all lowercase
* letters to a randomly chosen special character.
*
*/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // initializing random number generator
  time_t t;
  srand((unsigned) time(&t));

  // array that contains the special characters
  char replace[4] = {'@', '!', '#', '*'};

  // receives word from the user
  char buff[32]; // string buffer to receive word
  printf("Enter a word: ");
  scanf("%s", buff);

  int n = strlen(buff); // length of string

  // goes through each character replaces all lowercase letters
  for (int i = 0; i < n; i++) {
    if (buff[i] > 96 && buff[i] < 123) {
      buff[i] = replace[rand() % 4];
    }
  }

  printf("OMG! %s\n", buff);
  return 0;
}
