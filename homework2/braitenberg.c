#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include "world.h"
#include "bmp.h"
#include "vector.h"
#include "colours.h"
#include "bres_line.h"
#include "geometry.h"
#include "image_server.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PI_BR 3.1415926535897
#define FILL true
#define NO_FILL false

void init_robot_shape(robot_t *robot);
void init_lamp_shape(world_t *world);
void draw_lamp(polygon_t *lamp, bitmap_t *bmp);
void draw_robot(robot_t *robot, bitmap_t *bmp);
void move_away(robot_t *robot, lamp_t *lamp, double distance);
double max(double a, double b);
double min(double a, double b);
void clear_graphics(bitmap_t *bmp);
void border(bitmap_t *bmp);
void update_graphics(robot_t *robot, world_t *world, bitmap_t *bmp);
void calc_robot_collision(robot_t *robot);
void fast_resolve_collsion(robot_t *robot, world_t *world);

void init_world(robot_t *robot, world_t *world) {
    world->n_lamps = 3;

    position_t robot_start_pos = {320, 240};
    position_t lamp_start_pos[3] = {{124.1, 224.1}, {349.1, 99.1}, {449.1, 349.1}};

    robot->Pose.center = robot_start_pos;
    robot->Pose.heading = 0;
    for (int i = 0; i < 3; i++) {
        world->lamp[i].Pose.center = lamp_start_pos[i];
        world->lamp[i].Pose.heading = 45;
        world->lamp[i].color = yellow;
        world->lamp[i].power = 100000;
    }

    dir_from_angle(-60, &robot->eye_l);
    dir_from_angle(60, &robot->eye_r);

    init_robot_shape(robot); //remember to free malloced variables
    init_lamp_shape(world); // remember to  free malloced variables

    robot->color = green;

    robot->max_l_speed = 12;
    robot->max_r_speed = 12;

    robot->wheel_base = 80;
}

void border(bitmap_t *bmp) {
    vector_t path, rnd_border;
    codList_t border;
    vector_init(&path);
    border = *calcRect(&border, 600, 440);
    border = *transPoly(&border, 320, 240);
    rnd_border = *roundRect(&border, &rnd_border);
    plotPoly(&rnd_border, &path, bmp, NO_FILL, white);
    free(path.data);
    free(rnd_border.data);
    free(border.data);
}

void init_graphics(robot_t *robot, world_t *world, bitmap_t *bmp) {
    for (int i = 0; i < world->n_lamps; i++) {
        draw_lamp(&world->lamp[i].Shape, bmp);
    }
    draw_robot(robot, bmp);
}

void move(robot_t *robot, world_t *world) {
    double move_l = 0;
    double move_r = 0;
    dir_vector_t dv = {0};

    for (int i = 0; i < world->n_lamps; i++) {
        //calculate the squared distance
        //printf("----lamp %d----\n", i);
        double sqr_dist = sq_dist(&robot->Pose.center, &world->lamp[i].Pose.center);
        //printf("sq_dist:%.2f\n", sqr_dist);
        //store the direction vector into dv
        dir_from_pts(&world->lamp[i].Pose.center, &robot->Pose.center, &dv);
        //printf("dv:%.2fi + %.2fj\n", dv.x, dv.y);
        //store the sensor angles in degrees
        dir_from_angle(robot->Pose.heading + 60, &robot->eye_l); //swapping signs due to computer's
        dir_from_angle(robot->Pose.heading - 60, &robot->eye_r); //notation of axis
        move_l += max(0.0, dot(&dv, &robot->eye_r)) * world->lamp[i].power / sqr_dist;
        move_r += max(0.0, dot(&dv, &robot->eye_l)) * world->lamp[i].power / sqr_dist;
        //printf("move_l: %.2f   |   move_r: %.2f\n", move_l, move_r);
        //printf("\n");
    }
    //printf("L:%f | R:%f\n", move_l, move_r);

    move_l = min(robot->max_l_speed, move_l);
    move_r = min(robot->max_r_speed, move_r);

    robot->Pose.heading += ((move_r - move_l) / robot->wheel_base) * 180 / PI_BR;
    //printf("Heading:%.2f\n", robot->Pose.heading);
    double forward_dist = (move_l + move_r) / 2;
    double x_fwd = forward_dist * cos(PI_BR * (robot->Pose.heading) / 180);
    double y_fwd = forward_dist * sin(PI_BR * (robot->Pose.heading) / 180);
    robot->Pose.center.x += x_fwd;
    robot->Pose.center.y += y_fwd;
    //printf("Pose: x=%f | y=%f\n", robot->Pose.center.x, robot->Pose.center.y);
}

void resolve_collision(robot_t *robot, world_t *world, bitmap_t *bmp) {
    for (int i = 0; i < world->n_lamps; i++) {
        while (is_collide(&robot->collision_shape, &world->lamp[i].Shape)) {
            printf("Collision!\n");
            move_away(robot, &world->lamp[i], 0.5);
            update_graphics(robot, world, bmp);
        }
    }
}

