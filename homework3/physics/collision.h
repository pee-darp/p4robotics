#include <stdbool.h>
#include "../structures/primitives.h"

bool is_collide(polygon_t *pg1, polygon_t *pg2);
bool is_contain(polygon_t *pg1, polygon_t *pg2);
void rotPoly(polygon_t *pg);
void transPoly(polygon_t *pg);
void TF_Poly(polygon_t *pg);
double cross(double (*line)[2], double *point);
bool onSegment(double (*line)[2], double *point);
double dist(double *point1, double *point2);
