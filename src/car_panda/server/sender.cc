#include "sender.h"

#include <base/env.h>
#include <base/printf.h>
#include <util/xml_node.h>
#include <os/config.h>

#include <cstdio>
#include <cstring>

Mqtt_Sender::Mqtt_Sender(const char *id, const char *topic, const char *host, int port)
    : mosqpp::mosquittopp(id), topic(topic), host(host), port(port) {

        mosqpp::lib_init();
        this->keepalive = 60;
        connect_async(host, port, keepalive);
        loop_start();
}

Mqtt_Sender::~Mqtt_Sender() {
    while (want_write()) {
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
	PDBG(">> Mqtt_Sender - connected with server");
    } else {
	PDBG(">> Mqtt_Sender - could not connect with server");
    }
};

void Mqtt_Sender::on_disconnect(int rc) {
    PDBG(">> Mqtt_Sender - disconnected from server"); 
};

void Mqtt_Sender::on_publish(int mid) {
    PDBG(">> Mqtt_Sender - message %d published", mid);
};
