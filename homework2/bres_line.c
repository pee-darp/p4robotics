#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp.h"
#include "vector.h"

#define PI 3.1415

void set_pixel(bitmap_t *bmp, int x, int y) {
    if (x >= 0 && y >= 0) {
        // bmp->data[x + y * bmp->width].r = 255;
        // bmp->data[x + y * bmp->width].g = 255;
        // bmp->data[x + y * bmp->width].b = 255;
        bmp->data[bmp->width * bmp->height - bmp->width - y * bmp->width + x].r = 255;
        bmp->data[bmp->width * bmp->height - bmp->width - y * bmp->width + x].g = 255;
        bmp->data[bmp->width * bmp->height - bmp->width - y * bmp->width + x].b = 255;
    }
}

void set_color_pixel(bitmap_t *bmp, int x, int y, color_bgr_t color) {
    if (x >= 0 && y >= 0) {
        bmp->data[bmp->width * bmp->height - bmp->width - y * bmp->width + x] = color;
    }
}

vector_t *plotLine_q1(int x0, int y0, int x1, int y1, vector_t *path, bool plot,
                      bitmap_t *bmp, color_bgr_t color) {
    int dx, dy, p, x, y;
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;
    y = y0;
    vec_append(path, x, y);
    set_color_pixel(bmp, x, y, color);
    //vec_append(path, y);
    if (dx >= dy) {
        p = 2 * dy - dx;
        while (x < x1) {
            if (p >= 0) {
                y = y + 1;
                p = p + 2 * dy - 2 * dx;
            } else {
                p = p + 2 * dy;
            }
            //printf("%d | %d\n", x, y);
            x = x + 1;
            vec_append(path, x, y);
            if (plot) {
                set_color_pixel(bmp, x, y, color);
            }
            //vec_append(path, y);
        }
    } else {
        p = 2 * dx - dy;
        while (y < y1) {
            if (p >= 0) {
                x = x + 1;
                p = p + 2 * dx - 2 * dy;
            } else {
                p = p + 2 * dx;
            }
            //printf("%d | %d\n", x, y);
            y = y + 1;
            vec_append(path, x, y);
            if (plot) {
                set_color_pixel(bmp, x, y, color);
            }
            //vec_append(path, y);
        }
    }
    return path;
}

vector_t *plotLine_q2(int x0, int y0, int x1, int y1, vector_t *path,
                      bitmap_t *bmp, color_bgr_t color) {
    path = plotLine_q1(-x0, y0, -x1, y1, path, 0, bmp, color);
    for (int i = 0; i < path->size; i++) {
        path->data[i].x = -path->data[i].x;
        set_color_pixel(bmp, path->data[i].x, path->data[i].y, color);
    }
    return path;
}

vector_t *plotLine_q3(int x0, int y0, int x1, int y1, vector_t *path,
                      bitmap_t *bmp, color_bgr_t color) {
    path = plotLine_q1(-x0, -y0, -x1, -y1, path, 0, bmp, color);
    for (int i = 0; i < path->size; i++) {
        path->data[i].x = -path->data[i].x;
        path->data[i].y = -path->data[i].y;
        set_color_pixel(bmp, path->data[i].x, path->data[i].y, color);
    }
    return path;
}

vector_t *plotLine_q4(int x0, int y0, int x1, int y1, vector_t *path,
                      bitmap_t *bmp, color_bgr_t color) {
    path = plotLine_q1(x0, -y0, x1, -y1, path, 0, bmp, color);
    for (int i = 0; i < path->size; i++) {
        path->data[i].y = -path->data[i].y;
        set_color_pixel(bmp, path->data[i].x, path->data[i].y, color);
    }
    return path;
}

vector_t *plotLine(int x0, int y0, int x1, int y1, vector_t *path,
                   bitmap_t *bmp, color_bgr_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    if (dx >= 0) {
        if (dy >= 0) {
            path = plotLine_q1(x0, y0, x1, y1, path, 1, bmp, color);
        } else {
            path = plotLine_q4(x0, y0, x1, y1, path, bmp, color);
        }
    } else {
        if (dy >= 0) {
            path = plotLine_q2(x0, y0, x1, y1, path, bmp, color);
        } else {
            path = plotLine_q3(x0, y0, x1, y1, path, bmp, color);
        }
    }
    return path;
}

codList_t *calcRect(codList_t *rect, double width, double height) {
    cod_init(rect);
    cod_append(rect, -width / 2, -height / 2);
    cod_append(rect, -width / 2, height / 2);
    cod_append(rect, width / 2, height / 2);
    cod_append(rect, width / 2, -height / 2);
    return rect;
}

