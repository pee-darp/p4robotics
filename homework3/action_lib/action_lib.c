#include "../structures/chase.h"
#include "action_lib.h"

void a1(agent_t *robot) {
    //do nothing
}

void a2(agent_t *robot) {
    if (robot->vel <= 10) {
        robot->vel += 2;
    }
}

void a3(agent_t *robot) {
    robot->ang_vel += 3.141592653589 / 16;
}

void a4(agent_t *robot) {
    robot->ang_vel -= 3.141592653589 / 16;
}

void apply_action(int action, agent_t *robot) {
    switch (action) {
    case 1:
        a1(robot);
        break;
    case 2:
        a2(robot);
        break;
    case 3:
        a3(robot);
        break;
    case 4:
        a4(robot);
        break;
    default:
        return;
    }
}
