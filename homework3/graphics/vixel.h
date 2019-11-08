#ifndef VIXEL_H
#define VIXEL_H

#include <stdint.h>

#define VECTOR_INIT_CAPACITY 4

typedef struct pixel {
    int32_t x;
    int32_t y;
} pixel_t;

typedef struct vixel {
    pixel_t *data;
    int size;
    int cap;
} vixel_t;

void vixel_init(vixel_t *v);
void vixel_resize(vixel_t *v, int capacity);
void vixel_add(vixel_t *v, int32_t x, int32_t y);
void vixel_clear(vixel_t *v);
void print_vix(vixel_t *v);
void vixel_free(vixel_t *v);

#endif
