/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 04/03/2023
 * Description: This program reads a binary file of integers and finds a specific 
 * value in the binary file by dividing the work between N threads specified by user
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// struct to pass all necessary data into a thread
struct thread_input {
  int start_index; // start index of search
  int end_index; // end index of search
  int* array; // array containing all the data
  int value; // value to find
  int found; // index of value in data
};

// function that thread will run
void* find(void *foo) {

  // casting to appropriate pointer
  struct thread_input* input = (struct thread_input *) foo;

  // looping through all the given indices
  for (int i = input->start_index; i < input->end_index; i++) {

    // if value found, returns the index of the value within the struct
    if (input->array[i] == input->value) {
      input->found = i;
      return NULL;
    } 
  }

  // if value is not found, return -1 as index of value
  input->found = -1;
  return NULL;
} 

int main(int argc, char** argv) {

  // checks if appropriate arguments were passed
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

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
  int val;
  scanf("%i", &val);

  // converting the number of threads to int
  int nums = atoi(argv[1]);

  // calculating size to be handled by each thread
  int subsize = size/nums;

  // creating number of threads and structs as needed
  pthread_t* threads = malloc(sizeof(pthread_t) * nums);
  struct thread_input* inputs = malloc(sizeof(struct thread_input) * nums);

  // looping through each thread
  for (int i = 0; i < nums; i++) {

    // initializing the data inside the struct
    inputs[i].array = data;
    inputs[i].value = val;
    inputs[i].start_index = subsize * i;
    inputs[i].end_index = subsize * (i+1);

    // spawning a thread with appropriate function and data input
    pthread_create(&threads[i], NULL, find, &inputs[i]);
  }

  // reaping all the threads
  for (int i = 0; i < nums; i++) {
    pthread_join(threads[i], NULL);

    // checking if the value was found in a specific thread
    if (inputs[i].found >= 0) {

      // if found, return the index and which thread found the value
      printf("Thread %i found %i at index %i\n", i+1, val, inputs[i].found);

      // freeing all the heap memory and ending the program
      free(data);
      free(threads);
      free(inputs);
      fclose(fp);
      return 0;
    }
  }

  // if not found, output accordingly
  printf("Not found!\n");

  // freeing all the heap memory and ending the program
  free(data);
  free(threads);
  free(inputs);
  fclose(fp);
  return 0;
}
