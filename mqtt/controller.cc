#include "controller.h"

int Controller::transform_steer(double value) {
    if (value < -1 || value > 1) {
        return -1;
    }

    value = (value + 1)/2;
    return (SERVO_UPPER_BOUND - SERVO_LOWER_BOUND) * value + SERVO_LOWER_BOUND;
}

int Controller::transform_brake(double value) {
    if (value < 0 || value > 1) {
        return -1;
    }

    return (SERVO_UPPER_BOUND - SERVO_LOWER_BOUND) * value + SERVO_LOWER_BOUND;
}
