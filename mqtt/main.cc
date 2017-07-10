#include <iostream>
#include "mqtt_entity.h"
#include <unistd.h>

int main(int argc, char *argv[]) {

    Mqtt_Entity *mqtt_entity = new Mqtt_Entity("topic/send");

    mqtt_entity->send_message("test message");
    mqtt_entity->my_subscribe("topic/receive");

    while (true) {
        sleep(1000);
    }

    delete mqtt_entity;

    return 0;
}
