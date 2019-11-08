#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "../structures/primitives.h"
#include "collision.h"
#define PI 3.141592653589

bool is_collide(polygon_t *pg1, polygon_t *pg2) {
    double line1[2][2] = {{0, 0}, {0, 0}};
    double line2[2][2] = {{0, 0}, {0, 0}};
    double collide_cross_prod[4] = {0, 0, 0, 0};
    //code to check collision.
    for (int l1 = 0; l1 < pg1->size; l1++) {
        line1[0][0] = pg1->vertex[l1 % pg1->size][0];
        line1[0][1] = pg1->vertex[l1 % pg1->size][1];
        line1[1][0] = pg1->vertex[(l1 + 1) % pg1->size][0];
        line1[1][1] = pg1->vertex[(l1 + 1) % pg1->size][1];
        for (int l2 = 0; l2 < pg2->size; l2++) {
            line2[0][0] = pg2->vertex[l2 % pg2->size][0];
            line2[0][1] = pg2->vertex[l2 % pg2->size][1];
            line2[1][0] = pg2->vertex[(l2 + 1) % pg2->size][0];
            line2[1][1] = pg2->vertex[(l2 + 1) % pg2->size][1];
            collide_cross_prod[0] = cross(line1, line2[0]);
            collide_cross_prod[1] = cross(line1, line2[1]);
            collide_cross_prod[2] = cross(line2, line1[0]);
            collide_cross_prod[3] = cross(line2, line1[1]);
            if (collide_cross_prod[0] * collide_cross_prod[1] < 0 &&
                collide_cross_prod[2] * collide_cross_prod[3] < 0) {
                return true;
            }
            if (onSegment(line1, line2[0]) || onSegment(line1, line2[1]) ||
                onSegment(line2, line1[0]) || onSegment(line2, line1[1])) {
                return true;
            }
        }
    }
    return false;
}

bool is_contain(polygon_t *pg1, polygon_t *pg2) {
    polygon_t *polygon[2] = {pg1, pg2};
    double lineA[2][2] = {{0, 0}, {0, 0}};
    double pointB[2] = {0, 0};
    for (int i = 0; i < 2; i++) {
        int positive = 0;
        polygon_t *pgA = polygon[i % 2];
        polygon_t *pgB = polygon[(i + 1) % 2];
        pointB[0] = pgB->vertex[0][0];
        pointB[1] = pgB->vertex[0][1];
        for (int j = 0; j < pgA->size; j++) {
            lineA[0][0] = pgA->vertex[j % pgA->size][0];
            lineA[0][1] = pgA->vertex[j % pgA->size][1];
            lineA[1][0] = pgA->vertex[(j + 1) % pgA->size][0];
            lineA[1][1] = pgA->vertex[(j + 1) % pgA->size][1];
            if (cross(lineA, pointB) > 0) {
                positive++;
            }
        }
        if (positive == 0 || positive == pgA->size) {
            return true;
        }
    }
    return false;
}

double cross(double (*line)[2], double *point) {
    return ((line[1][0] - line[0][0]) * (line[0][1] - point[1]) -
            (line[1][1] - line[0][1]) * (line[0][0] - point[0]));
}

bool onSegment(double (*line)[2], double *point) {
    return dist(line[0], line[1]) == dist(line[0], point) + dist(point, line[1]);
}

double dist(double *point1, double *point2) {
    return (point2[0] - point1[0]) * (point2[0] - point1[0]) +
           (point2[1] - point1[1]) * (point2[1] - point1[1]);
}

// int main(void) {
//     polygon_t square = {0};
//     polygon_t tr = {0};
//
//     square.size = 4;
//     tr.size = 3;
//
//     square.vertex[0][0] = 0; square.vertex[0][1] = 0;
//     square.vertex[1][0] = 0; square.vertex[1][1] = 5;
//     square.vertex[2][0] = 5; square.vertex[2][1] = 5;
//     square.vertex[3][0] = 5; square.vertex[3][1] = 0;
//
//     tr.vertex[0][0] = 1; tr.vertex[0][1] = 1;
//     tr.vertex[1][0] = 2.5; tr.vertex[1][1] = 4;
//     tr.vertex[2][0] = 4; tr.vertex[2][1] = 1;
//
//
//     tr.x = 0; tr.y = 0; tr.rot = 3.14*5/180;
//
//     printf("collsion:%d\n", is_collide(&square, &tr));
//     printf("contain:%d\n", is_contain(&square, &tr));
// }
