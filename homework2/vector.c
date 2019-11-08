#include "vector.h"

void vector_init(vector_t *v) {
    v->cap = 500;
    v->data = malloc(sizeof(*v->data) * v->cap);
    v->size = 0;
}

void cod_init(codList_t *v) {
    v->cap = 4;
    v->data = malloc(sizeof(*v->data) * v->cap);
    v->size = 0;
}

void vec_append(vector_t *v, int item1, int item2) {
    if (v->cap == v->size) {
        v->data = realloc(v->data, sizeof(*v->data) * (2 * v->cap));
        v->cap = 2 * v->cap;
        v->data[v->size].x = item1;
        v->data[v->size].y = item2;
        v->size++;
        //printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
    } else {
        v->data[v->size].x = item1;
        v->data[v->size].y = item2;
        v->size++;
        //printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
    }
}

void cod_append(codList_t *v, double item1, double item2) {
    if (v->cap == v->size) {
        v->data = realloc(v->data, sizeof(*v->data) * (2 * v->cap));
        v->cap = 2 * v->cap;
        v->data[v->size].x = item1;
        v->data[v->size].y = item2;
        v->size++;
        //printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
    } else {
        v->data[v->size].x = item1;
        v->data[v->size].y = item2;
        v->size++;
        //printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
    }
}

void print_vec(vector_t *v) {
    for (int i = 0; i < v->size; i++) {
        printf("%d|", v->data[i].x);
        printf("%d , ", v->data[i].y);
    }
    printf("\n");
}

void print_cod(codList_t *v) {
    for (int i = 0; i < v->size; i++) {
        printf("%f|", v->data[i].x);
        printf("%f , ", v->data[i].y);
    }
    printf("\n");
}
