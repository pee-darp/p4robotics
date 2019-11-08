#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "geometry.h"

bool is_intersect(line_t *l1, line_t *l2) {
    double cp1 = (l1->p2.x - l1->p1.x) * ((l2->p1.y) - (l1->p1.y)) -
                 ((l1->p2.y - l1->p1.y) * ((l2->p1.x) - (l1->p1.x)));
    double cp2 = ((l1->p2.x - l1->p1.x) * (l2->p2.y - l1->p1.y)) -
                 ((l1->p2.y - l1->p1.y) * (l2->p2.x - l1->p1.x));
    double cp3 = (l2->p2.x - l2->p1.x) * ((l1->p1.y) - (l2->p1.y)) -
                 ((l2->p2.y - l2->p1.y) * ((l1->p1.x) - (l2->p1.x)));
    double cp4 = ((l2->p2.x - l2->p1.x) * (l1->p2.y - l2->p1.y)) -
                 ((l2->p2.y - l2->p1.y) * (l1->p2.x - l2->p1.x));
    //printf("cp1:%f, cp2:%f\n", cp1, cp2);
    if (cp1 * cp2 < 0 && cp3 * cp4 < 0) {
        return 1;
    }
    if (cp1 * cp2 > 0) {
        return 0;
    }
    if (cp1 * cp2 == 0) {
        if (is_pt_eq(&l1->p1, &l2->p1) || is_pt_eq(&l1->p1, &l2->p2) ||
            is_pt_eq(&l1->p2, &l2->p1) || is_pt_eq(&l1->p2, &l2->p2)) {
            return 1;
        }
        return 0;
    }
    return 0;
}

void init_line(line_t *l, double x1, double y1, double x2, double y2) {
    l->p1.x = x1;
    l->p1.y = y1;
    l->p2.x = x2;
    l->p2.y = y2;
}

bool is_pt_eq(point_t *p1, point_t *p2) {
    if (p1->x == p2->x && p1->y == p2->y) {
        return 1;
    }
    return 0;
}

bool is_collide(polygon_t *pol1, polygon_t *pol2) {
    for (int i = 0; i < pol1->size; i++) {
        for (int j = 0; j < pol2->size; j++) {
            //printf("side %d x side %d:\n", i, j);
            if (is_intersect(&pol1->side[i], &pol2->side[j])) {
                return 1;
            }
        }
    }
    return 0;
}

void init_poly(polygon_t *pol, point_t *point) {
    for (int i = 0; i < (pol->size) - 1; i++) {
        init_line(&pol->side[i], point->x, point->y, (point + 1)->x, (point + 1)->y);
        point++;
    }
    init_line(&pol->side[pol->size - 1], point->x, point->y,
              (point - (pol->size - 1))->x, (point - (pol->size - 1))->y);
}

double cross(line_t *l, point_t *p) {
    return ((l->p2.x - l->p1.x) * (l->p1.y - p->y) - (l->p2.y - l->p1.y) * (l->p1.x - p->x));
}

bool is_contain(polygon_t *pol1, polygon_t *pol2) {
    int size[2] = {pol1->size, pol2->size};
    double cross_prev = cross(&pol1->side[0], &pol2->side[0].p1);
    //printf("cross_prev:%lf\n", cross_prev);
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            double cros = cross(&pol1->side[i], &pol2->side[j].p1);
            //printf("side[%d]xside[%d]:%lf\n", i, j, cros);
            if (cros * cross_prev < 0) {
                return 0;
            }
            cross_prev = cros;
        }
    }
    return 1;
}

void rot(point_t *p, double theta) {
    double theta_rad = (theta * 3.14159) / 180.0;
    double x = p->x;
    double y = p->y;
    //printf("theta_rad:%f | p.x:%f | p.y:%f\n",theta_rad, p->x, p->y);
    p->x = (x * cos(theta_rad) - y * sin(theta_rad));
    p->y = (x * sin(theta_rad) + y * cos(theta_rad));
    //printf("x:%f|y:%f\n",p->x,p->y);
}

void trans(point_t *p, double trans_x, double trans_y) {
    p->x = p->x + trans_x;
    p->y = p->y + trans_y;
}
