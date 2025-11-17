#include "variance.h"
#include <stdio.h>
#include <string.h>

#define MAXCHAR 1000
// 1) Read from a CSV
// 2) Fill Dictionary
// 3) Variance Calculation
int main() {
    // int num_elements = 0;

    size_t *occupied_hash_values = calloc(DICT_SIZE, sizeof(size_t));
    int *num_entries = calloc(1, sizeof(int));
    *num_entries = 0;
    Entry *dictionary = calloc(DICT_SIZE, sizeof(Entry)); // init all zeros
    configure(dictionary);
    FILE *csv = fopen("baseline.csv", "r");

    char line[MAXCHAR];
    char *token;

    while (feof(csv) != true) {
        fgets(line, MAXCHAR, csv);
        Entry temp_entry = read_trial_data((char *)line);
        insert_trial_data(dictionary, temp_entry, occupied_hash_values, num_entries);
    }
    print_stats(num_entries);
    const char *input_world = "world 5";
    printf("trying to do stats from key\n");
    stats_from_key(dictionary, input_world);
    for (int entryCounter = 0; entryCounter < *num_entries; entryCounter++) {

        process_entry(dictionary, occupied_hash_values[entryCounter]);
    }
    data_out(dictionary, occupied_hash_values, num_entries);
    free(occupied_hash_values);
    free(dictionary);

    return 0;
}

