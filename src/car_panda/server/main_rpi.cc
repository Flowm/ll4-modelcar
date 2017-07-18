#include "mqtt_entity.h"
#include "servo.h"
#include <unistd.h>
#include <iostream>
#include <string.h>

int main(int argc, char *argv[]) {
    char cmd[20];
    char *split, *channel, *target;

    Mqtt_Entity *mqtt_entity = new Mqtt_Entity("rpi", "car-state");
    Servo *servo = new Servo("/dev/ttyUSB0");

    mqtt_entity->my_subscribe("car-servo");

    while (true) {
        sem_wait(&mqtt_entity->msgSem);
        mqtt_entity->getCmd(cmd, sizeof(cmd));

        // Send to polulu
        split = strtok(cmd, ",");
        if (!split) {
            continue;
        }
        channel = split;

        target = strtok(NULL, ",");
        if (!target) {
            continue;
        }

        servo->maestroSetTarget(strtoul(channel, NULL, 0), strtoul(target, NULL, 0));
    }

    delete mqtt_entity;
    delete servo;

    return 0;
}
