#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char *get_context(char *key, int lines_to_read, int key_len);
int strlenw(char *w, char delimiter);

char file_buffer[64 * 1024];

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Could not open %s: ", argv[1]);
        perror("");
        return 1;
    }
    int bytes_read = fread(file_buffer, 1, sizeof(file_buffer) - 1, f);
    if (!feof(f)) {
        fprintf(stderr, "Could not read entire file. Is it too big?\n");
        return 1;
    }
    fclose(f);
    // we want this to be a null-treminated string,
    // but fread just reads the file as binary, so we add it ourselves
    file_buffer[bytes_read] = '\0';
    //char tempo;
    //tempo = file_buffer[0];
    //file_buffer[0] = '$';
    char *key_loc, *ptr;
    key_loc = file_buffer;
    int key_len = strlen(argv[2]);
    while (key_loc) {
        key_loc = strstr(key_loc, argv[2]);
        key_loc = get_context(key_loc, strtol(argv[3], &ptr, 10), key_len);
        //printf("\n");
    }
    return 0;
}

char *get_context(char *key, int lines_to_read, int key_len) {
    if (!key) {
        return NULL;
    }
    int lines = lines_to_read;
    char letter, temp;
    //int key_len = strlenw(key, ',');
    char *key_end = key + key_len;
    temp = *key_end;
    *key_end = '\0';
    while (lines > 0) {
        letter = *key;
        key--;
        if (letter == '\n') {
            lines--;
        }
        if (key == file_buffer) {
            break;
        }
    }
    if (key != file_buffer) {
        if (lines_to_read != 0) {
            printf("%s\n\n", key + 2);
        } else {
            printf("%s\n\n", key);
        }
    } else {
        printf("%s\n\n", key);
    }
    *key_end = temp;
    return key_end;
}

int strlenw(char *w, char delimiter) {
    int length = 0;
    while (*w != delimiter && *w != ' ' && *w != '\0') {
        length++;
        w++;
    }
    return length;
}
