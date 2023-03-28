/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 03/28/2023
 * Description: This program reads a binary file of integers and finds a specific 
 * value in the binary file by dividing the work between the parent and the child.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main() {

  // accessing the file
  FILE* fp = fopen("data.bin", "rb");

  // checking for malloc error
  if (fp == NULL) {
    exit(0);
  }

  // the number of integers in the file
  int size = getw(fp);

  // creating an array of proper size
  int* data = malloc(sizeof(int) * size);

  // checking for malloc error
  if (data == NULL) {
    exit(0);
  }

  // entering all the data into the array
  fread(data, sizeof(int), size, fp);
  
  // receiving a value to search from the user
  printf("Enter a value to search: ");
  int value;
  scanf("%i", &value);

  // splitting the array size into half and then creating a child process
  int half = size/2;
  pid_t pid;
  pid = fork();

  // parent process
  if (pid) {

    // loops from the start of the array to the middle 
    int i;
    for (i = 0; i < half; i++) {

      // if value is found 
      if (value == data[i]) {
        printf("Parent found %i at index %i\n", value, i);
        break;
      }
    }

    //if value is not found
    if (i == half) {
      printf("Not found!\n");
    }
  }

  // child process
  else {

    // loops from the middle of the array to the end
    int i;
    for (i = half; i < size; i++) {

      // if value is found
      if (value == data[i]) {
        printf("Child found %i at index %i\n", value, i);
        break;
      }
    }

    // if value is not found
    if (i == size) {
      printf("Not found!\n");
    }
  }

  // freeing all the heap memory
  free(data);
  fclose(fp);
  return 0;
}
