#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vixel.h"
#include "vector.h"
#include "bmp.h"
#include "image_server.h"
#include <unistd.h>
#include "../structures/primitives.h"
#include "graphics.h"
#include <stdbool.h>

void init_bmp(bitmap_t *bmp, int width, int height) {
    bmp->width = width;
    bmp->height = height;
    bmp->data = calloc(bmp->width * bmp->height, sizeof(color_bgr_t));
}

void gx_set_pixel(bitmap_t *bmp, pixel_t *pixel, color_bgr_t color) {
    //printf("%d | %d\n", pixel->x, pixel->y);
    if (pixel->x >= 0 && pixel->y >= 0) {
        bmp->data[bmp->width * bmp->height -
                  bmp->width - pixel->y * bmp->width + pixel->x] = color;
    }
}

void gx_draw_vixel(bitmap_t *bmp, vixel_t *line, color_bgr_t color) {
    for (int i = 0; i < line->size; i++) {
        gx_set_pixel(bmp, &line->data[i], color);
    }
}

void gx_clear(bitmap_t *bmp, color_bgr_t color) {
    for (int row = 0; row < bmp->height; row++) {
        for (int col = 0; col < bmp->width; col++) {
            bmp->data[row * bmp->width + col] = color;
        }
    }
}

vixel_t *gx_rasterize_line(int x0, int y0, int x1, int y1) {
    vixel_t *line = malloc(sizeof(*line));
    vixel_init(line);
    int dx, sx, dy, sy, err, e2;
    dx = abs(x1 - x0);
    sx = x0 < x1 ? 1 : -1;
    dy = -abs(y1 - y0);
    sy = y0 < y1 ? 1 : -1;
    err = dx + dy;
    vixel_add(line, x0, y0);
    while (1) {
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
        vixel_add(line, x0, y0);
    }
    //print_vix(line);
    return line;
}

void gx_draw_line(bitmap_t *bmp, int x0, int y0, int x1, int y1, color_bgr_t color) {
    vixel_t *points = gx_rasterize_line(x0, y0, x1, y1);
    gx_draw_vixel(bmp, points, color);
    vixel_free(points);
}

vixel_t *gx_rasterize_poly(polygon_t *poly) {
    vixel_t *rnd_poly = malloc(sizeof(*rnd_poly));
    vixel_init(rnd_poly);
    double x_min = 1000.0;
    double y_min = 1000.0;
    double epsilon = 0.000001;
    int32_t x = 0;
    int32_t y = 0;
    //find minimum x and y coordinate
    //apply trasnformations first
    for (int i = 0; i < poly->size; i++) {
        if (poly->vertex[i][0] < x_min) {
            x_min = poly->vertex[i][0];
        }
        if (poly->vertex[i][1] < y_min) {
            y_min = poly->vertex[i][1];
        }
    }

    //rounding the polygon
    for (int i = 0; i < poly->size; i++) {
        if (poly->vertex[i][0] == x_min) {
            x = ceil(poly->vertex[i][0]);
        } else {
            x = floor(poly->vertex[i][0] - epsilon);
        }
        if (poly->vertex[i][1] == y_min) {
            y = ceil(poly->vertex[i][1]);
        } else {
            y = floor(poly->vertex[i][1] - epsilon);
        }
        vixel_add(rnd_poly, x, y);
    }

    return rnd_poly;
}

