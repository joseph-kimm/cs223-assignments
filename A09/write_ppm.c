/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 03/21/2023
 * Description: This program has a function that receives a 2D array
 * containing all the pixels of an image and writes a PPM file.
 ---------------------------------------------*/


#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

/**
 * this function writes a PPM file.
 *
 * @param filename: file name to be written
 * @param pixels: pointer to 2D array containing all the color values of all pixels
 * @param w: width of image
 * @param h: height of image
 * return: does not return anything
*/
void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

  // creates a file pointer to the file to be written 
  FILE* fp = fopen(filename, "wb");

  // checking for malloc error
  if (fp == NULL) {
    return;
  }

  // writes the header of the PPM file
  fprintf(fp, "%s\n", "P6");
  fprintf(fp, "%s\n", "# Created by GIMP version 2.10.24 PNM plug-in");

  // writes the width and height of the image
  fprintf(fp, "%i %i\n", w, h);
  fprintf(fp, "%i\n", 255);

  // copying all the color values of each row of pixel
  for (int i = 0; i < h; i++) {
    fwrite(pixels[i], sizeof(struct ppm_pixel), w, fp);
  }

  // freeing malloc and closing file pointer
  fclose(fp);
  return;
}

