#include "hash.h"
#include <stdio.h>
#include <string.h>

#define MAXCHAR 1000
//1) Read from a CSV
//2) Fill Dictionary
//3) Variance Calculation
int main() {
  // int num_elements = 0;
  Entry *dictionary = calloc(DICT_SIZE, sizeof(Entry)); // init all zeros
  

  FILE * csv = fopen("baseline.csv","r");

  char line[MAXCHAR];
  char *token;
  
  while (feof(csv) != true)
  {
    fgets(line, MAXCHAR, csv);
    printf("Line: %s", line);

    token = strtok(line, ",");
  }
    while (token!= NULL)
{ 
  printf("Token: %s\n", token);
  token = strtok(NULL, ",");
    }

  free(dictionary);

  return 0;
}

