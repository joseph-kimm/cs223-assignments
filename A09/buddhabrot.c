/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 04/08/2023
 * Description: This program outputs a PPM image of the buddhabrot set using
 * 4 threads.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>
#include <math.h>

pthread_mutex_t mutex;
pthread_barrier_t barrier;

// struct to enter all the data into a thread
struct thread_input {
  int min_col; // min column
  int max_col; // max column
  int min_row; // min row
  int max_row; // max row
  struct ppm_pixel** pixel; // pointer to all the pixels of image
  int** membership; // pointer to the membership 2D array
  int** count; // pointer to the visited count 2D array
  int size; // size of image
  float xmin; // leftmost coordinate
  float xmax; // rightmost coordinate
  float ymin; // top most coordinate
  float ymax; // bottom most coordinate
  int maxIterations; // number of iterations
};


// function where each thread would start from 
void* fill(void* foo) {

  // casting the pointer
  struct thread_input* input = (struct thread_input*) foo;

  // printing the rows and columns each thread will create
  printf("Thread %lu) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", pthread_self(), 
    input->min_col, input->max_col, input->min_row, input->max_row);
  
  // Step 1: determine membership

  // looping for each assigned row 
  for (int i = input->min_row; i < input->max_row; i++) {

    // looping for each assigned column
    for (int j = input->min_col; j < input->max_col; j++) {

      // steps to determine mandelbrot set membership
      float xfrac = (float)i / input->size;
      float yfrac = (float)j / input->size;
      float x0 = input->xmin + xfrac * (input->xmax - input->xmin);
      float y0 = input->ymin + yfrac * (input->ymax - input->ymin);

      float x = 0;
      float y = 0;
      int iter = 0;

      while (iter < input->maxIterations && (x*x + y*y < 2*2)) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      // if it escaped the bounds
      if (iter < input->maxIterations) {
        input->membership[i][j] = 0;
      }

      // if it was not able to escape the bounds
      else {
        input->membership[i][j] = 1;
      }
    }
  }

  // Step 2: Compute visited counts

  // for storing the max count
  int max_count = 0;

  // looping for each assigned row 
  for (int i = input->min_row; i < input->max_row; i++) {

    // looping for each assigned column
    for (int j = input->min_col; j < input->max_col; j++) {

      if (input->membership[i][j] == 1) {continue;}

      float xfrac = (float)i / input->size;
      float yfrac = (float)j / input->size;
      float x0 = input->xmin + xfrac * (input->xmax - input->xmin);
      float y0 = input->ymin + yfrac * (input->ymax - input->ymin);

      float x = 0;
      float y = 0;

      while(x*x + y*y < 2*2) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;

        int yrow = round(input->size * (y - input->ymin) / (input->ymax - input->ymin));
        int xcol = round(input->size * (x - input->xmin) / (input->xmax - input->xmin));
        
        // check if yrow and xcol are out of bounds
        if (yrow < 0 || yrow >= input->size) {continue;}
        if (xcol < 0 || xcol >= input->size) {continue;}

        // increment count at the speicifc position using mutex
        pthread_mutex_lock(&mutex);
        input->count[yrow][xcol] += 1;
        pthread_mutex_unlock(&mutex);

        // update max count
        if (input->count[yrow][xcol] > max_count) {
          max_count = input->count[yrow][xcol];
        }
      }

    }
  }

  // wating for all threads to finish step 1 and 2
  pthread_barrier_wait(&barrier);

  // Step 3: Compute colors

  float gamma = 0.681;
  float factor = 1.0/gamma;

  // looping for each assigned row 
  for (int i = input->min_row; i < input->max_row; i++) {

    // looping for each assigned column
    for (int j = input->min_col; j < input->max_col; j++) {
    
      float value = 0;

      if (input->count[i][j] > 0) {
        value = log(input->count[i][j]) / log(max_count);
        value = pow(value, factor);
      }

      input->pixel[i][j].red = value*255;
      input->pixel[i][j].green = value*255;
      input->pixel[i][j].blue = value*255;
    }
  }

  // signalling the end of a thread
  printf("Thread %lu) finished\n", pthread_self());
  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480; // image width and height
  float xmin = -2.0; // leftmost coordinate
  float xmax = 0.47; // rightmost coordinate
  float ymin = -1.12; //topmost coordinate
  float ymax = 1.12; // bottommost coordinate
  int maxIterations = 1000; // number of iterations
  int numProcesses = 4; // number of threads to compute mandelbrot

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

  // variables required to calculate the time taken
  double timer;
  struct timeval tstart, tend;

  // time when the making the mandelbrot set began
  gettimeofday(&tstart, NULL);

  // initializing mutex and barrier
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, 4);

  // creating an array of pointers for the buddhabrot
  struct ppm_pixel** pixel = malloc(size * sizeof(struct ppm_pixel*));

  // checking for malloc error
  if (pixel == NULL) {
    exit(0);
  }

  // looping through each row of the imagae
  for (int i = 0; i < size; i++) {

    // initializng a row of the PPM file
    pixel[i] = malloc(sizeof(struct ppm_pixel) * size);

    // checking for malloc error
    if (pixel[i] == NULL) {
      exit(0);
    }
  }

  // creating an array of pointers for membership
  int** membership = malloc(sizeof(int*) * size);

  // checking for malloc error
  if (membership == NULL) {
    exit(0);
  }

  // looping through each row
  for (int i = 0; i < size; i++) {

    // initializing each row of the membership array
    membership[i] = malloc(sizeof(int) * size);

    // checking for malloc error
    if (membership[i] == NULL) {
      exit(0);
    }
  }

  // creating an array of pointers for visited count
  int** count = malloc(sizeof(int*) * size);

  // checking for malloc error
  if (count == NULL) {
    exit(0);
  }

  // looping through each row
  for (int i = 0; i < size; i++) {

    // initializing each row of the visited count array
    count[i] = malloc(sizeof(int) * size);

    // checking for malloc error
    if (count[i] == NULL) {
      exit(0);
    }

    // making each count value in 2D array 0
    for (int j = 0; j < size; j++) {
      count[i][j] = 0;
    }
  }

  // creating 4 threads and 4 structs for the thread
  pthread_t threads[4];
  struct thread_input inputs[4];

  // looping for each thread
  for (int t = 0; t < numProcesses; t++) {

    // initializing all the data in the struct
    inputs[t].pixel = pixel;
    inputs[t].membership = membership;
    inputs[t].count = count;
    inputs[t].size = size;
    inputs[t].xmin = xmin;
    inputs[t].xmax = xmax;
    inputs[t].ymin = ymin;
    inputs[t].ymax = ymax;
    inputs[t].maxIterations = maxIterations;

    // if its thread 0 or thread 2, columns (0, 240)
    if (t % 2 == 0) {
        inputs[t].min_col = 0;
        inputs[t].max_col = size/2;
    }

    // if its thread 1 or thread 3, columns (240, 480)
    else {
      inputs[t].min_col = size/2;
      inputs[t].max_col = size;
    }

    // if its thread 0 or thread 1, rows (0, 240)
    if (t < 2) {
      inputs[t].min_row = 0;
      inputs[t].max_row = size/2;
    }

    // if its thread 2 or thread 3, rows (240, 480)
    else {
      inputs[t].min_row = size/2;
      inputs[t].max_row = size;
    }

    // creates a thread
    pthread_create(&threads[t], NULL, fill, &inputs[t]);
  }

  // wating for all the threads to join
  for (int t = 0; t < numProcesses; t++) {
    pthread_join(threads[t], NULL);
  }

  // time when making mandelbrot set ended
  gettimeofday(&tend, NULL);

  // calculating and printing the time it took to make the set
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%ix%i) in %f seconds\n", size, size, timer);

  // file name to save the mandelbrot set
  char filename[128];
  strcpy(filename, "buddhabrot-");

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

  // freeing all the heap memory
  for (int i = 0; i < size; i++) {
    free(pixel[i]);
    free(membership[i]);
    free(count[i]);
  }

  free(pixel);
  free(membership);
  free(count);

  return 0;
}