#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        int parity = 0;
        for (int i = 0; i < strlen(argv[1]); i++) {
            char num = argv[1][i];
            for (int j = 0; j < 8; j++) {
                parity = parity ^ (num & 0x01);
                num = num >> 1;
            }
        }
        printf("%d\n", parity);
        return 0;
    }
    if (argc == 1) {
        printf("0\n");
        return 1;
    }
    if (argc > 2) {
        printf("Too  many arguments\nusage: ./parity [<text>]\n");
        return 1;
    }
}
