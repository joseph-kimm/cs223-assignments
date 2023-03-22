/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 03/21/2023
 * Description: This program outputs a PPM image of the mandelbrot set.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480; // image width and height
  float xmin = -2.0; // leftmost coordinate
  float xmax = 0.47; // rightmost coordinate
  float ymin = -1.12; //topmost coordinate
  float ymax = 1.12; // bottommost coordinate
  int maxIterations = 1000; // number of iterations

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

  // variable that contains all the pixels of the PPM file
  struct ppm_pixel** pixel = malloc(sizeof(struct ppm_pixel*) * size);

  // checking for malloc error
  if (pixel == NULL) {
    exit(0);
  }

  // looping for each row of the PPM image
  for (int i = 0; i < size; i++) {

    // initializng a row of the PPM file
    pixel[i] = malloc(sizeof(struct ppm_pixel) * size);

    // checking for malloc error
    if (pixel[i] == NULL) {
      exit(0);
    }

    // looping through each column of the PPM image
    for (int j = 0; j < size; j++) {

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

  // time when making mandelbrot set ended
  gettimeofday(&tend, NULL);

  // calculating and printing the time it took to make the set
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%ix%i) in %f seconds\n", size, size, timer);

  // file name to save the mandelbrot set
  char filename[128];
  strcpy(filename, "mandelbrot-");

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
  free(palette);

  for (int i = 0; i < size; i++) {
    free(pixel[i]);
  }
  free(pixel);

  return 0;
}