Entry read_trial_data(char *line) {

    Entry trial_data;
    int token_counter = 0;
    char *token;
    token = strtok(line, ",");
    trial_data.num_success = 0;
    while (token != NULL) {
        switch (token_counter) {
        case WORLD_NUM:
            snprintf(trial_data.Key, sizeof(trial_data.Key) - 1, "%s", token);
            // strncpy(trial_data.Key, token, sizeof(trial_data.Key - 1));
            break;
        case TRIAL_RESULT:
            snprintf(trial_data.trial_result, sizeof(trial_data.trial_result) - 1, "%s", token);
            // strncpy(trial_data.trial_result, token, sizeof(trial_data.trial_result - 1));
            // break;
        case LOCAL_GOAL_REACHED:
            trial_data.local_goal_reached[0] = atoi(token);
            break;
        case TRIAL_TIME:
            trial_data.trial_time[0] = atof(token);
            break;
        case CMD_AVG_LIN:
            trial_data.cmd_v[0] = atof(token);
            break;
        case CMD_AVG_ANG:
            trial_data.cmd_w[0] = atof(token);
            break;
        case ODOM_AVG_LIN:
            trial_data.odom_v[0] = atof(token);
            break;
        case ODOM_AVG_ANG:
            trial_data.odom_w[0] = atof(token);
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

bool insert_trial_data(Entry *dictionary, Entry trial_data, size_t *occupied_hash_values, int *num_entries) {
    printf("Trying to insert a entry\n");
    printf("Trial result : %s\n", trial_data.trial_result);
    if ((strcmp(trial_data.trial_result, "SUCCESS")) == 0) {
        insert_Entry(dictionary, &trial_data, occupied_hash_values, num_entries);
    } else {
        printf("Not a successful trial\n\n");
    }
    return true;
}

void process_entry(Entry *dictionary, size_t hash_value) {

    // average trial time = sum of trial_time / num_successes
    float sum_trial_time = 0;
    for (int i = 0; i < dictionary[hash_value].num_success; i++) {
        sum_trial_time += dictionary[hash_value].trial_time[i];
    }
    dictionary[hash_value].avg_trial_time = sum_trial_time / dictionary[hash_value].num_success;

    float sum_cmd_v = 0;
    for (int i = 0; i < dictionary[hash_value].num_success; i++) {
        sum_cmd_v += dictionary[hash_value].cmd_v[i];
    }
    dictionary[hash_value].avg_cmd_v = sum_cmd_v / dictionary[hash_value].num_success;

    float sum_cmd_w = 0;
    for (int i = 0; i < dictionary[hash_value].num_success; i++) {
        sum_cmd_w += dictionary[hash_value].cmd_w[i];
    }
    dictionary[hash_value].avg_cmd_w = sum_cmd_w / dictionary[hash_value].num_success;

    float sum_odom_v = 0;
    for (int i = 0; i < dictionary[hash_value].num_success; i++) {
        sum_odom_v += dictionary[hash_value].odom_v[i];
    }
    dictionary[hash_value].avg_odom_v = sum_odom_v / dictionary[hash_value].num_success;

    float sum_odom_w = 0;
    for (int i = 0; i < dictionary[hash_value].num_success; i++) {
        sum_odom_w += dictionary[hash_value].odom_w[i];
    }
    dictionary[hash_value].avg_odom_w = sum_odom_w / dictionary[hash_value].num_success;
    variance(&dictionary[hash_value]);
    printf("average values at %s, trial_time : %f cmd_v : %f, cmd_w : %f odom_v : %f, odom_w : %f\n",
           dictionary[hash_value].Key, dictionary[hash_value].avg_trial_time, dictionary[hash_value].avg_cmd_v,
           dictionary[hash_value].avg_cmd_w, dictionary[hash_value].avg_odom_v, dictionary[hash_value].avg_odom_w);
    printf("variance values at %s, trial_time: %f cmd_v: %f, cmd_w: %f odom_v: %f, odom_w: %f\n",
           dictionary[hash_value].Key, dictionary[hash_value].trial_time_variance,
           dictionary[hash_value].cmd_v_variance, dictionary[hash_value].cmd_w_variance,
           dictionary[hash_value].odom_v_variance, dictionary[hash_value].odom_w_variance);
}

void variance(Entry *e) {
    if (e->num_success > 1) {
        float variance = 0;
        for (int i = 0; i < e->num_success; i++) {
            variance += ((e->trial_time[i] - e->avg_trial_time) * (e->trial_time[i] - e->avg_trial_time));
        }
        e->trial_time_variance = variance / e->num_success;

        variance = 0;
        for (int i = 0; i < e->num_success; i++) {
            variance += ((e->cmd_v[i] - e->avg_cmd_v) * (e->cmd_v[i] - e->avg_cmd_v));
        }
        e->cmd_v_variance = variance / e->num_success;

        variance = 0;
        for (int i = 0; i < e->num_success; i++) {
            variance += ((e->cmd_w[i] - e->avg_cmd_w) * (e->cmd_w[i] - e->avg_cmd_w));
        }
        e->cmd_w_variance = variance / e->num_success;

        variance = 0;
        for (int i = 0; i < e->num_success; i++) {
            variance += ((e->odom_v[i] - e->avg_odom_v) * (e->odom_v[i] - e->avg_odom_v));
        }
        e->odom_v_variance = variance / e->num_success;

        variance = 0;
        for (int i = 0; i < e->num_success; i++) {
            variance += ((e->odom_w[i] - e->avg_odom_w) * (e->odom_w[i] - e->avg_odom_w));
        }
        e->odom_w_variance = variance / e->num_success;
        return;
    } else {
        e->odom_v_variance = 0;
        e->odom_w_variance = 0;
        e->cmd_v_variance = 0;
        e->cmd_w_variance = 0;
        e->trial_time_variance = 0;
    }
}

void data_out(Entry *dictionary, size_t *occupied_hash_slots, int *num_entries) {

    FILE *output = fopen("output.csv", "w");

    fprintf(output, "world,num_success, avg_trial_time,var_trial_time,avg_cmd_v,var_cmd_v,avg_cmd_w,var_cmd_w, "
                    "avg_odom_v,var_odom_v, avg_odom_w, var_odom_w\n");

    for (int i = 0; i < *num_entries; i++) {
        Entry *e = &dictionary[occupied_hash_slots[i]];
        fprintf(output, "%s,%d, %.3f,%.3f,%.3f,%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", e->Key, e->num_success,
                e->avg_trial_time, e->trial_time_variance, e->avg_cmd_v, e->cmd_v_variance, e->avg_cmd_w,
                e->cmd_w_variance, e->avg_odom_v, e->odom_v_variance, e->avg_odom_w, e->odom_w_variance);
    }
    fclose(output);
    return;
}
