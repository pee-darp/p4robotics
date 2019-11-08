#include <stdio.h>
#include <math.h>

int main(void) {
    printf("Enter a number:\n");
    int p = 0;
    int vals_read = scanf("%d", &p);
    if (vals_read != 1) {
        fprintf(stderr, "Invalid input. Please enter a number.\n");
        return 1;
    }
    printf("Prime factors:\n");
    if (p == 1) {
        return 1;
    }
    while (p % 2 == 0) {
        printf("%d\n", 2);
        p = p / 2;
    }

    for (int i = 3; i <= sqrt(p); i = i + 2) {
        while (p % i == 0) {
            printf("%d\n", i);
            p = p / i;
        }
    }

    if (p > 2) {
        printf("%d\n", p);
    }
}
