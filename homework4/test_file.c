#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen("book.txt", "r");
    printf("fp:%p\n", fp);
    char letter = fgetc(fp);
    printf("%c\n", letter);
    printf("fp:%p\n", fp);
    letter = fgetc(fp);
    printf("%c\n", letter);
}
