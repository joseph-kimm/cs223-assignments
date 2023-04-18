#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

// struct to store information about requested memory
struct chunk {
  int size;
  int used;
  struct chunk *next;
};

/**
 * used to calculate size and status of requested memory blocks
 *
 * @param freelist: header of freed memory linked list
 * @param buffer[]: array of pointers that lead to NULL or used memory
 * @param len: size of buffer[]
 * return: nothing
 *
*/
void memstats(struct chunk* freelist, void* buffer[], int len) {

  int free_block = 0; // number of freed memory blocks
  int used_block = 0; // number of used memory blocks

  int free_memory = 0; // memory freed
  int used_memory = 0;  // memory in buffer
  int underused_memory = 0;// memory in buffer but not used

  struct chunk* curr = freelist;

  // looping through each of node of the linked list
  while (curr != NULL) {

    // updating the memory and block count
    free_block++;
    free_memory += curr->size;

    curr = curr->next;
  }

  // looping through each pointer in array
  for (int i = 0; i < len; i++) {

    // checking if pointer points to used memory
    if (buffer[i] != NULL) {

      // used to get information about the current memory block
      struct chunk* curr = (struct chunk*) ((struct chunk*) buffer[i] - 1);

      // updating memory and block count
      used_block ++;
      used_memory += curr->size;
      underused_memory += curr->size - curr->used;
    } 
  }

  // printing all the information about memory blocks and memory
  printf("Total blocks: %i Free blocks: %i Used blocks: %i\n", free_block + used_block, 
    free_block, used_block);

  printf("Total memory allocated: %i Free memory: %i Used memory: %i\n", free_memory + used_memory, 
    free_memory, used_memory);

  printf("Underutlized memory: %.2f\n", (float) underused_memory / used_memory);

  return;
}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
