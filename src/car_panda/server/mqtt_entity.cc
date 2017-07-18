#include "mqtt_entity.h"
#include "utils.h" 
#include <string.h>
#include <stdio.h> 

Mqtt_Entity::Mqtt_Entity(const char* id, const char *topic, const char* host)
    : mosqpp::mosquittopp(id), topic(topic) {

        this->keepalive = 60;
        this->host = host;
        this->port = 1883;

        sem_init(&msgSem, 0, 0);

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
        print_message("Mqtt_Entity - connected with server");
    } else {
        print_error("Mqtt_Entity - could not connect with server");
    }
};

void Mqtt_Entity::on_disconnect(int rc) {
    print_message("Mqtt_Entity - disconnected from server");
};

void Mqtt_Entity::on_publish(int mid) {
	snprintf(msg_buffer, sizeof(msg_buffer), "Mqtt_Entity - message %d published", mid);
    print_message(msg_buffer);
};

void Mqtt_Entity::on_message(const struct mosquitto_message *message) {
    char *msg = (char*) message->payload;
	snprintf(msg_buffer, sizeof(msg_buffer), "Mqtt_Entity - message received %s", msg);
    print_message(msg_buffer);
    strncpy(cmd, msg, sizeof(cmd));
    sem_post(&msgSem);
}

