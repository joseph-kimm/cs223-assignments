/*
* songs.c
*
* Author: Joseph Kim
* Version: February 1, 2023
*
* This program has a array that stores information about 3 songs
* and allows the users to edit information of one of the songs
*
*/

#include <stdio.h>
#include <string.h>

// struct for saving all the information of a song
struct song {
  int duration; //duration of song in seconds
  float dance; //danceability of song
  char title[32]; //title of song
  char artist[32]; //artist of the song
};

// forward declaration of function
void playlistInfo(struct song playlist[], int size);

int main() {

  // three songs that will go in the playlist array
  struct song song1 = {281, 0.7, "Thinking out Loud", "Ed Sheeran"};
  struct song song2 = {165, 0.5, "Wonder", "Shawn Mendes"};
  struct song song3 = {329, 0.4, "All Too Well", "Taylor Swift"};

  // array that stores the three songs and int holding its size
  struct song  playlist[10] = {song1, song2, song3};
  int size = 3;

  // welcomes users and prints all the songs
  printf("Welcome to Steven Struct's Song List.\n");
  playlistInfo(playlist, size);

  // recieves the song id that user wants to change
  printf("\nEnter a song id to edit [0, 1, 2]: ");
  int response;
  scanf("%i", &response);

  // if song id is invalid, prints error message
  if (response >= size) {
    printf("Invalid choice!\n");
  }

  // if valid, asks which attribute they want to change
  else {
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    char attr[20];
    scanf("%s", attr);

    // case 1: changes the artist for the song
    if (strcmp(attr, "artist") == 0) {
      printf("Enter an artist: ");
      char artist_r[32];
      scanf(" %[^\n]%*c", artist_r);
      strcpy(playlist[response].artist, artist_r);
    }

    // case 2: changes the title of the song
    else if (strcmp(attr, "title") == 0) {
      printf("Enter a title: ");
      char title_r[32] = "";
      scanf(" %[^\n]%*c", title_r);
      strcpy(playlist[response].title, title_r);
    }

    // case 3: changes the duration of the song
    else if (strcmp(attr, "duration") == 0) {

      // takes the minutes of the song
      printf("Enter a duration (minutes): ");
      int duration_min;
      scanf("%i", &duration_min);

      // takes the seconds of the song
      printf("Enter a duration (seconds): ");
      int duration_sec;
      scanf("%i", &duration_sec);

      // converts minutes to seconds
      while(duration_min > 0) {
        duration_sec += 60;
        duration_min --;
      }

      playlist[response].duration = duration_sec;
    }

    // case 4: changes the danceability of the song
    else if (strcmp(attr, "danceability") == 0) {
      printf("Enter danceability: ");
      float dance_r;
      scanf("%f", &dance_r);
      playlist[response].dance = dance_r;
    }

    // shows all the songs including the changed one
    playlistInfo(playlist, size);
  }
  return 0;
}

/**
* prints all the songs in an array in a clean way
*
* @param playlist[]: the array that holds all the songs
* @param size: int that holds size of array
* @return does not return anything
*
*/
void playlistInfo(struct song playlist[], int size) {
  printf("\n");

  // loops through each of the songs in the array
  for (int i = 0; i < size; i++) {

    // converts the duration of song from seconds to minutes:seconds
    int seconds = playlist[i].duration;
    int minutes = 0;
    while (seconds >= 60) {
      minutes++;
      seconds -= 60;
    }

    // prints all the information of a song
    printf("%i)  %-20s artist: %-20s duration: %i:%02d  danceability: %.2f\n", i, playlist[i].title,
      playlist[i].artist, minutes, seconds, playlist[i].dance);
  }
  printf("\n=======================\n");
}

