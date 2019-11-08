#ifndef WORLD_H
#define WORLD_H

#include "geometry.h"
#include "bmp.h"

#define NUM_LAMPS 3
#define NUM_ROBOTS 1

typedef struct robot {
    pose_t Pose;
    dir_vector_t eye_l;
    dir_vector_t eye_r;
    polygon_t Shape;
    polygon_t collision_shape;
    color_bgr_t color;
    int max_l_speed;
    int max_r_speed;
    double wheel_base;
} robot_t;

typedef struct lamp {
    pose_t Pose;
    polygon_t Shape;
    color_bgr_t color;
    double power;
} lamp_t;

typedef struct world {
    int n_lamps;
    lamp_t lamp[NUM_LAMPS];
    robot_t robot[NUM_ROBOTS];
} world_t;

#endif
