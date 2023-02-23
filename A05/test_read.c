/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/22/2023
 * Description: this program reads a PPM file and prints out all the 
 * color values of each pixel of image
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h; // width and height of image

  // getting the 2D array containing all the pixel color values, height and width
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

  // checking for any error during the reading of the PPM file
  if (pixels == NULL) {
    return 0;
  }

  // printing height and width of the image
  printf("Testing file feep-raw.ppm: %i %i\n", h, w);

  // printing the color values of each pixel
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      printf("(%u,%u,%u)  ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
    printf("\n");
  }

  // freeing all the heap memory
  for (int i = 0; i < h; i++) {
    free(pixels[i]);
  }
  free(pixels);
  return 0;
}

