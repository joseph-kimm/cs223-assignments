/*----------------------------------------------
 * Author: Joseph Kim
 * Date: 02/14/2023
 * Description: This program reads a csv file containing a list of songs
 * and stores all the information in a linked list. It then prints out the
 * song with the highest danceability if the users asks so.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// struct for saving all the information of a song
struct song {
  char title[128];
  char artist[128];
  int duration;
  float dance;
  float energy;
  float tempo;
  float valence;
};

// struct for the linked list data structure
struct node {
  struct song info;
  struct node* next;
};

// forward declaration of functions
struct song split(char* line); // converts a string into a struct song
void print(struct node* head); // prints out all the conent of the linked list
struct node* insert(struct song s, struct node* head); // inserts a node in front of the list
void clear(struct node* head); // removes all the nodes from the list
struct node*  danceable(struct node* head); // prints out the most danceable song

int main() {
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

  // to skip the line containing headers
  fgets(buff, buff_len, infile);

  // adding all the songs into a linked list
  struct node* head = NULL;
  while(fgets(buff, buff_len, infile) != 0) {
    struct song temp_song = split(buff);
    head = insert(temp_song, head);
  }

  // printing all the list of songs once
  print(head);

  // taking in response from the user
  printf("Press 'd' to show the most danceable song (any other key to quit): ");
  char response;
  scanf(" %c", &response);

  // repeats while the response of the user is 'd'
  while (response == 100) {

    // prints out the most danceable song if the linked list is not empty
    if (head !=NULL) {
      head = danceable(head);
    }
    print(head);

    // asking response from user again
    printf("Press 'd' to show the most danceable song (any other key to quit): ");
    scanf(" %c", &response);
  }

  // if the linked list contains songs, they are all freed and deleted
  if (head != NULL) {
    clear(head);
  }

  // closing/freeing pointers to prevent memory leak
  fclose(infile);
  infile = NULL;

  free(buff);
  buff = NULL;

  return 0;
}


/**
 * takes in the head of the linked list, prints out the most danceable song
 * and removes it from the linked list
 *
 * @param head: head of the linked list
 * @return: returns the new head of the linked list
 *
 */
struct node* danceable(struct node* head) {

  struct node* temp = head; //pointer that points to the current node of linked list
  float max_d = 0; // stores the highest danceability of a song
  char max_title[128]; // title of the song with with highest danceability

  // finds the song with the highest danceability
  // also saves the title of the song to find it from the list later
  while (temp != NULL) {
    if (temp->info.dance > max_d) {
      max_d = temp->info.dance;
      strcpy(max_title, temp->info.title);
    }
    temp = temp->next;
  }

  temp = head;

  // edge case of the song with highest danceability being the head of list
  if (head != NULL && strcmp(head->info.title, max_title) == 0) {

    // converts the duration of song from milliseconds to minutes and seconds
    int seconds = head->info.duration / 1000;
    int minutes = 0;
    while (seconds >= 60) {
      minutes++;
      seconds -= 60;
    }

    // prints out all the content of the song
    printf("\n---------------------------------------- Most danceable ------------------------------------\n");
    printf("%-28s %-20s (%i:%02d)  D: %.3f",
      head->info.title, head->info.artist, minutes, seconds, head->info.dance);

    printf("  E: %.3f  T: %.3f  V: %.3f\n",
      head->info.energy, head->info.tempo, head->info.valence);
    printf("--------------------------------------------------------------------------------------------\n\n");

    // makes the next node the new head
    head = temp->next;

    // removes the old head and then returns the new head
    free(temp);
    return head;
  }

  // other case where the song with highest danceability in the middle of list
  struct node* next;
  while(temp->next != NULL) {
    next = temp->next;

    // checks if the the song in the node is the song with highest danceability
    if (strcmp(next->info.title, max_title) == 0) {

      // converts the duration of song from milliseconds to minutes and seconds
      int seconds = next->info.duration / 1000;
      int minutes = 0;
      while (seconds >= 60) {
        minutes++;
        seconds -= 60;
      }

      // prints out all the content of the song
      printf("\n---------------------------------------- Most danceable ------------------------------------\n");
      printf("%-28s %-20s (%i:%02d)  D: %.3f",
        next->info.title, next->info.artist, minutes, seconds, next->info.dance);

      printf("  E: %.3f  T: %.3f  V: %.3f\n",
        next->info.energy, next->info.tempo, next->info.valence);
      printf("--------------------------------------------------------------------------------------------\n\n");

      // removes the song from the list and stops the loop
      temp->next = next->next;
      free(next);
      break;
    }
    temp = temp->next;
  }

  // returns the head of the list
  return head;
}


/**
 * deletes all the nodes in the linked list
 *
 * @return: does not return anything
 *
 */
void clear(struct node* head) {
  struct node* temp; // temporary storage of the next node
  while (head != NULL) {
    temp = head->next;
    free(head); // frees the space taken by a node in the heap
    head = temp;
  }
}


/**
 * inserts a node in the front of a linked list
 *
 * @param s: a struct song that contains all the information
 * @param head: the head of the linked list
 * @return: the head of the list
 *
 */
struct node* insert(struct song s, struct node* head) {

  //creates a new node to store the struct song
  struct node* n = malloc(sizeof(struct node));

  // checking for malloc error
  if (n == NULL) {
    exit(0);
  }

  // copies all the content of the struct song to the node
  strcpy(n->info.title, s.title);
  strcpy(n->info.artist, s.artist);
  n->info.duration = s.duration;
  n->info.dance = s.dance;
  n->info.energy = s.energy;
  n->info.tempo = s.tempo;
  n->info.valence = s.valence;

  // returns the new node as the new head of the linked list
  n->next = head;
  return n;
}


/**
 * takes in a string and converts it to a struct song
 *
 * @param buff: string that contains all the information of the song
 * @return: the struct song that contains all the info
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
 * takes in a linked list and then prints out all the content
 * of all the songs in the linked list
 *
 * @param head: head of the linked list
 * @return: does not return anything
 *
 */
void print(struct node* head) {
  int size = 0; // size of the linked list
  while(head != NULL) {

    // converts the duration of song from milliseconds to minutes and seconds
    int seconds = head->info.duration / 1000;
    int minutes = 0;
    while (seconds >= 60) {
      minutes++;
      seconds -= 60;
    }

    // prints all the information of a song
    printf("%i) %-25s %-20s (%i:%02d)  D: %.3f",
      size ,head->info.title, head->info.artist, minutes, seconds, head->info.dance);

    printf("  E: %.3f  T: %.3f  V: %.3f\n",
      head->info.energy, head->info.tempo, head->info.valence);

    head = head->next;
    size++;
  }

  // prints out how many song is in linked list
  printf("\nDataset contains %i songs.\n\n", size);
  printf("=======================\n");
}
