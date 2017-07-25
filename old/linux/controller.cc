#include "controller.h"
#include "utils.h"

int Controller::transform_steer(double value) {
    if (value < -1 || value > 1) {
        print_error("Invalid steering angle - range is -1 to 1");
        return -1;
    }

    value = (value + 1)/2;
    return (SERVO_UPPER_BOUND - SERVO_LOWER_BOUND) * value + SERVO_LOWER_BOUND;
}

int Controller::transform_brake(double value) {
    if (value < 0 || value > 1) {
        print_error("Invalid target brake position - range is 0 to 1");
        return -1;
    }

    return (SERVO_UPPER_BOUND - SERVO_LOWER_BOUND) * value + SERVO_LOWER_BOUND;
}
