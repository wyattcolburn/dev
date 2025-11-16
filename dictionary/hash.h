#ifndef HASH_HPP
#define HASH_HPP

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { EMPTY, OCCUPIED, TOMBSTONE } SlotState;
typedef struct {
  char *Key;
  int Value;
  float trial_time;
  float avg_trial_time;
  char *trial_result;
  int num_success;
  int local_goal_reached;
  float cmd_v;
  float cmd_w;
  float avg_cmd_v;
  float avg_cmd_w;
  SlotState State;
} Entry;

#define DICT_SIZE 10007
extern int num_elements;
// Function Prototypes

bool insert_element(char *key, int input_value, Entry *dictionary);
bool delete_element(char *key, Entry *dictionary);
int hash(char const *input);
unsigned int rol(unsigned int value, int shift);
void print_stats();
int get_value(char *key, Entry *dictionary);
void print_dictionary(Entry *dictionary);
#endif
