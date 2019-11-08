#include "vixel.h"
#include <stdlib.h>
#include <stdio.h>
void vixel_init(vixel_t *v) {
    v->cap = 4;
    v->data = malloc(sizeof(*v->data) * v->cap);
    v->size = 0;
}

void vixel_resize(vixel_t *v, int capacity) {
    void *memory = realloc(v->data, sizeof(*v->data) * capacity);
    if (memory) {
        v->data = memory;
        v->cap = capacity;
    } else {
        fprintf(stderr, "Could not reallocate memory to vixel_t.\n");
        exit(1);
    }
}

void vixel_add(vixel_t *v, int32_t x, int32_t y) {
    if (v->cap == v->size) {
        vixel_resize(v, v->cap * 2);
    }
    v->data[v->size].x = x;
    v->data[v->size].y = y;
    v->size++;
}

void vixel_clear(vixel_t *v) {
    for (int i = 0; i < v->size; i++) {
        v->data[i].x = 0;
        v->data[i].y = 0;
    }
}

void print_vix(vixel_t *v) {
    for (int i = 0; i < v->size; i++) {
        printf("%d|", v->data[i].x);
        printf("%d , ", v->data[i].y);
    }
    printf("\n");
}

void vixel_free(vixel_t *v) {
    free(v->data);
    free(v);
}
