#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include "geometry.h"

typedef struct pixel {
    int x;
    int y;
} pixel_t;

typedef struct vector {
    pixel_t *data;
    int size;
    int cap;
} vector_t;

typedef struct codList {
    point_t *data;
    int size;
    int cap;
} codList_t;

void vector_init(vector_t *v);
void cod_init(codList_t *v);

void vec_append(vector_t *v, int item1, int item2);
void cod_append(codList_t *v, double item1, double item2);

void print_vec(vector_t *v);
void print_cod(codList_t *v);

#endif
