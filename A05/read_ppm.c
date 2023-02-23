/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/22/2023
 * Description: this program contains a function that opens up a PPM file
 * and stores the color values of each pixel in a 2D array
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  return NULL;
}

/**
 * reads a PPM file and stores the color values of each pixel in a 2D array
 *
 * @param filename: file name to be read
 * @param w: pointer that points to an integer storing the width of image
 * @param h: pointer that points to an integer storing the height of image
 * return: a pointer to the 2D array in heap
*/
struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  FILE* fp = fopen(filename, "rb");

  // checking for malloc error
  if (fp == NULL) {
    return NULL;
  }

  char buff[200];

  // skips the first two lines containing information about file
  fgets(buff, 200, fp);
  fgets(buff, 200, fp);

  // gets the height and width of the image
  fgets(buff, 200, fp);
  sscanf(buff, "%i %i", h, w);

  // skips the max value of each color value
  fgets(buff, 200, fp);

  // creating a 2D array
  struct ppm_pixel** point = malloc(sizeof(struct ppm_pixel*) * *h);

  // checking for malloc error
  if (point == NULL) {
    return NULL;
  }

  // creating each row of the 2D array
  for (int i = 0; i < *h; i++) {
    point[i] = malloc(sizeof(struct ppm_pixel) * *w);

    // checking for malloc error
    if (point[i] == NULL) {
      return NULL;
    }

    // taking each row of pixel color values
    fread(point[i], sizeof(struct ppm_pixel), *w, fp);
  }

  // closing file and returning the pointer
  fclose(fp);
  return point;
}
