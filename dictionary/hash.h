#ifndef HASH_HPP
#define HASH_HPP

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_SIZE 32
#define MAX_TRIALS 10
#define MAX_RESULT_SIZE 32
#define NUM_TRIALS 5
typedef enum { EMPTY, OCCUPIED, TOMBSTONE } SlotState;
typedef struct {
    char Key[MAX_KEY_SIZE];
    int Value;
    float trial_time[NUM_TRIALS];
    float avg_trial_time;
    char trial_result[MAX_RESULT_SIZE];
    int num_success;
    int local_goal_reached[NUM_TRIALS];
    float cmd_v[NUM_TRIALS];
    float cmd_w[NUM_TRIALS];
    float odom_v[NUM_TRIALS];
    float odom_w[NUM_TRIALS];
    float avg_cmd_v;
    float avg_cmd_w;
    float avg_odom_v;
    float avg_odom_w;
    SlotState State;
    float trial_time_variance;
    float cmd_v_variance;
    float cmd_w_variance;

    float odom_v_variance;
    float odom_w_variance;
} Entry;

#define DICT_SIZE 10007
extern int num_elements;
// Function Prototypes
void configure(Entry *dictionary);
bool insert_element(char *key, int input_value, Entry *dictionary);
bool insert_Entry(Entry *entry, Entry *dictionary, size_t *occupied_hash_slots, int *num_entries);
bool delete_element(char *key, Entry *dictionary);
// int hash(char const *input);
// unsigned int rol(unsigned int value, int shift);
static inline uint32_t rol32(uint32_t x, unsigned r);
size_t hash(const char *s);
void print_stats(int *num_entries);
int get_value(char *key, Entry *dictionary);
void print_dictionary(Entry *dictionary);
void stats_from_key(Entry *dictionary, const char *s);
void data_out(Entry *dictionary, size_t *occupied_hash_slots, int *num_entries);
#endif
