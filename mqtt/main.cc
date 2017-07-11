#include "mqtt_entity.h"
#include "servo.h"
#include <unistd.h>
#include <iostream>
#include <string.h>

int main(int argc, char *argv[]) {

    Mqtt_Entity *mqtt_entity = new Mqtt_Entity("topic/send");
    Servo *servo = new Servo("/dev/ttyUSB0");

    mqtt_entity->my_subscribe("topic/receive");
    
    char *cmd;
    char *split, *channel, *target;
    while (true) {

        cmd = mqtt_entity->getCmd();
        if (cmd != NULL) {
            // send to polulu
            split = strtok(cmd, ",");
            if (split != NULL) {
                channel = split;
                target = strtok(NULL, ",");
            }
        
            servo->maestroSetTarget(strtoul(channel, NULL, 0), strtoul(target, NULL, 0));         
            mqtt_entity->setCmd(NULL);
        } 

        sleep(1000);
    }

    delete mqtt_entity;
    delete servo;

    return 0;
}