void gx_draw_poly(bitmap_t *bmp, polygon_t *poly, bool is_filled, color_bgr_t color) {
    if (is_filled) {
        //draw polygon if filling is enabled
        vixel_t *rnd_poly = gx_rasterize_poly(poly);
        vixel_t buffer = {0};
        vixel_init(&buffer);
        for (int i = 0; i < bmp->height; i++) {
            vixel_add(&buffer, -1, -1);
        }
        for (int i = 0; i < rnd_poly->size; i++) {
            vixel_t *points = NULL;
            points = gx_rasterize_line(rnd_poly->data[i].x, rnd_poly->data[i].y,
                                       rnd_poly->data[(i + 1) % rnd_poly->size].x,
                                       rnd_poly->data[(i + 1) % rnd_poly->size].y);
            for (int j = 0; j < points->size; j++) {
                if (buffer.data[points->data[j].y].x == -1) {
                    buffer.data[points->data[j].y].x = points->data[j].x;
                    buffer.data[points->data[j].y].y = points->data[j].x;
                } else {
                    if (buffer.data[points->data[j].y].x > points->data[j].x) {
                        buffer.data[points->data[j].y].x = points->data[j].x;
                    }
                    if (buffer.data[points->data[j].y].y < points->data[j].x) {
                        buffer.data[points->data[j].y].y = points->data[j].x;
                    }
                }
            }
            for (int j = 0; j < buffer.size; j++) {
                gx_draw_line(bmp, buffer.data[j].x, j, buffer.data[j].y, j, color);
            }
            gx_draw_vixel(bmp, points, color);
            vixel_free(points);
        }
        free(buffer.data);
        vixel_free(rnd_poly);
    } else {
        //draw the polygon if filling is unabled
        vixel_t *rnd_poly = gx_rasterize_poly(poly);
        for (int i = 0; i < rnd_poly->size; i++) {
            gx_draw_line(bmp, rnd_poly->data[i].x, rnd_poly->data[i].y,
                         rnd_poly->data[(i + 1) % rnd_poly->size].x,
                         rnd_poly->data[(i + 1) % rnd_poly->size].y,
                         color);
        }
        vixel_free(rnd_poly);
    }
}

polygon_t *gx_make_rect(bitmap_t *bmp, double width, double height) {
    polygon_t *poly = malloc(sizeof(*poly));
    poly->size = 4;
    poly->vertex[0][0] = -width / 2;
    poly->vertex[0][1] = -height / 2;
    poly->vertex[1][0] = -width / 2;
    poly->vertex[1][1] = height / 2;
    poly->vertex[2][0] = width / 2;
    poly->vertex[2][1] = height / 2;
    poly->vertex[3][0] = width / 2;
    poly->vertex[3][1] = -height / 2;
    return poly;
}

polygon_t *gx_make_triangle(bitmap_t *bmp, double size) {
    double ratio = 4.0 / 3.0;
    polygon_t *poly = malloc(sizeof(*poly));
    poly->size = 3;
    poly->vertex[0][0] = -(ratio * size) / 2;
    poly->vertex[0][1] = -size / 2;
    poly->vertex[1][0] = -ratio * size / 2;
    poly->vertex[1][1] = size / 2;
    poly->vertex[2][0] = ratio * size / 2;
    poly->vertex[2][1] = 0;
    return poly;
}

void gx_trans_poly(polygon_t *poly, double x, double y) {
    poly->x += x;
    poly->y += y;
    for (int i = 0; i < poly->size; i++) {
        poly->vertex[i][0] += x;
        poly->vertex[i][1] += y;
    }
}

void gx_rot_poly(polygon_t *poly, double rot) {
    poly->rot = rot;
    double c = cos(poly->rot);
    double s = sin(poly->rot);
    double x_old = 0;
    double y_old = 0;
    for (int i = 0; i < poly->size; i++) {
        x_old = poly->vertex[i][0];
        y_old = poly->vertex[i][1];
        poly->vertex[i][0] = x_old * c + y_old * s;
        poly->vertex[i][1] = y_old * c - x_old * s;
    }
}

void gx_run(bitmap_t *bmp) {
    //graphics backend happens here
    size_t bmp_size = bmp_calculate_size(bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(bmp, serialized_bmp);
    //writing bitmap to a file
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);
    //serving the image on an image server
    //image_server_set_data(bmp_size, serialized_bmp);
    //image_server_start("8000");
    //sleep(1);
    free(serialized_bmp);
}
