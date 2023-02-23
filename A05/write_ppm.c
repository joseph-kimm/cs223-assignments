/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/22/2023
 * Description: this program reads a PPM file and write all the information from the 
 * file to another file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {

}

/**
 * this function reads a file and stores all the information in another file
 *
 * @param filename: file name to be created
 * @param pixels: pointer to 2D array containing all the color values of all pixels
 * @param w: width of image
 * @param h: height of image
 * return: does not return anything
*/
void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

  // reads both the file to read and write
  FILE* infp = fopen("feep-raw.ppm", "rb");
  FILE* fp = fopen(filename, "wb");

  // checking for malloc error
  if (infp == NULL || fp == NULL) {
    return;
  }

  char buff[200];

  // copying the headers of the files
  fgets(buff, 200, infp);
  fprintf(fp, "%s", buff);
  fgets(buff, 200, infp);
  fprintf(fp, "%s", buff);
  fgets(buff, 200, infp);
  fprintf(fp, "%s", buff);
  fgets(buff, 200, infp);
  fprintf(fp, "%s", buff);

  // copying all the color values of each row of pixel
  for (int i = 0; i < h; i++) {
    fwrite(pixels[i], sizeof(struct ppm_pixel), w, fp);
  }

  // closing both file pointer
  fclose(fp);
  fclose(infp);
  return;

}
