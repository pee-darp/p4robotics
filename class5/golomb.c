/*
1.create a base array and allocate some memory to it
2.size = length(base_array)
3.capacity  = space left = size - current_index
4.create a function to append data to vector (array, data, size, capacity, new_cap_mult)
  a. check for remaining capacity
  b. if space is available, append data to the last index. If not, realloc memory
  c. Update the capacity and size.
  d. return the size of the vector
5.initialize the base array as [1 2 2]
6.max = number upto which i have to printf
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct vector {
    int *data;
    int size;
    int cap;
} vector_t;

void vector_init(vector_t *v) {
    v->cap = 2;
    v->data = malloc(sizeof(*v->data) * v->cap);
    v->size = 0;
}

void vec_append(vector_t *v, int item) {
    if (v->cap == v->size) {
        v->data = realloc(v->data, sizeof(*v->data) * (2 * v->cap));
        v->cap = 2 * v->cap;
        v->data[v->size] = item;
        v->size++;
        //printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
    } else {
        v->data[v->size] = item;
        v->size++;
        //printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: ./golomb <max golomb number>\n");
        return 1;
    }
    vector_t golomb;
    vector_init(&golomb);
    vec_append(&golomb, 1);
    vec_append(&golomb, 2);
    vec_append(&golomb, 2);
    //vec_append(&golomb, 2);
    //vec_append(&golomb, 2);
    char *ptr;
    int max = strtol(argv[1], &ptr, 10);
    if (max == 0) {
        free(golomb.data);
        return 0;
    }
    if (max == 1) {
        printf("%d\n", 1);
        free(golomb.data);
        return 0;
    }
    if (max != 2) {
        int i = 3;
        while (i <= max) {
            for (int j = 0; j < golomb.data[i - 1]; j++) {
                vec_append(&golomb, i);
            }
            i++;
        }
    }
    for (int s = golomb.size; s > 0; s--) {
        printf("%d\n", golomb.data[s - 1]);
    }
    free(golomb.data);
    return 0;
}
