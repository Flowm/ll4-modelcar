#include "mqtt_entity.h"
#include <string.h>
#include <iostream>

Mqtt_Entity::Mqtt_Entity(const char* id, const char *topic)
    : mosqpp::mosquittopp(id), topic(topic) {

        this->keepalive = 60;
        this->host = "localhost";
        this->port = 1883;

        //sem_init(&msgSem, 0, 0);

        mosqpp::lib_init();
        connect(host, port, keepalive);
        loop_start();
}

Mqtt_Entity::~Mqtt_Entity() {
    while (want_write()) {}
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

void Mqtt_Entity::getCmd(char *buffer, size_t size) {
    strncpy(buffer, cmd, size);
}

void Mqtt_Entity::on_connect(int rc) {
    if (rc == 0) {
        PDBG("Mqtt_Entity - connected with server");
    } else {
        PDBG("Mqtt_Entity - could not connect with server");
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
    //sem_post(&msgSem);
}

