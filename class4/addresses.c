#include <stdio.h>
#include <stdlib.h>
char char1;
int int1;
double doubles[3];
int main(void) {
    char char2;
    int int2;
    float floats[3];
    printf("%p\n", (void *)&char1);
    printf("%p\n", (void *)&int1);
    printf("%ld\n", ((long)&char1 - (long)&int1));
    printf("%p\n", (void *)&doubles);
    for (int i = 0; i < 3; i++) {
        printf("%p\n", (void *)&doubles[i]);
    }
    printf("%ld\n", (long)&doubles[1] - (long)&doubles[0]);
    printf("%ld\n", (long)&doubles[2] - (long)&doubles[1]);
    printf("%p\n", (void *)&char2);
    printf("%p\n", (void *)&int2);
    printf("%ld\n", (long)&char2 - (long)&int2);
    printf("%p\n", (void *)&floats);
    for (int i = 0; i < 3; i++) {
        printf("%p\n", (void *)&floats[i]);
    }
    printf("%ld\n", (long)&floats[1] - (long)&floats[0]);
    printf("%ld\n", (long)&floats[2] - (long)&floats[1]);
    void *mem = malloc(1); // ask for memory
    printf("%p\n", mem);   // memory is already "given" to us as an address!
    free(mem);             // give memory back
}