void fast_resolve_collsion(robot_t *robot, world_t *world) {
    for (int i = 0; i < world->n_lamps; i++) {
        while (is_collide(&robot->collision_shape, &world->lamp[i].Shape)) {
            printf("Collision!\n");
            move_away(robot, &world->lamp[i], 0.5);
            calc_robot_collision(robot);
        }
    }
}

void update_graphics(robot_t *robot, world_t *world, bitmap_t *bmp) {
    clear_graphics(bmp);
    border(bmp);
    for (int i = 0; i < world->n_lamps; i++) {
        draw_lamp(&world->lamp[i].Shape, bmp);
    }
    draw_robot(robot, bmp);
}

void init_robot_shape(robot_t *robot) {
    vector_t path_robot;
    codList_t triangle;
    vector_init(&path_robot);
    triangle = *calcTriangle(&triangle, 21);
    //triangle = *rotPoly(&triangle, 0);
    //triangle = *transPoly(&triangle, 400, 400);
    point_t p1 = {triangle.data[0].x, triangle.data[0].y};
    point_t p2 = {triangle.data[1].x, triangle.data[1].y};
    point_t p3 = {triangle.data[2].x, triangle.data[2].y};
    line_t side1 = {p1, p3};
    line_t side2 = {p3, p2};
    line_t side3 = {p2, p1};
    polygon_t rob_poly = {3, {side1, side2, side3}, 400, 400, 0};
    robot->Shape = rob_poly;
    robot->collision_shape = rob_poly;
    free(path_robot.data);
    free(triangle.data);
}

void init_lamp_shape(world_t *world) {
    for (int i = 0; i < world->n_lamps; i++) {
        vector_t path_lamp;
        codList_t square;
        vector_init(&path_lamp);
        square = *calcRect(&square, 12, 12);
        square = *rotPoly(&square, 45 * PI_BR / 180);
        square = *transPoly(&square, world->lamp[i].Pose.center.x, world->lamp[i].Pose.center.y);
        point_t p1 = {square.data[0].x, square.data[0].y};
        point_t p2 = {square.data[1].x, square.data[1].y};
        point_t p3 = {square.data[2].x, square.data[2].y};
        point_t p4 = {square.data[3].x, square.data[3].y};
        line_t side1 = {p1, p2};
        line_t side2 = {p2, p3};
        line_t side3 = {p3, p4};
        line_t side4 = {p4, p1};
        // polygon_t lamp = {4, {side1, side2, side3, side4},
        //                   world->lamp[i].Pose.center.x,
        //                   world->lamp[i].Pose.center.y, 45 * PI_BR / 180};
        polygon_t lamp = {0};
        lamp.size = 4;
        lamp.side[0] = side1;
        lamp.side[1] = side2;
        lamp.side[2] = side3;
        lamp.side[3] = side4;
        lamp.x_glob = world->lamp[i].Pose.center.x;
        lamp.y_glob = world->lamp[i].Pose.center.y;
        lamp.rot = 45 * PI_BR / 180;
        world->lamp[i].Shape = lamp;
        free(path_lamp.data);
        free(square.data);
    }
}

void draw_lamp(polygon_t *lamp, bitmap_t *bmp) {
    codList_t lamp_vector;
    vector_t r_lamp_vector, path;
    vector_init(&path);
    cod_init(&lamp_vector);
    for (int i = 0; i < lamp->size; i++) {
        cod_append(&lamp_vector, lamp->side[i].p1.x, lamp->side[i].p1.y);
    }
    r_lamp_vector = *roundRect(&lamp_vector, &r_lamp_vector);
    plotPoly(&r_lamp_vector, &path, bmp, FILL, yellow);
    free(lamp_vector.data);
    free(r_lamp_vector.data);
    free(path.data);
}

void draw_robot(robot_t *robot, bitmap_t *bmp) {
    codList_t robot_vector;
    vector_t r_robot_vector, path;
    vector_init(&path);
    cod_init(&robot_vector);
    for (int i = 0; i < robot->Shape.size; i++) {
        cod_append(&robot_vector, robot->Shape.side[i].p1.x, robot->Shape.side[i].p1.y);
    }
    robot_vector = *rotPoly(&robot_vector, (robot->Pose.heading * PI_BR / 180));
    robot_vector = *transPoly(&robot_vector, robot->Pose.center.x, robot->Pose.center.y);
    r_robot_vector = *roundRect(&robot_vector, &r_robot_vector);
    for (int j = 0; j < r_robot_vector.size - 1; j++) {
        robot->collision_shape.side[j].p1.x = robot_vector.data[j].x;
        robot->collision_shape.side[j].p1.y = robot_vector.data[j].y;
        robot->collision_shape.side[j].p2.x = robot_vector.data[j + 1].x;
        robot->collision_shape.side[j].p2.y = robot_vector.data[j + 1].y;
    }
    robot->collision_shape.side[3].p1.x = robot_vector.data[3].x;
    robot->collision_shape.side[3].p1.y = robot_vector.data[3].y;
    robot->collision_shape.side[3].p2.x = robot_vector.data[0].x;
    robot->collision_shape.side[3].p2.y = robot_vector.data[0].y;
    //print_vec(&r_robot_vector);
    plotPoly(&r_robot_vector, &path, bmp, FILL, robot->color);
    free(robot_vector.data);
    free(r_robot_vector.data);
    free(path.data);
}

