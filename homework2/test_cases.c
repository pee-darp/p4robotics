#include "bmp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "image_server.h"
#include "bres_line.h"

#define FILL true
#define NO_FILL false
#define PI 3.1415926535897

void black_img_T1(bitmap_t *bmp) {
    color_bgr_t black = {0, 0, 0};
    for (int row = 0; row < bmp->height; row++) {
        for (int col = 0; col < bmp->width; col++) {
            bmp->data[row * bmp->width + col] = black;
        }
    }
}

void line_T2(bitmap_t *bmp) {
    vector_t path;
    vector_init(&path);
    path = *plotLine(10, 10, 200, 200, &path, bmp);
    free(path.data);
}

void four_by_four_T3(bitmap_t *bmp) {
    vector_t path, rnd_square;
    codList_t square;
    vector_init(&path);
    square = *calcRect(&square, 4, 4);
    rnd_square = *roundRect(&square, &rnd_square);
    plotPoly(&rnd_square, &path, bmp, NO_FILL);
    free(path.data);
    free(rnd_square.data);
    free(square.data);
}

void four_by_four_centered_T4(bitmap_t *bmp) {
    vector_t path, rnd_square;
    codList_t square;
    vector_init(&path);
    square = *calcRect(&square, 4, 4);
    square = *transPoly(&square, 2, 2);
    rnd_square = *roundRect(&square, &rnd_square);
    plotPoly(&rnd_square, &path, bmp, NO_FILL);
    free(path.data);
    free(rnd_square.data);
    free(square.data);
}

void five_by_five_T5(bitmap_t *bmp) {
    vector_t path, rnd_square;
    codList_t square;
    vector_init(&path);
    square = *calcRect(&square, 5, 5);
    square = *transPoly(&square, 2, 2);
    rnd_square = *roundRect(&square, &rnd_square);
    plotPoly(&rnd_square, &path, bmp, NO_FILL);
    free(path.data);
    free(rnd_square.data);
    free(square.data);
}

void border_T6(bitmap_t *bmp) {
    vector_t path, rnd_border;
    codList_t border;
    vector_init(&path);
    border = *calcRect(&border, 600, 440);
    border = *transPoly(&border, 320, 240);
    rnd_border = *roundRect(&border, &rnd_border);
    plotPoly(&rnd_border, &path, bmp, NO_FILL);
    free(path.data);
    free(rnd_border.data);
    free(border.data);
}

void fill_border_T7(bitmap_t *bmp) {
    vector_t path, rnd_border;
    codList_t border;
    vector_init(&path);
    border = *calcRect(&border, 600, 440);
    border = *transPoly(&border, 320, 240);
    rnd_border = *roundRect(&border, &rnd_border);
    plotPoly(&rnd_border, &path, bmp, FILL);
    free(path.data);
    free(rnd_border.data);
    free(border.data);
}

void robot_triangle_T8(bitmap_t *bmp) {
    vector_t path, r_triangle;
    codList_t triangle;
    vector_init(&path);
    triangle = *calcTriangle(&triangle, 21);
    triangle = *rotPoly(&triangle, 0);
    triangle = *transPoly(&triangle, 400, 400);
    //print_cod(&triangle);
    r_triangle = *roundRect(&triangle, &r_triangle);
    plotPoly(&r_triangle, &path, bmp, FILL);
    free(path.data);
    free(r_triangle.data);
    free(triangle.data);
}

void robot_triangle_rot_T9(bitmap_t *bmp) {
    vector_t path, r_triangle;
    codList_t triangle;
    vector_init(&path);
    triangle = *calcTriangle(&triangle, 21);
    //print_cod(&triangle);
    triangle = *rotPoly(&triangle, -PI / 6);
    //print_cod(&triangle);
    triangle = *transPoly(&triangle, 400, 400);
    print_cod(&triangle);
    r_triangle = *roundRect(&triangle, &r_triangle);
    print_vec(&r_triangle);
    plotPoly(&r_triangle, &path, bmp, FILL);
    free(path.data);
    free(r_triangle.data);
    free(triangle.data);
}
