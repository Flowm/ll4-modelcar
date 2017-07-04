#include "sender.h"
#include <string.h>
#include <iostream>

Mqtt_Sender::Mqtt_Sender(const char *id, const char *topic, const char *host, int port)
    : mosqpp::mosquittopp(id), id(id), topic(topic), host(host), port(port) {

        mosqpp::lib_init();
        this->keepalive = 60;
        connect_async(host, port, keepalive);
        loop_start();
}

Mqtt_Sender::~Mqtt_Sender() {
    while (want_write()) {
        std::cout << 42 << std::endl;
    }
    disconnect();
    loop_stop();
    mosqpp::lib_cleanup();
}

bool Mqtt_Sender::send_message(const char *message) {
    int ret = publish(NULL, this->topic, strlen(message), message, 1, false);
    return (ret == MOSQ_ERR_SUCCESS); 
}

void Mqtt_Sender::on_connect(int rc) {
    if (rc == 0) {
        std::cout << ">> Mqtt_Sender - connected with server" << std::endl;
    } else {
        std::cout << ">> Mqtt_Sender - could not connect with server" << std::endl;
    }
};

void Mqtt_Sender::on_disconnect(int rc) {
    std::cout << ">> Mqtt_Sender - disconnected from server" << std::endl;
};

void Mqtt_Sender::on_publish(int mid) {
    std::cout << ">> Mqtt_Sender - message " << mid << " published" << std::endl;
};
