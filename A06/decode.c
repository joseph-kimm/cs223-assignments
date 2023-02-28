/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/28/2023
 * Description: this program reads a PPM file, finds the hidden message
 * stored in the colors and then outputs it.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {

  // case when the file name was not given
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  // the width and the height of the image
  int w;
  int h;

  // 2D array that contains all the color values in each pixel of the image
  struct ppm_pixel** pixel = read_ppm_2d(argv[1], &w, &h);
  printf("Reading %s with width %i and height %i\n", argv[1], w, h);

  // calculating the number of characters that could be hidden in the image
  int max = (w*h*3) / 8;
  printf("Max number of characters in the image: %i\n", max);

  // string to store the hidden message
  char* hidden = malloc(sizeof(char) * (max+2));

  int index = 0; // index of the string
  int count = 0; // the bit count of each character
  unsigned int mod = 0; // the least significant bit of each color

  // repeats for all the pixels in the image
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {

      // finding the least significant bit of color red
      mod = pixel[i][j].red % 2;

      // adding the least significant bit to the current character
      hidden[index] = hidden[index] << 1;
      hidden[index] += mod;
      count++;

      // checking if we have to move to the next character
      if (count == 8) {
        index++;
        count = 0;
      }

      // finding the least significant bit of color green
      mod = pixel[i][j].green % 2;

      // adding the least significant bit to the current character
      hidden[index] = hidden[index] << 1;
      hidden[index] += mod;
      count++;

      // checking if we have to move to the next character
      if (count == 8) {
        index++;
        count = 0;
      }

      // finding the least significant bit of the color blue 
      mod = pixel[i][j].blue % 2;

      // adding the least significant bit to the current character
      hidden[index] = hidden[index] << 1;
      hidden[index] += mod;
      count++;

      // checking if we have to move to the next character
      if (count == 8) {
        index++;
        count = 0;
      }
    }
  }

  // outputing the hidden message
  printf("%s\n", hidden);

  // freeing all the heap memory
  free(hidden);

  for (int i = 0; i < h; i++) {
    free(pixel[i]);
  }
  free(pixel);

  return 0;
}

