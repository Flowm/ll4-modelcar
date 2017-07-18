#include "mqtt_entity.h"
#include "controller.h"
#include <unistd.h>
#include <iostream>
#include <string.h>

#define STEER_CHANNEL "6"
#define BRAKE_LEFT_FRONT_CHANNEL "1"
#define BRAKE_RIGHT_FRONT_CHANNEL "2"
#define BRAKE_REAR_CHANNEL "0"

static enum IDs {
    STEER = 0; // [-1;1]
    BRAKE = 1; // [0;1]
    ACCEL = 2; // [0,1]
};


int main(int argc, char *argv[]) {
    char recv_cmd[50];
    char servo_cmd[10];
    
    char *split, *id, *target; 
    double value;
    int servoVal;

    Mqtt_Entity *mqtt_entity = new Mqtt_Entity("car-servo");
    Controller *controller = new Controller();

    mqtt_entity->my_subscribe("car-control");

    while (true) {
        sem_wait(&mqtt_entity->msgSem);
        mqtt_entity->getCmd(recv_cmd, sizeof(recv_cmd));

        split = strtok(recv_cmd, ",");
        if (!split) {
            continue;
        }
        id = split;

        target = strtok(NULL, ",");
        if (!target) {
            continue;
        }
        value = stod(target);

        switch (strtoul(id, NULL, 0)) {
            case STEER :
                servoVal = controller->transform_steer(value);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", STEER_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                break;
            case BRAKE :
                servoVal = controller->tranfrom_brake(value);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", BRAKE_LEFT_FRONT_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", BRAKE_RIGHT_FRONT_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", BRAKE_REAR_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                break;
            case ACCEL :
                //servoVal = controller->transform_accel(value);
                break;
            default :
                servoVal = -1;
                break;              
        }
    }

    delete mqtt_entity;
    delete controller;

    return 0;
}