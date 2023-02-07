/*----------------------------------------------
 * Author: Joseph Kim
 * Date: Feb 6th, 2023 
 * Description: This prject takes in a string and converts it into an ent-phrase
 * where there are '.' characters between each letter
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

  // getting the number of '.'s that will follow each letter
  printf("Pause length: ");
  int p_len;
  scanf("%i", &p_len);

  // getting the string that will turn into ent-phrase
  printf("Text: ");
  char buff[32];
  scanf("%s", buff);
  int t_len = strlen(buff); // int containing length of input string

  // string that will contain the dots that will come after each letter
  char* dots = NULL;
  dots = malloc(sizeof(char) * (p_len+1));

  // checking for malloc error
  if (dots == NULL) {
    printf("Bad malloc error\n");
    exit(0);
  }

  // putting the given number of dots to a string
  for (int i = 0; i < p_len; i++) {
    dots[i] = '.';
  }
  dots[p_len] = '\0';

  // string that will contain the ent phrase with sufficient memory
  char* ent = NULL;
  ent = malloc(sizeof(char) * (t_len * (p_len + 1) + 1));

  // checking for malloc error
  if (ent == NULL) {
    printf("Bad malloc error\n");
    exit(0);
  }

  // forming the ent phrase by putting in letter and the dots
  for (int j = 0; j < t_len; j++) {
    ent[j * (p_len + 1)] = buff[j];
    strcat(ent, dots);
  }
  ent[t_len * (p_len + 1)] = '\0';

  // printing the ent phrase
  printf("%s\n", ent);

  // freeing all the pointers to avoid memory leak
  free(dots);
  dots = NULL;

  free(ent);
  ent = NULL;

  return 0;
}

