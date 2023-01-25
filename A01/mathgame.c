/***************************************************
 * mathgame.c
 *
 * Author:Joseph Kim
 * Version:January 25, 2023
 *
 * Implements a math game that asks the users a series of addition problems of 
 * numbers between 1 and 9. It also keeps the number of correct answers.
 *
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
  int rounds = 0; //number of rounds
  scanf("%d", &rounds);

  int correct = 0; // number of correct answer
  int response = 0; // value of the response

  // for loop asking each question
  for (int i=0; i<rounds; i++) {
    printf("\n");

    // generating two random integers between 1 and 9
    int num1 = (rand() % 9) + 1;
    int num2 = (rand() % 9) + 1;

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
