#include "hash.h"
// need a hash funciton
// A check if this is still efficient enough
// Delete
// Insert
// Lookup
// In this case, strings are the key, and ints are the value, ex: leaderboard
//
// http://www.cse.yorku.ca/~oz/hash.html, where hash function comes from
//
//
//
// Use Case:
//
// Input CSV: Headers:
//            World | Success | Trial Result | Local Goal Reached | Trial_Time | CMD_AVG_LIN _CMD _AVG ANG
//
//            Want Num Success per World Trial Time CMD_LIN CMD_ANG, Variance Time, Variance CMD V, Variance CMD_W
// ['timestamp', 'world_num', 'model_path', 'initial_x', 'initial_y', 'initial_yaw',
//               'goal_x', 'goal_y', 'trial_result', 'local_goal_reached', 'num_lg',
//               'RADIUS', 'NUM_VALID_OBSTACLES', 'OFFSET', 'TRIAL_TIME',
//               'CMD_AVG_LIN', 'CMD_AVG_ANG', 'CMD_RMS_LIN', 'CMD_RMS_ANG',
//               'ODOM_AVG_LIN', 'ODOM_AVG_ANG', 'ODOM_RMS_LIN', 'ODOM_RMS_ANG']
// int main() {
//   Entry *dictionary = calloc(DICT_SIZE, sizeof(Entry)); // init all zeros
//   printf("Start of dictionary implementation\n");
//   char inputString[] = "Hi";
//
//   insert_element(inputString, 5, dictionary);
//   char inputString_2[] = "Hello";
//
//   insert_element(inputString_2, 20, dictionary);
//   insert_element(inputString, 10, dictionary);
//   // delete_element(inputString, dictionary);
//   // delete_element(inputString, dictionary);
//
//   print_stats();
//   printf("Value from inputString %d\n", get_value(inputString_2, dictionary));
//   print_dictionary(dictionary);
//   free(dictionary);
//   return 0;
// }

void configure(Entry *dictionary) {

    for (size_t slot_init = 0; slot_init < DICT_SIZE; slot_init++) {
        Entry entry;
        dictionary[slot_init] = entry;
        dictionary[slot_init].State = EMPTY;
    }
    return;
}

bool insert_Entry(Entry *dictionary, Entry *entry) {
    printf("insertining a entry\n");
    unsigned long hash_value = hash(entry->Key);
    if (dictionary[hash_value].State != OCCUPIED) {
        dictionary[hash_value].trial_time = entry->trial_time;
        dictionary[hash_value].trial_result = entry->trial_result;
        dictionary[hash_value].cmd_v = entry->cmd_v;
        dictionary[hash_value].cmd_w = entry->cmd_w;
        dictionary[hash_value].local_goal_reached = entry->local_goal_reached;
        dictionary[hash_value].State = OCCUPIED;
        printf("Successful insertation of entry of %s", entry->Key);
    } else {
        printf("Collision\n");
        return true;
    }
}

bool insert_element(char *key, int value, Entry *dictionary) {
    // return true if successful insertation
    unsigned long hash_value = hash((const char *)key);
    if (dictionary[hash_value].State != OCCUPIED) {
        dictionary[hash_value].Value = value;
        dictionary[hash_value].Key = key;
        dictionary[hash_value].State = OCCUPIED;
        // num_elements++;

        printf("Successful insertion\n");
        return true;
    }
    // if they are editing over
    else if (!strcmp(dictionary[hash_value].Key, (const char *)key)) {
        printf("overwritingu\n");
        dictionary[hash_value].Value = value;
        return true;
    }
    printf("Element already exists\n");
    unsigned long collision_index = hash_value + 1;
    printf("Trying at %ld\n", collision_index);

    while (dictionary[collision_index].State != EMPTY) {
        printf("still a collision");
        collision_index += 1;
        if (collision_index > DICT_SIZE - 1) {
            collision_index = 0;
        }
    }
    if (dictionary[collision_index].State == EMPTY) {
        printf("adding element at : %ld", collision_index);

        dictionary[collision_index].Key = key;
        dictionary[collision_index].Value = value;
        dictionary[collision_index].State = OCCUPIED;
        // num_elements++;
    }
    return true;
}

bool delete_element(char *key, Entry *dictionary) {
    // return true if successful deletion, false if value DNE
    unsigned long hash_value = hash((const char *)key);

    if (dictionary[hash_value].State == OCCUPIED) {
        dictionary[hash_value].Value = 0;
        dictionary[hash_value].State = TOMBSTONE;
        // num_elements--;

        printf("Successful deletion\n");
        return true;
    }
    printf("Key does not associate with a value\n");
    return false;
}
static inline uint32_t rol32(uint32_t x, unsigned r) { return (x << r) | (x >> (32 - r)); }

size_t hash(const char *s) {
    uint32_t h = 0x55555555u; // unsigned seed
    while (*s) {
        h ^= (uint8_t)*s++; // treat chars as bytes
        h = rol32(h, 5);
    }
    return (size_t)(h % DICT_SIZE); // always 0..DICT_SIZE-1
}
// unsigned int rol(unsigned int value, int shift) { return (value << shift) | (value >> (32 - shift)); }
// int hash(char const *input) {
//     int result = 0x55555555;
//
//     while (*input) {
//         result ^= *input++;
//         result = rol(result, 5);
//     }
//     printf("result from hash is : %d\n", (result % DICT_SIZE));
//     return (result % DICT_SIZE);
// }

int get_value(char *key, Entry *dictionary) {

    unsigned long hash_value = hash((const char *)key);
    if (dictionary[hash_value].State != EMPTY) {
        if (!strcmp(dictionary[hash_value].Key, (const char *)key)) {
            return dictionary[hash_value].Value;
        }
    }
    hash_value += 1;
    if (hash_value > DICT_SIZE - 1) {
        hash_value = 0;
    }
    while (strcmp(dictionary[hash_value].Key, (const char *)key)) {
        hash_value += 1;
    }
    return dictionary[hash_value].Value;
}

void print_stats() {
    return;
    // printf("Num of elements : %d, total capacity %d\n", num_elements, DICT_SIZE);
}

void print_dictionary(Entry *dictionary) {

    for (size_t dict_counter = 0; dict_counter < DICT_SIZE; dict_counter++) {
        if (dictionary[dict_counter].State == OCCUPIED) {
            printf("Key : %s Value : %d\n", dictionary[dict_counter].Key, dictionary[dict_counter].Value);
        }
    }
}
