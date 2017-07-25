#include "mqtt_entity.h"

#include <string.h>
#include <stdio.h>
#include <base/printf.h>

Mqtt_Entity::Mqtt_Entity(const char* id, const char *topic, const char* host)
    : mosqpp::mosquittopp(id), topic(topic) {

        sem_init(&msgSem, 0, 0);

        // init mosquitto, set params and start loop
        mosqpp::lib_init();
        connect(host, 1883, 60);
        loop_start();
    }

Mqtt_Entity::~Mqtt_Entity() {
    while (want_write()) {}

    // clean up mosquitto
    disconnect();
    loop_stop();
    mosqpp::lib_cleanup();
}

bool Mqtt_Entity::send_message(const char *message) {
    int ret = publish(NULL, this->topic, strlen(message), message, 1, false);
    return (ret == MOSQ_ERR_SUCCESS);
}

bool Mqtt_Entity::my_subscribe(const char *topic) {
    int ret = subscribe(NULL, topic);
    return (ret == MOSQ_ERR_SUCCESS);
}

void Mqtt_Entity::get_cmd(char *buffer, size_t size) {
    strncpy(buffer, cmd, size);
}

void Mqtt_Entity::on_connect(int rc) {
    if (rc == 0) {
        PDBG("Mqtt_Entity - connected with server");
    } else {
        PERR("Mqtt_Entity - could not connect with server");
    }
};

void Mqtt_Entity::on_disconnect(int rc) {
    PDBG("Mqtt_Entity - disconnected from server");
};

void Mqtt_Entity::on_publish(int mid) {
    PDBG("Mqtt_Entity - message %d published", mid);
};

void Mqtt_Entity::on_message(const struct mosquitto_message *message) {
    char *msg = (char*) message->payload;
    PDBG("Mqtt_Entity - message received %s", msg);
    strncpy(cmd, msg, sizeof(cmd));
    sem_post(&msgSem);
}

