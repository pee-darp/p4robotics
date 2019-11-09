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
    char *buffer = malloc(513);
    int word_size;
    //fseek(fp, -100, SEEK_END);
    read_word(fp, word, 256);
    while (!feof(fp)) {
        char *prev_word = strdup(word);
        read_word(fp, word, 256);
        snprintf(buffer, 513, "%s %s", prev_word, word);
        printf("%s\n", buffer);
        free(prev_word);
    }
    free(word);
    free(buffer);
    return 0;
}
