/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/22/2023
 * Description: this program reads a file, copies all the information to another file
 * and prints all the pixel values of the new file
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h; // height and width of image

  // getting 2D array containing all the color values, height, and width
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

  // writing all the information to a new file
  write_ppm_2d("test.ppm", pixels, w, h);

  // freeing all the heap memory
  for (int i = 0; i < h; i++) {
    free(pixels[i]);
  }
  free(pixels);

  // getting 2D array, height, and width from the new file
  pixels = read_ppm_2d("test.ppm", &w, &h);

  printf("Testing file feep-raw.ppm: %i %i\n", h, w);

  // printing all the information of the image in the new file
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
