#include "hashtable.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct buffer {
    int capacity;
    int size;
    char *buff;
} buffer_t;

void skip_to_alpha(FILE *fp) {
    char letter;
    while(1) {
        
    }
}
int read_word(void *fp, char *word, int max_word_len) {
    int i = 0;
    while (i < max_word_len) {
        word[i] = fgetc(fp);
        if (!isalpha(word[i])) {
            word[i] == '\0';
            break;
        }
        i++;
    }
    while(!isalpha(fgetc(fp))) {

    }
    fp--;
    return i;
}

void read_n_words(buffer_t *word_buff, int n_words, FILE *fp) {
    skip_to_alpha(fp);
    read_word(fp, word_buff->buff, word_buff->capacity);
}

int main(int argc, char *argv[]) {

    FILE *fp = fopen("book.txt", "r");

    buffer_t word_buff;
    word_buff.capacity = 256;
    word_buff.size = 0;
    word_buff.buff = malloc(word_buff.capacity);

    hashtable_t *bigrams = hashtable_create();

    while (1) {
        read_n_words(&word_buff, 2, fp); //leaves fp after the last letter. and buff at end of word.
        go_back_n_words(fp, 1); //brings fp to start of previous nth word.
        *(word_buff.buff + 1) = '\0';
        hashtable_set(bigrams, word_buff.buff, 1); //change hashtable_set to increase value when slot is filled and key also matches.
        clear_buffer(&word_buff);
        if (check_eof(fp)) {
            break;
        }
    }

    for (int i = 0; i < hashtable_probe_max(bigrams); i++) {
        //TBW
    }
    return 0;
}
