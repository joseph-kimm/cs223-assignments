/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/28/2023
 * Description: This program takes in a single 64-bit unsigned long and 
 * outputs it as an 8x8 1bpp sprite
 ---------------------------------------------*/
#include <stdio.h>

int main() {

  // taking in an unsigned long from a text file 
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here

  // repeating for each bit of the unsigned long starting from left
  for (int i = 63; i >= 0; i--) {

    // using a mask and bit position to find the value of the
    // bit at each position
    unsigned long mask = 0x1ul << i;
    unsigned long pixel = img & mask;
    pixel = pixel >> i;

    // case 1: bit is one and the pixel is black
    if (pixel == 1) {
      printf("@ ");
    }

    // case 2: bit is zero and the pixel is white
    else {
      printf("  ");
    }

    // every 8 bits/pixels, move to a newline
    if (i%8 == 0) {
      printf("\n");
    }
  }

  printf("____\n");
  return 0;
}

