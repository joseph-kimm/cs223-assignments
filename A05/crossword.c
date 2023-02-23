/*------------------------------------------------
 * Author: Joseph Kim
 * Date: 02/22/2023
 * Description: this program takes two words fro the user, identifies
 * a common letter and makes a crossword witht the two words
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// forward declaration of function
char findCommon(char* str1, char* str2);

int main(int rgc, char* argv[]) {
  int len1 = strlen(argv[1]); // length of first word
  int len2 = strlen(argv[2]); // length of second word
  char* str1 = malloc(sizeof(char) * len1 + 1); // first word
  char* str2 = malloc(sizeof(char) * len2 + 1); // second word

  // checking for malloc error
  if (str1 == NULL || str2 == NULL) {
    exit(1);
  }

  strcpy(str1, argv[1]);
  strcpy(str2, argv[2]);

  // finds common letter 
  char common = findCommon(str1, str2);

  // case where there is no common letter
  if (common == 63) {
    printf("No common letter!\n");
    free(str1);
    free(str2);
    return 0;
  }

  // finding the position of the common letter for both the strings
  int row = (int)(strchr(str1, common) - str1);
  int column = (int)(strchr(str2, common) - str2);

  // creates a 2D array to store the crossword
  char** crossword = malloc(sizeof(char*) * len1);
  for (int i = 0; i < len1; i++) {
    crossword[i] = malloc(sizeof(char) * len2);
  }

  // going through each tile of the crossword
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {

      // entering a letter from the vertical word
      if (j == column) {
        crossword[i][j] = str1[i];
      }

      // entering a letter from the horizontal word 
      else if (i == row) {
        crossword[i][j] = str2[j];
      }

      // filling the rest of crossword with '.'
      else {
        crossword[i][j] = '.';
      }

      // printing each tile of the crossword
      printf("%c ", crossword[i][j]); 
    }
    printf("\n");
  }

  // freeing all the heap memory
  for (int i = 0; i < len1; i++) {
    free(crossword[i]);
  }
  free(crossword);
  free(str1);
  free(str2);
  return 0;
}

/**
 * this function receives two strings and finds a common letter for both
 * the strings
 *
 * @param str1: first string
 * @param str2: second string
 * return: common letter, returns ? if there are no common letters
*/
char findCommon(char* str1, char* str2) {

  // compares all the letters of string 1 with all the letters
  // of string 2
  for (int i = 0; i < strlen(str1); i++) {
    for (int j = 0; j < strlen(str2); j++) {
      if (str1[i] == str2[j]) {
        return str1[i];
      }
    }
  }

  // case when there is no common letter
  return 63;
}
