#include "hashtable.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 128

const uint32_t factor32 = 2654435769;

int log2n(int n) {
    return (n > 1) ? 1 + log2n(n / 2) : 0;
}

uint32_t rotate_left(uint32_t value, uint32_t count) {
    return value << count | value >> (32 - count);
}

uint32_t fxhash32_step(uint32_t hash, uint32_t value) {
    const uint32_t key = 0x27220a95;
    return (rotate_left(hash, 5) ^ value) * key;
}

uint32_t fxhash32_hash(char *data, int n) {
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

uint32_t fibonacci32_reduce(uint32_t hash, int log_table_size) {
    return (hash * factor32) >> (32 - log_table_size);
}

int hashtable_collisions(hashtable_t *ht);

typedef struct hashtable_entry {
    char *data_;
    int value_;
} hashtable_entry_t;

typedef struct hashtable {
    hashtable_entry_t *entries_;
    int table_size_;
    int n_entries_;
    int n_collisions_;
} hashtable_t;

hashtable_t *hashtable_create(void) {
    hashtable_t *hashtable = calloc(sizeof(*hashtable), 1);
    hashtable->table_size_ = TABLE_SIZE;
    hashtable->entries_ = calloc(sizeof(hashtable_entry_t), hashtable->table_size_);
    return hashtable;
}

void hashtable_destroy(hashtable_t *ht) {
    for (int i = 0; i < ht->table_size_; i++) {
        free(ht->entries_[i].data_);
    }
    free(ht->entries_);
    free(ht);
}

void hashtable_set(hashtable_t *ht, char *key, int value) {
    if (((double)ht->n_entries_ / (double)ht->table_size_) >= 0.5) {
        printf("Rehashing reduced collisions from %d to ", hashtable_collisions(ht));
        hashtable_grow(ht);
        printf("%d\n", hashtable_collisions(ht));
    }
    bool slot_found = false;
    bool key_found = false;
    int log_table_size = log2n(ht->table_size_);
    uint32_t hash = fibonacci32_reduce((fxhash32_hash(key, (int)strlen(key))),
                                       log_table_size);
    while (!slot_found && !key_found) {
        if (!ht->entries_[hash].data_) {
            ht->entries_[hash].data_ = malloc(strlen(key) + 1);
            memcpy(ht->entries_[hash].data_, key, strlen(key) + 1);
            ht->entries_[hash].value_ = value; //copying the value into empty slot
            ht->n_entries_++; //updating number of entries in the table
            slot_found = true;
        } else {
            if (!strcmp(key, ht->entries_[hash].data_)) {
                //ht->entries_[hash].value_ = value; //updating value of the entry.
                ht->entries_[hash].value_++;
                key_found = true;
            } else {
                hash++;
                if (hash >= ht->table_size_) {
                    hash = 0;
                }
            }
        }
    }
}

bool hashtable_get(hashtable_t *ht, char *key, int *value) {
    int log_table_size = log2n(ht->table_size_);
    uint32_t hash = fibonacci32_reduce((fxhash32_hash(key, (int)strlen(key))),
                                       log_table_size);
    while (1) {
        if (!ht->entries_[hash].data_) {
            return false;
        }
        if (!strcmp(key, ht->entries_[hash].data_)) {
            *value = ht->entries_[hash].value_;
            return true;
        }
        hash++;
        if (hash > ht->table_size_) {
            hash = 0;
        }
    }
}

int hashtable_size(hashtable_t *ht) {
    return ht->n_entries_;
}

int hashtable_probe_max(hashtable_t *ht) {
    return ht->table_size_;
}

bool hashtable_probe(hashtable_t *ht, int i, char **key, int *val) {
    if (i > hashtable_probe_max(ht)) {
        fprintf(stderr, "index out of bounds for given hashtable.\n");
        exit(1);
    }
    if (ht->entries_[i].data_) {
        key[i] = (char *)ht->entries_[i].data_;
        *val = ht->entries_[i].value_;
        return true;
    } else {
        return false;
    }
}

void hashtable_grow(hashtable_t *ht) {
    hashtable_t *new_hashtable = calloc(sizeof(*new_hashtable), 1);
    new_hashtable->table_size_ = 2 * ht->table_size_;
    new_hashtable->entries_ = calloc(sizeof(hashtable_entry_t), new_hashtable->table_size_);
    for (int i = 0; i < ht->table_size_; i++) {
        if (ht->entries_[i].data_) {
            hashtable_set(new_hashtable, ht->entries_[i].data_, ht->entries_[i].value_);
        }
    }
    for (int i = 0; i < ht->table_size_; i++) {
        free(ht->entries_[i].data_);
    }
    free(ht->entries_);
    ht->entries_ = new_hashtable->entries_;
    ht->table_size_ = new_hashtable->table_size_;
    ht->n_entries_ = new_hashtable->n_entries_;
    ht->n_collisions_ = new_hashtable->n_collisions_;
    free(new_hashtable);
}

int hashtable_collisions(hashtable_t *ht) {
    int log_table_size = log2n(ht->table_size_);
    // for (int i = 0; i < hashtable_probe_max(ht); i++) {
    //     if (ht->entries_[i].data_) {
    //         //printf("%d\n", i);
    //         uint32_t hash = fibonacci32_reduce((fxhash32_hash(ht->entries_[i].data_,
    //                                             strlen(ht->entries_[i].data_))), log_table_size);
    //         if (hash != i) {
    //             ht->n_collisions_++;
    //         }
    //     }
    // }
    // return ht->n_collisions_;
    bool *is_hashed = calloc(ht->table_size_, sizeof(bool));
    ht->n_collisions_ = 0;
    for (int i = 0; i < hashtable_probe_max(ht); i++) {
        if (ht->entries_[i].data_) {
            uint32_t hash = fibonacci32_reduce(fxhash32_hash(ht->entries_[i].data_,
                                                             (int)strlen(ht->entries_[i].data_)),
                                               log_table_size);
            if (is_hashed[hash]) {
                ht->n_collisions_++;
            } else {
                is_hashed[hash] = true;
            }
        }
    }
    free(is_hashed);
    return ht->n_collisions_;
}
