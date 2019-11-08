#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct point {
    double x;
    double y;
} point_t;

typedef struct line {
    point_t p1;
    point_t p2;
} line_t;

typedef struct polygon {
    int size;
    line_t side[16];
    double x_glob, y_glob;
    double rot;
} polygon_t;

typedef struct position {
    double x;
    double y;
} position_t;

typedef struct dir_vector {
    double x;
    double y;
} dir_vector_t;

typedef struct pose {
    position_t center;
    double heading;
} pose_t;

double dot(dir_vector_t *u, dir_vector_t *v);
double sq_dist(position_t *a, position_t *b);
void dir_from_pts(position_t *a, position_t *b, dir_vector_t *v);
void dir_from_angle(double angle, dir_vector_t *v);

bool is_intersect(line_t *l1, line_t *l2);
bool is_pt_eq(point_t *p1, point_t *p2);
void init_line(line_t *l, double x1, double y1, double x2, double y2);
bool is_collide(polygon_t *pol1, polygon_t *pol2);
void init_poly(polygon_t *pol, point_t *point);
bool is_contain(polygon_t *pol1, polygon_t *pol2);
double cross(line_t *l, point_t *p);
void rot(point_t *p, double theta);
void trans(point_t *p, double trans_x, double trans_y);

#endif
