#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_ON 0

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

uint32_t djb2a(uint8_t *data, int n) {
    uint32_t hash = 5381;
    for (int i = 0; i < n; i++) {
        hash = ((hash << 5) + hash) ^ *data;
        data++;
    }
    return hash;
}

uint32_t fnv1a(uint8_t *data, int n) {
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

uint32_t fxhash32(uint8_t *data, int n) {
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

int main(int argc, char *argv[]) {
    uint32_t hash = 0;
    setup_table_hash();
    if (!strcmp(argv[1], "add")) {
        hash = add_hash((uint8_t *)argv[2], (int)strlen(argv[2]));
    } else if (!strcmp(argv[1], "table_a")) {
        hash = table_a_hash((uint8_t *)argv[2], (int)strlen(argv[2]));
    } else if (!strcmp(argv[1], "table_b")) {
        hash = table_b_hash((uint8_t *)argv[2], (int)strlen(argv[2]));
    } else if (!strcmp(argv[1], "djb2a")) {
        hash = djb2a((uint8_t *)argv[2], (int)strlen(argv[2]));
    } else if (!strcmp(argv[1], "fnv1a")) {
        hash = fnv1a((uint8_t *)argv[2], (int)strlen(argv[2]));
    } else if (!strcmp(argv[1], "fxhash32")) {
        hash = fxhash32((uint8_t *)argv[2], (int)strlen(argv[2]));
    } else {
        fprintf(stderr, "invalid function!\n");
        exit(1);
    }
    printf("0x%x\n", hash);
    return 0;
}