vector_t *roundRect(codList_t *rect, vector_t *rnd_rect) {
    vector_init(rnd_rect);
    double min_x = 10000000;
    double min_y = 10000000;
    double epsilon = 0.000001;
    int x, y;
    for (int i = 0; i < rect->size; i++) {
        if (rect->data[i].x < min_x) {
            min_x = rect->data[i].x;
        }
        if (rect->data[i].y < min_y) {
            min_y = rect->data[i].y;
        }
    }
    for (int j = 0; j < rect->size; j++) {
        if (rect->data[j].x == min_x) {
            x = ceil(rect->data[j].x);
        } else {
            x = floor(rect->data[j].x - epsilon);
        }
        if (rect->data[j].y == min_y) {
            y = ceil(rect->data[j].y - epsilon);
        } else {
            y = floor(rect->data[j].y - epsilon);
        }
        vec_append(rnd_rect, x, y);
    }
    return rnd_rect;
}

void plotPoly(vector_t *polygon, vector_t *path, bitmap_t *bmp, bool is_filled, color_bgr_t color) {
    vector_t x_min, x_max, n_path;
    vector_init(&x_min);
    vector_init(&x_max);
    vector_init(&n_path);
    for (int y = 0; y < bmp->height; y++) {
        vec_append(&x_min, 100000, y);
        vec_append(&x_max, -1, y);
    }
    for (int side = 0; side < polygon->size - 1; side++) {
        n_path = *plotLine(polygon->data[side].x, polygon->data[side].y,
                           polygon->data[side + 1].x, polygon->data[side + 1].y,
                           &n_path, bmp, color);
        if (is_filled) {
            //printf("side:%d | size:%d | cap:%d\n",side, n_path.size, n_path.cap);
            for (int i = 0; i < n_path.size; i++) {
                //printf("x:%d|y:%d\n",n_path.data[i].x, n_path.data[i].y);
                if (n_path.data[i].x < x_min.data[n_path.data[i].y].x) {
                    x_min.data[n_path.data[i].y].x = n_path.data[i].x;
                    //printf("x_min[%d]:%d\n",n_path.data[i].y, n_path.data[i].x );
                }
                if (n_path.data[i].x > x_max.data[n_path.data[i].y].x) {
                    x_max.data[n_path.data[i].y].x = n_path.data[i].x;
                    //printf("x_max[%d]:%d\n",n_path.data[i].y, n_path.data[i].x );
                }
            }
        }
        n_path.size = 0;
    }
    n_path = *plotLine(polygon->data[polygon->size - 1].x, polygon->data[polygon->size - 1].y,
                       polygon->data[0].x, polygon->data[0].y,
                       &n_path, bmp, color);
    //print_vec(&n_path);
    //print_vec(&x_max);
    if (is_filled) {
        for (int i = 0; i < n_path.size; i++) {
            //printf("%d > %d",n_path.data[i].x, x_max.data[n_path.data[i].y].x);
            if (n_path.data[i].x < x_min.data[n_path.data[i].y].x) {
                x_min.data[n_path.data[i].y].x = n_path.data[i].x;
                //printf("xx_min[%d]:%d\n",n_path.data[i].y, n_path.data[i].x );
            }
            if (n_path.data[i].x > x_max.data[n_path.data[i].y].x) {
                x_max.data[n_path.data[i].y].x = n_path.data[i].x;
                //printf("xx_max[%d]:%d\n",n_path.data[i].y, n_path.data[i].x );
            }
        }
        n_path.size = 0;

        //start filling the polygon here
        for (int i = 0; i < x_min.size; i++) {
            if (x_min.data[i].x != 10000 && x_max.data[i].x != -1) {
                n_path = *plotLine(x_min.data[i].x, x_min.data[i].y, x_max.data[i].x,
                                   x_max.data[i].y, &n_path, bmp, color);
            }
        }
    }
    free(x_min.data);
    free(x_max.data);
    free(n_path.data);
}

codList_t *calcTriangle(codList_t *triangle, double size) {
    double ratio = 4.0 / 3.0;
    cod_init(triangle);
    cod_append(triangle, -(ratio * size) / 2, -size / 2);
    cod_append(triangle, -ratio * size / 2, size / 2);
    cod_append(triangle, ratio * size / 2, 0);
    //print_cod(triangle);
    return triangle;
}

codList_t *rotPoly(codList_t *polygon, double rotation) {
    double c = cos(rotation);
    double s = sin(rotation);
    for (int i = 0; i < polygon->size; i++) {
        double x_old, y_old;
        x_old = polygon->data[i].x;
        y_old = polygon->data[i].y;
        polygon->data[i].x = x_old * c - y_old * s;
        polygon->data[i].y = y_old * c + x_old * s;
    }
    return polygon;
}

codList_t *transPoly(codList_t *polygon, double x_trans, double y_trans) {
    for (int i = 0; i < polygon->size; i++) {
        polygon->data[i].x += x_trans;
        polygon->data[i].y += y_trans;
    }
    return polygon;
}
