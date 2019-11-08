#ifndef PRIMITIVES_H
#define PRIMITIVES_H

typedef struct polygon {
    int size;
    double vertex[16][2];
    double x, y;
    double rot;
} polygon_t;

#endif
