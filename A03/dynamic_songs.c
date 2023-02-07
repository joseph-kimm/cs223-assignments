/*----------------------------------------------
 * Author: Joseph Kim 
 * Date: Feb 7th, 2023
 * Description: This program reads a csv file containing information of songs
 * and then stores them in an array. It then prints out all the information.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// struct for saving all the informaiton of a song
struct song {
  char title[128];
  char artist[128];
  int duration;
  float dance;
  float energy;
  float tempo;
  float valence;
};

// forward declaration of functions
struct song split(char* line);
void playlistInfo(struct song playlist[], int size); 

int main() {
  printf("Welcome to Dynamic Donna's Danceability Directory.\n");

  FILE* infile; // file pointer
  int buff_len = 300; // size of the below string
  char* buff; // string that will contain each line of the file
  buff = malloc((sizeof(char) * buff_len));

  // checking for malloc error
  if (buff == NULL) {
    exit(0);
  }

  // accessing the csv file containing all the songs
  infile = fopen("songlist.csv", "r");

  // checking for malloc error
  if (infile == NULL) {
    exit(0);
  }

  // getting the number of songs in the csv file
  fgets(buff, buff_len, infile);
  int size = atoi(strtok(buff, ","));

  // to skip the line containing headers
  fgets(buff, buff_len, infile);

  // array of struct song that will store all the information
  struct song* playlist;
  playlist = malloc(sizeof(struct song) * size);

  // checking for malloc error
  if (playlist == NULL) {
    exit(0);
  }

  // takes in each line of the file and then stores it in an array of struct song
  for (int i = 0; i < size; i++) {
    fgets(buff, buff_len, infile);
    playlist[i] = split(buff);
  }

  // prints out all the information of the songs
  playlistInfo(playlist, size);

  // closing/freeing pointers to prevent memory leak
  fclose(infile);
  infile = NULL;

  free(buff);
  buff = NULL;

  free(playlist);
  playlist = NULL;

  return 0;
}

/**
 * takes in a string that contains information of a song and puts
 * them in the correct fields of a struct song
 *
 * @param buff: string that contains all the info
 * @return: struct song containing all the info
 *
 */
struct song split(char* buff) {
  struct song temp; // creates a temporary struct

  // puts all the info into appropriate fields, converting the string
  // into integer or float if necessary
  strcpy(temp.title, strtok(buff, ","));
  strcpy(temp.artist, strtok(NULL, ","));
  temp.duration = atoi(strtok(NULL, ","));
  temp.dance = atof(strtok(NULL, ","));
  temp.energy = atof(strtok(NULL, ","));
  temp.tempo = atof(strtok(NULL, ","));
  temp.valence = atof(strtok(NULL, ","));
  return temp;
}

/**
 * prints out all the songs in an array in a clean way
 *
 * @param playlist[]: array that holds all the songs
 * @param size: int holding size of array
 * @return: does not return anything
 *
 */
void playlistInfo(struct song playlist[], int size) {
  printf("\n");

  // loops through each of the songs in the array
  for (int i = 0; i < size; i++) {

    // converts the duration of song from milliseconds to minutes:seconds
    int seconds = playlist[i].duration / 1000;
    int minutes = 0;
    while (seconds >= 60) {
      minutes++;
      seconds -= 60;
    }

    // checks if the current song index is greater than 10 for clean formatting
    if (i >= 10) {
      printf("%i)  %-24s ", i, playlist[i].title);
    }
    else {
      printf("%i)  %-25s ", i, playlist[i].title);
    }

    // prints rest of the information of a song
    printf("artist: %-25s duration: %i:%02d  D: %.3f",
      playlist[i].artist, minutes, seconds, playlist[i].dance);

    printf("  E: %.3f  T: %.3f  V: %.3f\n", playlist[i].energy,
      playlist[i].tempo, playlist[i].valence);
  }
}
