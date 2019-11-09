#define _GNU_SOURCE
#include "hashtable.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

void skip_to_alpha(void *fp) {
    while (!isalpha(fgetc(fp)) && !feof(fp)) {
    }
    if (!feof(fp)) {
        fseek(fp, -1, SEEK_CUR);
    }
}

int read_word(void *fp, char *word, int max_word_len) {
    int i = 0;
    while (i < max_word_len && !feof(fp)) {
        word[i] = (char)fgetc(fp);
        if (!isalpha(word[i]) || feof(fp)) {
            word[i] = '\0';
            break;
        }
        i++;
    }
    skip_to_alpha(fp);
    return i;
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen("book.txt", "r");
    char *word = malloc(256);
    //char *prev_word = malloc(256);
    char *buffer = malloc(513);
    hashtable_t *bigrams = hashtable_create();
    int most_bigrams = 0;
    int *bg_ptr = &most_bigrams;
    bool print_all = true;
    read_word(fp, word, 256);
    while (!feof(fp)) {
        char *prev_word = strdup(word);
        read_word(fp, word, 256);
        snprintf(buffer, 513, "%s %s", prev_word, word);
        hashtable_set(bigrams, buffer, 1);
        free(prev_word);
    }
    char **key = malloc(hashtable_probe_max(bigrams) * sizeof(key));
    for (int i = 0; i < hashtable_probe_max(bigrams); i++) {
        if (hashtable_probe(bigrams, i, key, bg_ptr)) {
            if (*bg_ptr >= 200) {
                printf("Bigram '%s' has count of %d\n", key[i], *bg_ptr);
                print_all = false;
            }
        }
    }
    if (print_all) {
        char **keys = malloc(hashtable_probe_max(bigrams) * sizeof(keys));
        for (int i = 0; i < hashtable_probe_max(bigrams); i++) {
            if (hashtable_probe(bigrams, i, keys, bg_ptr)) {
                printf("Bigram '%s' has count of %d\n", key[i], *bg_ptr);
            }
        }
        free(keys);
    }
    printf("Total of %d different bigrams recorded\n", hashtable_size(bigrams));
    free(key);
    free(word);
    free(buffer);
    hashtable_destroy(bigrams);
    return 0;
}
