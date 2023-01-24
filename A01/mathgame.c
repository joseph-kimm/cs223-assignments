/***************************************************
 * mathgame.c
 * Author:Joseph Kim
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  // initializing random number generator
  time_t t;
  srand((unsigned) time(&t));

  // welcoming user and receiving answer
  printf("Welcome to Math Game!\n");
  printf("How many rounds do you want to play? ");
  int rounds = 0;
  scanf("%d", &rounds);

  // initializing other variables
  int correct = 0;
  int response = 0;

  // for loop ask each question
  for (int i=0; i<rounds; i++) {
    printf("\n");

    // generating two random integers between 1 and 9 
    int num1 = rand() % 9 + 1;
    int num2 = rand() % 9 + 1;

    // asking user and then recieving answer
    printf("%d + %d = ? ", num1, num2);
    scanf("%d", &response);

    // checking if answer is correct or not
    if (response == num1 + num2) {
      printf("Correct!\n");
      correct += 1;
    } else {
      printf("Incorrect :(\n");
    }
  }

  // printing the results of the game
  printf("You answered %d/%d correctly.\n", correct, rounds);
}
