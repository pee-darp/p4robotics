#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 8192

#define DEBUG_ON 0

const uint32_t factor32 = 2654435769;

uint32_t table_hash_keys[256];

uint32_t add_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash += *data;
        data++;
    }
    return hash;
}

void setup_table_hash(void) {
    for (int i = 0; i < 256; i++) {
        table_hash_keys[i] = rand();
        if (DEBUG_ON) {
            printf("hash key: %d\n", table_hash_keys[i]);
        }
    }
}

uint32_t table_a_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash += table_hash_keys[*data];
        data++;
    }
    return hash;
}

uint32_t table_b_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash = (hash << 3) ^ table_hash_keys[*data];
        data++;
    }
    return hash;
}

uint32_t djb2a_hash(uint8_t *data, int n) {
    uint32_t hash = 5381;
    for (int i = 0; i < n; i++) {
        hash = ((hash << 5) + hash) ^ *data;
        data++;
    }
    return hash;
}

uint32_t fnv1a_hash(uint8_t *data, int n) {
    uint32_t hash = 2166136261;
    uint32_t prime = 16777619;
    for (int i = 0; i < n; i++) {
        hash = (hash ^ *data) * prime;
        data++;
    }
    return hash;
}

uint32_t rotate_left(uint32_t value, uint32_t count) {
    return value << count | value >> (32 - count);
}

uint32_t fxhash32_step(uint32_t hash, uint32_t value) {
    const uint32_t key = 0x27220a95;
    return (rotate_left(hash, 5) ^ value) * key;
}

uint32_t fxhash32_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    while (n > 3) {
        uint32_t number;
        memcpy(&number, data, sizeof(number));
        hash = fxhash32_step(hash, number);
        n -= 4;
        data += 4;
    }
    while (n > 0) {
        hash = fxhash32_step(hash, *data);
        n--;
        data++;
    }
    return hash;
}

uint32_t modulo2_reduce(uint32_t hash) {
    return hash & ((1 << 13) - 1);
}

uint32_t modulo_prime_reduce(uint32_t hash) {
    return hash % 8191;
}

uint32_t fibonacci32_reduce(uint32_t hash) {
    return (hash * factor32) >> (32 - 13);
}

typedef struct test_entry {
    uint8_t *data;
    int n;
} test_entry_t;

void evaluate_hash_reduce(int n_entries, test_entry_t *entries,
                          uint32_t (*hash_f)(uint8_t *, int), uint32_t (*reduce_f)(uint32_t)) {
    clock_t start = clock();
    long iterations = 0;
    while ((double)(clock() - start) / (double)CLOCKS_PER_SEC < 0.5) {
        for (int i = 0; i < n_entries; i++) {
            reduce_f(hash_f(entries[i].data, entries[i].n));
            iterations++;
        }
    }
    bool is_hashed[8192] = {0};
    int collision;
    collision = 0;
    for (int i = 0; i < n_entries; i++) {
        uint32_t hash = reduce_f(hash_f(entries[i].data, entries[i].n));
        if (is_hashed[hash]) {
            collision++;
        } else {
            is_hashed[hash] = true;
        }
    }
    double avg_time = ((double)(clock() - start)) / (iterations * (double)CLOCKS_PER_SEC);
    printf("%.2fns per iteration, with %d collisions\n", 1000000000 * avg_time, collision);
    // double elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    // printf("complex_calculation() took %.12f seconds\n", elapsed);
}

int main(int argc, char **argv) {
    setup_table_hash();
    // First we need to collect all the entries/strings/data that we will try to hash
    // This needs to be done up front for the benchmarking later to be valid.
    int max_entries = TABLE_SIZE / 2;
    int n_entries = max_entries;
    test_entry_t *entries = calloc(max_entries, sizeof(test_entry_t));

    for (int i = 0; i < 1000; i++) {
        uint16_t num = i;
        entries[i].data = malloc(sizeof(num));
        memcpy(entries[i].data, &num, sizeof(num));
        entries[i].n = sizeof(num);
    }
    FILE *book = fopen("book.txt", "r");
    if (!book) {
        fprintf(stderr, "file not found!\n");
        return 1;
    }
    for (int i = 1000; i < n_entries; i++) {
        entries[i].data = malloc(sizeof(char) * 100);
        char *line = fgets((char *)entries[i].data, 100, book);
        entries[i].n = (int)strlen(line);
    }
    // This is an array of function pointers
    // Which will let us iterate through all the different hash functions
    // The syntax is actually only slightly different from a normal function
    // for a function pointer, the name of the function is in parenthesis
    // and it has an asterisk in front of it. The brackets after the name
    // show it is an array of function pointers. Everything outside those parentheses
    // work like normal for the function definition, and the arguments don't need names.
    uint32_t (*hash_functions[])(uint8_t *, int) = {add_hash, table_a_hash, table_b_hash,
                                                    djb2a_hash, fnv1a_hash, fxhash32_hash};
    // This is a good idiom to get the number of elements in a static array in C
    // This only works because the hash function array is declared as a literal array with { ... }
    int n_hash_functions = sizeof(hash_functions) / sizeof(hash_functions[0]);

    uint32_t (*reduce_functions[])(uint32_t) = {modulo2_reduce, modulo_prime_reduce,
                                                fibonacci32_reduce};
    int n_reduce_functions = sizeof(reduce_functions) / sizeof(reduce_functions[0]);

    for (int hash_i = 0; hash_i < n_hash_functions; hash_i++) {
        for (int reduce_i = 0; reduce_i < n_reduce_functions; reduce_i++) {
            evaluate_hash_reduce(n_entries, entries, hash_functions[hash_i],
                                 reduce_functions[reduce_i]);
        }
        printf("\n");
    }
    for (int i = 0; i < n_entries; i++) {
        free(entries[i].data);
    }
    free(entries);
    return 0;
}
