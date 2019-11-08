#include "bmp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "image_server.h"
#include "bres_line.h"
#include "test_cases.h"
#include "colours.h"

#define FILL true
#define NO_FILL false
#define PI 3.1415

int main(int argc, char *argv[]) {
    int args_read = 0;
    int test_case = 0;
    args_read = sscanf(argv[1], "%d", &test_case);
    if (args_read <= 0) {
        fprintf(stderr, "usage: ./rasterize <test case>\n");
        return 1;
    }
    //bitmap initialization
    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    //----test cases----//
    switch (test_case) {
    case 1:
        black_img_T1(&bmp);
        break;
    case 2:
        line_T2(&bmp);
        break;
    case 3:
        four_by_four_T3(&bmp);
        break;
    case 4:
        four_by_four_centered_T4(&bmp);
        break;
    case 5:
        five_by_five_T5(&bmp);
        break;
    case 6:
        border_T6(&bmp);
        break;
    case 7:
        fill_border_T7(&bmp);
        break;
    case 8:
        robot_triangle_T8(&bmp);
        break;
    case 9:
        robot_triangle_rot_T9(&bmp);
        //set_color_pixel(&bmp, 412, 393, red);
        //set_color_pixel(&bmp, 411, 393, blue);
        //set_color_pixel(&bmp, 410, 393, white);
        //set_color_pixel(&bmp, 409, 393, white);
        break;
    default:
        fprintf(stderr, "Invalid test case!\n");
    }
    //black_img_T1(&bmp);
    //line_T2(&bmp);
    //four_by_four_T3(&bmp);
    //four_by_four_centered_T4(&bmp);
    //five_by_five_T5(&bmp);
    //border_T6(&bmp);
    //fill_border_T7(&bmp);
    //robot_triangle_T8(&bmp);
    //robot_triangle_rot_T9(&bmp);
    //bitmap serialization
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    //writing bitmap to a file
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);
    //serving the image on an image server
    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);
    //free all malloced memory
    free(bmp.data);
    free(serialized_bmp);
    return 0;
}
