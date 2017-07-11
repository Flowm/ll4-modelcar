#include "mqtt_entity.h"
#include "servo.h"
#include <unistd.h>
#include <iostream>
#include <string.h>

int main(int argc, char *argv[]) {

    Mqtt_Entity *mqtt_entity = new Mqtt_Entity("topic/send");
    Servo *servo = new Servo("/dev/ttyUSB0");

    mqtt_entity->my_subscribe("topic/receive");
    
    char cmd[20];
    char *split, *channel, *target;
    while (true) {
        sem_wait(&mqtt_entity->msgSem);
        mqtt_entity->getCmd(cmd);
        
        // send to polulu
        split = strtok(cmd, ",");
        if (split != NULL) {
            channel = split;
            target = strtok(NULL, ",");
        }

        servo->maestroSetTarget(strtoul(channel, NULL, 0), strtoul(target, NULL, 0));         

    }

    delete mqtt_entity;
    delete servo;

    return 0;
}
