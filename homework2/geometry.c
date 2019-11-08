#include "geometry.h"
#include <stdio.h>
#include <math.h>
#define PIE 3.1415926535897
double dot(dir_vector_t *u, dir_vector_t *v) {
    return u->x * v->x + u->y * v->y;
}

double sq_dist(position_t *a, position_t *b) {
    return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
}

void dir_from_pts(position_t *a, position_t *b, dir_vector_t *v) {
    v->x = a->x - b->x;
    v->y = a->y - b->y;
    double length = pow((v->x * v->x + v->y * v->y), 0.5);
    v->x /= length;
    v->y /= length;
}

void dir_from_angle(double angle, dir_vector_t *v) {
    v->x = cos(PIE * angle / 180);
    v->y = sin(PIE * angle / 180);
}
