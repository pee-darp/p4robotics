#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int bin_search(const int *list, int size, int target);
int int_compare(const void *a, const void *b);
int *getRandArray(int size);

int main(int argc, char *argv[]) {
    //int ages[] = {10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
    int target = 0;
    int args_read = sscanf(argv[1], "%d", &target);
    if (args_read <= 0) {
        return 1;
    }
    int *list = getRandArray(1000);
    qsort(list, 1000, sizeof(int), int_compare);
    int index = bin_search(list, 1000, target);
    if (list[index] == target) {
        printf("%d\n", index);
    }
    free(list);
    return 0;
}

int bin_search(const int *list, int size, int target) {
    int left = 0, mid = 0, right = size;
    while (left < right) {
        mid = floor((left + right) / 2.0);
        printf("mid_i: %d\n", mid);
        if (list[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    //fprintf(stderr, "Search Unsuccessful.\n");
    return left;
}

int int_compare(const void *a, const void *b) {
    int val_a = *(int *)a;
    int val_b = *(int *)b;
    return val_a - val_b;
}

int *getRandArray(int size) {
    int *arr = calloc(sizeof(int), size);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000;
    }
    return arr;
}
