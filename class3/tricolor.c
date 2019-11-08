#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

char *trit_encodings[243];

int main(void) {
    for (int i = 0; i < 243; i++) {
        trit_encodings[i] = calloc(sizeof(char), 6);
        trit_encodings[i][5] = 0;
        int rem = 0;
        int index = 0;
        int num = i;
        while (index < 5) {
            rem = num % 3;
            num = num / 3;
            switch (rem) {
            case 0:
                trit_encodings[i][index] = ' ';
                break;
            case 1:
                trit_encodings[i][index] = ':';
                break;
            case 2:
                trit_encodings[i][index] = '@';
                break;
            default:
                return 1;
            }
            index++;
        }
        num++;
    }

    FILE *fp = fopen("img.bin", "rb");
    for (int row = 0; row < 40; row++) {
        for (int col = 0; col < 18; col++) {
            uint8_t c = fgetc(fp);
            printf("%s", trit_encodings[c]);
        }
        printf("\n");
    }
    return 0;
}