void calc_robot_collision(robot_t *robot) {
    codList_t robot_vector;
    vector_t r_robot_vector, path;
    vector_init(&path);
    cod_init(&robot_vector);
    for (int i = 0; i < robot->Shape.size; i++) {
        cod_append(&robot_vector, robot->Shape.side[i].p1.x, robot->Shape.side[i].p1.y);
    }
    robot_vector = *rotPoly(&robot_vector, (robot->Pose.heading * PI_BR / 180));
    robot_vector = *transPoly(&robot_vector, robot->Pose.center.x, robot->Pose.center.y);
    r_robot_vector = *roundRect(&robot_vector, &r_robot_vector);
    for (int j = 0; j < r_robot_vector.size - 1; j++) {
        robot->collision_shape.side[j].p1.x = robot_vector.data[j].x;
        robot->collision_shape.side[j].p1.y = robot_vector.data[j].y;
        robot->collision_shape.side[j].p2.x = robot_vector.data[j + 1].x;
        robot->collision_shape.side[j].p2.y = robot_vector.data[j + 1].y;
    }
    robot->collision_shape.side[3].p1.x = robot_vector.data[3].x;
    robot->collision_shape.side[3].p1.y = robot_vector.data[3].y;
    robot->collision_shape.side[3].p2.x = robot_vector.data[0].x;
    robot->collision_shape.side[3].p2.y = robot_vector.data[0].y;
    free(robot_vector.data);
    free(r_robot_vector.data);
    free(path.data);
}

void move_away(robot_t *robot, lamp_t *lamp, double distance) {
    dir_vector_t dv = {0};
    dir_from_pts(&lamp->Pose.center, &robot->Pose.center, &dv);
    //robot->Pose.center.x -= distance * cos(PI_BR * (robot->Pose.heading) / 180);
    robot->Pose.center.x -= distance * dv.x;
    robot->Pose.center.y -= distance * dv.y;
    //robot->Pose.center.y -= distance * sin(PI_BR * (robot->Pose.heading) / 180);
}

void clear_graphics(bitmap_t *bmp) {
    for (int row = 0; row < bmp->height; row++) {
        for (int col = 0; col < bmp->width; col++) {
            bmp->data[row * bmp->width + col] = black;
        }
    }
}

double max(double a, double b) {
    return (a >= b) ? a : b;
}

double min(double a, double b) {
    return (a <= b) ? a : b;
}

int main(int argc, char *argv[]) {
    int args_read = 0;
    int fast_mode = 0;
    int time_steps = 0;
    args_read = sscanf(argv[1], "%d", &time_steps);
    if (args_read <= 0) {
        fprintf(stderr, "usage: ./braitenberg <time steps> <fast=0|1>\n");
        return 1;
    }
    args_read = sscanf(argv[2], "%d", &fast_mode);
    if (args_read <= 0) {
        fprintf(stderr, "usage: ./braitenberg <time steps> <fast=0|1>\n");
        return 1;
    }

    robot_t robot = {0};
    world_t world = {0};
    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    if (fast_mode == 0) {
        int seconds = 0;
        long nanoseconds = 40 * 1000 * 1000;
        struct timespec interval = {seconds, nanoseconds};
        init_world(&robot, &world);
        init_graphics(&robot, &world, &bmp);
        for (int t_step = 0; t_step < time_steps; t_step++) {
            move(&robot, &world);
            resolve_collision(&robot, &world, &bmp);
            update_graphics(&robot, &world, &bmp);
            bmp_serialize(&bmp, serialized_bmp);
            //writing bitmap to a file
            FILE *f = fopen("braitenberg.bmp", "wb");
            fwrite(serialized_bmp, bmp_size, 1, f);
            fclose(f);
            //serving the image on an image server
            image_server_set_data(bmp_size, serialized_bmp);
            image_server_start("8000");
            nanosleep(&interval, NULL);
        }
        free(bmp.data);
        free(serialized_bmp);
    } else {
        init_world(&robot, &world);
        calc_robot_collision(&robot);
        for (int t_step = 0; t_step < time_steps; t_step++) {
            move(&robot, &world);
            calc_robot_collision(&robot);
            fast_resolve_collsion(&robot, &world);
        }
        update_graphics(&robot, &world, &bmp);
        bmp_serialize(&bmp, serialized_bmp);
        //writing bitmap to a file
        FILE *f = fopen("braitenberg.bmp", "wb");
        fwrite(serialized_bmp, bmp_size, 1, f);
        fclose(f);
        //serving the image on an image server
        image_server_set_data(bmp_size, serialized_bmp);
        image_server_start("8000");
        sleep(1);
        free(bmp.data);
        free(serialized_bmp);
    }
    //free all malloced memory
    return 0;
}
