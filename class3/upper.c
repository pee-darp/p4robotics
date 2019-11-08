#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc > 1) {
        for (int count = 1; count < argc; count++) {
            for (int letter = 0; letter < strlen(argv[count]); letter++) {
                if (argv[count][letter] > 96 && argv[count][letter] < 123) {
                    printf("%c", argv[count][letter] -= 32);
                } else {
                    printf("%c", argv[count][letter]);
                }
            }
        }
        printf("\n");
        return 0;
    }
    if (argc <= 1) {
        printf("usage: ./upper<text>\n");
        return 1;
    }
}
