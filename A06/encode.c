/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/28/2023
 * Description: this program reads a PPM file, hides a message from user
 * in the image and then puts the encoded image in a new PPM file
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {

  // case where the filename was not given to the program
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  // width and height of the image
  int w;
  int h;

  // 2D array containing all the colors values of each pixel of image
  struct ppm_pixel** pixel = read_ppm_2d(argv[1], &w, &h);
  printf("Reading %s with width %i and height %i\n", argv[1], w, h);

  // finding the max number of characters that can be hidden in the image
  int max = (w*h*3) / 8;
  printf("Max number of characters in the image: %i\n", max);

  // receiving a phrase from the user to hide
  char* hidden = malloc(sizeof(char) * max);
  printf("Enter a phrase: ");
  scanf("%s", hidden);

  int count = 7; // the bit position in each character
  unsigned char mask = 0x01; // the mask to find the bit value 
  unsigned char bit; // the bit value of each position
  int index = 0; // the index of the string

  // loops for each pixel of the image
  for (int i = 0; i < h; i++) {
    for (int j = 0; j <w; j++) {

      // finding the bit value of current bit position of current character
      bit = hidden[index] & (mask << count);
      bit = bit >> count;
      count --;

      // checks if the bit value is equal to the least significant bit of red color
      if (bit != pixel[i][j].red % 2) {

        // if bit value different and least significant bit is even, add 1
        if (pixel[i][j].red % 2 == 0) {
          pixel[i][j].red ++;
        }

        // if bit value is different and least significant bit is odd, minus 1
        else {
          pixel[i][j].red --;
        }
      }

      // checks if we have to move to the next character
      if (count < 0) {
        count = 7;
        index++;
      }

      // finding the bit value of the current bit position of current character
      bit = hidden[index] & (mask << count);
      bit = bit >> count;
      count --;

      // checks if the bit value is equal to the least significant bit of green color
      if (bit != pixel[i][j].green % 2) {

        // if bit value different and least significant bit is even, add 1
        if (pixel[i][j].green % 2 == 0) {
          pixel[i][j].green ++;
        }

        // if bit value different and least significant bit is odd, minus 1
        else {
          pixel[i][j].green --;
        }
      }

      // checking if we have to move to the next character
      if (count < 0) {
        count = 7;
        index++;
      }

      // finding the bit value of the current bit position of current character
      bit = hidden[index] & (mask << count);
      bit = bit >> count;
      count --;

      // checks if the bit value is equal to the least significant bit of blue color
      if (bit != pixel[i][j].blue % 2) {

        // if bit value is different and least significant bit is even, add 1
        if (pixel[i][j].blue % 2 == 0) {
          pixel[i][j].blue ++;
        }

        // if bit value different and least significant bit is odd, minus 1
        else {
          pixel[i][j].blue --;
        }
      }

      // checking if we have to move to the next character
      if (count < 0) {
        count = 7;
        index++;
      }
    }
  }

  // write the new image to the new file
  write_ppm_2d(argv[1], pixel, w, h);

  // freeing all the heap memory
  free(hidden);

  for (int i = 0; i < h; i++) {
    free(pixel[i]);
  }
  free(pixel);

  return 0;
}

