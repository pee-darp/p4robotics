#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vixel.h"
#include "vector.h"
#include "graphics.h"
#include "image_server.h"
#include <unistd.h>
#include "../structures/primitives.h"
#include "../physics/collision.h"

#define M_PI 3.14159
#define FILL 1
#define NO_FILL 0

color_bgr_t white = {255, 255, 255};
color_bgr_t black = {0, 0, 0};
color_bgr_t green = {0, 255, 0};

int main(void) {
    //initializing bitmap
    bitmap_t *bmp = malloc(sizeof(*bmp));
    init_bmp(bmp, 640, 480);

    //all graphic designing to be done here
    gx_clear(bmp, black);
    polygon_t *rect = gx_make_rect(bmp, 5, 5);
    gx_rot_poly(rect, 0);
    gx_trans_poly(rect, 50, 50);
    polygon_t *triangle = gx_make_triangle(bmp, 21);
    gx_rot_poly(triangle, 35 * M_PI / 180);
    gx_trans_poly(triangle, 50, 50);
    gx_trans_poly(triangle, -16.4, 0);

    printf("collsion:%d\n", is_collide(rect, triangle));
    printf("contain:%d\n", is_contain(rect, triangle));
    gx_draw_poly(bmp, rect, FILL, white);
    gx_draw_poly(bmp, triangle, FILL, white);
    //graphics designing ends

    //run the graphics engine
    gx_run(bmp);

    //free all malloced memory
    free(bmp->data);
    free(bmp);
    free(rect);
    free(triangle);
    return 0;
}
