#ifndef VARIANCE_H
#define VARIANCE_H

#include "hash.h"
typedef enum {
    WORLD_NUM = 1,
    TRIAL_RESULT = 8,
    LOCAL_GOAL_REACHED = 9,
    TRIAL_TIME = 14,
    CMD_AVG_LIN = 15,
    CMD_AVG_ANG = 16

} HEADER_NAME;

Entry read_trial_data(char *line);
bool insert_trial_data(Entry *dictionary, Entry trial_data);
#endif
