#ifndef CHASE_H
#define CHASE_H

#include "../graphics/bmp.h"
#include "primitives.h"
#include <stdbool.h>

typedef struct agent {
    bool is_runner;
    double x;
    double y;
    double theta;
    double vel;
    double ang_vel;
    color_bgr_t color;
} agent_t;

typedef struct search_node {
    int depth;
    agent_t runner;
    agent_t chaser;
} search_node_t;

typedef struct state {
    int time_step;
    bitmap_t *bmp;
    size_t image_size;
    uint8_t *image_data;

    bool runner_caught;

    agent_t runner;
    agent_t chaser;
} state_t;

typedef struct world {
    char *map;
    polygon_t *wall;
} world_t;

#endif
