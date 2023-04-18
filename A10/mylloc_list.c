/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 04/16/2023
 * Description: This program has an own implementation of malloc() and 
* free() function
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// struct to store information about requested memory  
struct chunk {
  int size; // size of requested memory
  int used; // size of memory currently used
  struct chunk *next; // pointer to the next freed memory
};

struct chunk *flist = NULL; // head node of linked list of freed memories

/**
 * implementation of malloc that uses first-fit strategy
 *
 * @param size: size of requested memory
 * return: pointer to the allocated memory
 *
*/
void* malloc (size_t size) {

  // does not allocate any memory if size is 0
  if (size == 0) {
    return NULL;
  }

  struct chunk* next = flist; // current node of freed memory list
  struct chunk* prev = NULL; // previous node of freed memory list
 
  // looping through each node of the list
  while (next != NULL) {

    // if the current node of freed memory has adequate size
    if (next->size >= size) {

      // remove the current node from the list
      if (prev != NULL) {
        prev->next = next->next;

      } else {
        flist = next->next;
      }

      // update the current use size of the memory
      next->used = size;

      // return the pointer to allocated memory
      return (void*) (next + 1);

    // if current node is not adequate, move to next node
    } else {
      prev = next;
      next = next->next;
    }
  }

  // if there is no freed memory that is adequate for requested memory
  // requests needed memory from OS
  void* memory = sbrk(size + sizeof(struct chunk));

  // error in requesting memory from OS
  if (memory == (void*) -1) {
    return NULL;

  // create chunk header and update values of chunk
  } else {
    struct chunk* cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->used = size;
    return (void*) (cnk + 1);
  }
}

/**
 * implementation of free that uses first-fit strategy
 *
 * @param memory: pointer to the memory to be freed
 * return: nothing
 *
*/
void free(void* memory) {

  // memory is freed only if the pointer points to something
  if (memory != NULL) {

    // accessing the header of the memory hidden from users
    struct chunk* cnk = (struct chunk*) ((struct chunk*) memory - 1);

    // moving the memory to the front of the freed memory linked list
    cnk -> next = flist;
    flist = cnk;
    
    // updating the use of the memory to 0
    cnk ->used = 0;

  }
  return;
}
