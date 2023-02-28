/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/22/2023
 * Description: this program reads a PPM file and write all the information from the 
 * file to another file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {

}

/**
 * this function reads a file and stores all the information in another file
 *
 * @param filename: file name to be read
 * @param pixels: pointer to 2D array containing all the color values of all pixels
 * @param w: width of image
 * @param h: height of image
 * return: does not return anything
*/
void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

  // new file name with -encoded added to the file name
  char* newfile = malloc(sizeof(char) * strlen(filename) + 10);

  // adding the file name to the new file name without the .ppm
  int fname_length = strlen(filename) - strlen(strstr(filename, ".ppm"));
  strncpy(newfile, filename, fname_length);
  newfile[fname_length] = '\0';

  // adding the -encoded.ppm to the file name
  strcat(newfile, "-encoded.ppm\0");

  // outputing the file name that is being created
  printf("Writing file %s\n", newfile);

  // reads both the file to read and write
  FILE* infp = fopen(filename, "rb");
  FILE* fp = fopen(newfile, "wb");

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

  // freeing mallocs and closing both file pointer
  free(newfile);
  fclose(fp);
  fclose(infp);
  return;

}
