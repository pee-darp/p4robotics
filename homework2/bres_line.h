#ifndef BRES_LINE_H
#define BRES_LINE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp.h"
#include "vector.h"

vector_t *plotLine_q1(int x0, int y0, int x1, int y1, vector_t *path,
                      bool plot, bitmap_t *bmp, color_bgr_t color);
vector_t *plotLine_q2(int x0, int y0, int x1, int y1, vector_t *path,
                      bitmap_t *bmp, color_bgr_t color);
vector_t *plotLine_q3(int x0, int y0, int x1, int y1, vector_t *path,
                      bitmap_t *bmp, color_bgr_t color);
vector_t *plotLine_q4(int x0, int y0, int x1, int y1, vector_t *path,
                      bitmap_t *bmp, color_bgr_t color);

vector_t *plotLine(int x0, int y0, int x1, int y1, vector_t *path,
                   bitmap_t *bmp, color_bgr_t color);

codList_t *calcRect(codList_t *rect, double width, double height);
codList_t *calcTriangle(codList_t *triangle, double size);
vector_t *roundRect(codList_t *rect, vector_t *rnd_rect);

void plotPoly(vector_t *polygon, vector_t *path, bitmap_t *bmp, bool is_filled,
              color_bgr_t color);
codList_t *transPoly(codList_t *polygon, double x_trans, double y_trans);
codList_t *rotPoly(codList_t *polygon, double rotation);

void set_pixel(bitmap_t *bmp, int x, int y);
void set_color_pixel(bitmap_t *bmp, int x, int y, color_bgr_t color);

#endif
