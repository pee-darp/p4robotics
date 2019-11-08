#include "bmp.h"
#include "../structures/primitives.h"
#include "vixel.h"
#include "vector.h"
#include <stdbool.h>

void init_bmp(bitmap_t *bmp, int width, int height);

void gx_set_pixel(bitmap_t *bmp, pixel_t *pixel, color_bgr_t color);

void gx_draw_vixel(bitmap_t *bmp, vixel_t *rasterized_line, color_bgr_t color);

void gx_clear(bitmap_t *bmp, color_bgr_t color);

vixel_t *gx_rasterize_line(int x0, int y0, int x1, int y1);

void gx_draw_line(bitmap_t *bmp, int x0, int y0, int x1, int y1, color_bgr_t color);

vixel_t *gx_rasterize_poly(polygon_t *poly);

void gx_draw_poly(bitmap_t *bmp, polygon_t *poly, bool is_filled, color_bgr_t color);

polygon_t *gx_make_rect(bitmap_t *bmp, double width, double height);

polygon_t *gx_make_triangle(bitmap_t *bmp, double size);

void gx_trans_poly(polygon_t *poly, double x, double y);

void gx_rot_poly(polygon_t *poly, double rot);

void gx_set_pos_poly(polygon_t *poly, double x, double y);

void gx_set_ori_poly(polygon_t *poly, double rot);

void gx_run(bitmap_t *bmp);
