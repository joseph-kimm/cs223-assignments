/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 03/28/2023
 * Description: This program outputs a PPM image of the mandelbrot set using 4 child processes.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>

int main(int argc, char* argv[]) {
  int size = 480; // image width and height
  float xmin = -2.0; // leftmost coordinate
  float xmax = 0.47; // rightmost coordinate
  float ymin = -1.12; //topmost coordinate
  float ymax = 1.12; // bottommost coordinate
  int maxIterations = 1000; // number of iterations
  int numProcesses = 4; // number of child processes to compute mandelbrot

  // option of visualizing other regions of the mandelbrot set
  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }

  // outputing the size, the x range, and the y range of the PPM file
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // using time to generate random numbers
  time_t t;
  srand((unsigned) time(&t));

  // palette that contains random numbers
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);

  // checking for malloc error
  if (palette == NULL) {
    exit(0);
  }

  // a black pixel with corresponding red, green, and blue values
  struct ppm_pixel black;
  black.red = 0;
  black.green = 0;
  black.blue = 0;

  // giving a random color for all the palette colors
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // variables required to calculate the time taken
  double timer;
  struct timeval tstart, tend;

  // time when the making the mandelbrot set began
  gettimeofday(&tstart, NULL);

  // creating a shared memory and checking for error
  int shmid;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size * size, 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  // accessing shared memory and checking for error
  struct ppm_pixel* data = shmat(shmid, NULL, 0);
  if (data == (void*) -1) {
    perror("Error:c cannot access shared memory\n");
    exit(1);
  }

  // creating an array of pointers for the shared memory
  struct ppm_pixel** pixel = malloc(size * sizeof(struct ppm_pixel*));

  // checking for malloc error
  if (pixel == NULL) {
    exit(0);
  }

  // setting each pointer in array to a space in shared memory
  for(int i = 0; i < size; i++) {
      pixel[i] = &data[size * i];
  }

  // column and row range for each of the child process
  int min_col;
  int max_col;
  int min_row;
  int max_row;

  // looping for each child process
  for (int p = 0; p < numProcesses; p++) {

    // creates a child process
    int pid = fork();

    // parent process
    if (pid) {
      printf("Launched child process: %i\n", pid);
    }
    
    // child process
    else {

      // if its child 0 or child 2, columns (0, 240)
      if (p % 2 == 0) {
        min_col = 0;
        max_col = size/2;
      }

      // if its child 1 or child 3, columns (240, 480)
      else {
        min_col = size/2;
        max_col = size;
      }

      // if its child 0 or child 1, rows (0, 240)
      if (p < 2) {
        min_row = 0;
        max_row = size/2;
      }

      // if its child 2 or child 3, rows (240, 480)
      else {
        min_row = size/2;
        max_row = size;
      }

      // printing the rows and columns each child will created
      printf("%i) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", getpid(), 
        min_col, max_col, min_row, max_row);
      
      // looping for each assigned row 
      for (int i = min_row; i < max_row; i++) {

        // looping for each assigned column
        for (int j = min_col; j < max_col; j++) {

          // steps to calculate the color of each pixel of the mandelbrot set
          float xfrac = (float)i / size;
          float yfrac = (float)j / size;
          float x0 = xmin + xfrac * (xmax - xmin);
          float y0 = ymin + yfrac * (ymax - ymin);

          float x = 0;
          float y = 0;
          int iter = 0;

          while (iter < maxIterations && (x*x + y*y < 2*2)) {
            float xtmp = x*x - y*y + x0;
            y = 2*x*y + y0;
            x = xtmp;
            iter++;
          }

          // if it escaped the bounds
          if (iter < maxIterations) {
            pixel[i][j] = palette[iter];
          }

          // if it was not able to escape the bounds
          else {
            pixel[i][j] = black;
          }
        }
      }

      // freeing all the heap memory for each child process
      free(palette);
      free(pixel);
      exit(0);

    }
  }

  // wating for all the child processes to be finished
  for (int p = 0; p < numProcesses; p++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }

  // time when making mandelbrot set ended
  gettimeofday(&tend, NULL);

  // calculating and printing the time it took to make the set
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%ix%i) in %f seconds\n", size, size, timer);

  // file name to save the mandelbrot set
  char filename[128];
  strcpy(filename, "multi-mandelbrot-");

  // adding the size of the image to the name
  char strsize[10];
  sprintf(strsize, "%d", size);
  strcat(filename, strsize);
  strcat(filename, "-");
  
  // adding the timestamp to the name
  time_t curr = time(0);
  char strtime[15];
  sprintf(strtime, "%ld", curr);
  strcat(filename, strtime);

  // adding the file extension to the name
  strcat(filename, ".ppm");

  // printing the file name
  printf("Writing file: %s\n", filename);

  // writing the PPM file
  write_ppm_2d(filename, pixel, size, size);

  // freeing all the heap memory of parent process
  free(palette);
  free(pixel);

  // detaching from shared memory and checking for error
  if (shmdt(data) == -1) {
    perror("Error: cannot detach from shared memory\n");
    exit(1);
  }

  // removing shared memory and checking for error
  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

  return 0;
}