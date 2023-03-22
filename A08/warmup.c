/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 03/21/2023
 * Description: This program makes the process spawn children processes and prints out 
 * certain things depending on the process
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {

  pid_t pid; // determines whether the process is the parent or the child
  printf("%i] A0\n", getpid());

  // creates a child process
  pid = fork();

  if (pid) { // if parent process
    printf("%i] B0\n", getpid());

    // creates a child process
    pid = fork();

    if (pid) { // if parent process
      printf("%i] C0\n", getpid());
    }

    else { // if child process
      printf("%i] C1\n", getpid());
    }

  } // if child process
  else {
    printf("%i] B1\n", getpid());
  }

  // for all processes
  printf("%i] Bye\n", getpid());
  return 0;
}
