#include "variance.h"
#include <stdio.h>
#include <string.h>

#define MAXCHAR 1000
// 1) Read from a CSV
// 2) Fill Dictionary
// 3) Variance Calculation
int main() {
    // int num_elements = 0;
    Entry *dictionary = calloc(DICT_SIZE, sizeof(Entry)); // init all zeros

    FILE *csv = fopen("baseline.csv", "r");

    char line[MAXCHAR];
    char *token;

    fgets(line, MAXCHAR, csv);
    read_trial_data((char *)line);
    fgets(line, MAXCHAR, csv);
    Entry first_entry = read_trial_data((char *)line);
    insert_trial_data(dictionary, first_entry);
    fgets(line, MAXCHAR, csv);
    Entry second_entry = read_trial_data((char *)line);
    insert_trial_data(dictionary, second_entry);
    fgets(line, MAXCHAR, csv);
    // while (feof(csv) != true) {
    //     fgets(line, MAXCHAR, csv);
    //     printf("Line: %s", line);
    //     int token_counter = 0;
    //     token = strtok(line, ",");
    //
    //     while (token != NULL) {
    //         printf("Token %d: %s\n", token_counter, token);
    //         token = strtok(NULL, ",");
    //         token_counter++;
    //     }
    // }
    free(dictionary);

    return 0;
}

Entry read_trial_data(char *line) {

    Entry trial_data;
    int token_counter = 0;
    char *token;
    token = strtok(line, ",");

    while (token != NULL) {
        printf("Token %d: %s\n", token_counter, token);
        switch (token_counter) {
        case WORLD_NUM:
            trial_data.Key = (const char *)(token);
            break;
        case TRIAL_RESULT:
            trial_data.trial_result = (token);
            break;
        case LOCAL_GOAL_REACHED:
            trial_data.local_goal_reached = atoi(token);
            break;
        case TRIAL_TIME:
            trial_data.trial_time = atof(token);
            break;
        case CMD_AVG_LIN:
            trial_data.cmd_v = atof(token);
            break;
        case CMD_AVG_ANG:
            trial_data.cmd_w = atof(token);
            break;
        default:
            // ignore other columns
            break;
        }
        token = strtok(NULL, ",");
        token_counter++;
    }
    printf("Key of trial_data: %s\n", trial_data.Key);
    return trial_data;
}

bool insert_trial_data(Entry *dictionary, Entry trial_data) {
    printf("Trying to insert a entry\n");
    printf("Trial result : %s", trial_data.trial_result);
    if ((strcmp(trial_data.trial_result, "SUCCESS")) == 0) {
        insert_Entry(dictionary, &trial_data);
    } else {
        printf("Not a successful trial\n");
    }
    return true;
}
