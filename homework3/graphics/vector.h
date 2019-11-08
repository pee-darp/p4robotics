#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

#define VECTOR_INIT_CAPACITY 4

typedef struct point {
    double x;
    double y;
} point_t;

typedef struct vector {
    point_t *data;
    int size;
    int cap;
} vector_t;

void vector_init(vector_t *v);
void vector_resize(vector_t *v, int capacity);
void vector_add(vector_t *v, double x, double y);
point_t vector_get_min(vector_t *v);
void print_vec(vector_t *v);
void vector_free(vector_t *v);

#endif
