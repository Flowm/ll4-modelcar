#include "mqtt_entity.h"
#include <string.h>
#include <termios.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

Mqtt_Entity::Mqtt_Entity(const char *topic) 
    : mosqpp::mosquittopp(NULL), topic(topic) {

	 const char * device = "/dev/ttyACM0";  // Linux
	 this->fd = open(device, O_RDWR | O_NOCTTY);
	 if (fd == -1)
	 {
		std::cout << "error opening file" << std::endl;
		 return;
	 }

	 struct termios options;
	 tcgetattr(fd, &options);
	 options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
	 options.c_oflag &= ~(ONLCR | OCRNL);
	 options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	 tcsetattr(fd, TCSANOW, &options);

        mosqpp::lib_init();
        this->keepalive = 60;
        this->host = "localhost";
        this->port = 1883;
        connect_async(host, port, keepalive);
        loop_start();
}

Mqtt_Entity::~Mqtt_Entity() {
    close(fd);
    while (want_write()) {
        std::cout << 42 << std::endl;
    }
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

void Mqtt_Entity::on_connect(int rc) {
    if (rc == 0) {
        std::cout << ">> Mqtt_Entity - connected with server" << std::endl;
    } else {
        std::cout << ">> Mqtt_Entity - could not connect with server" << std::endl;
    }
};

void Mqtt_Entity::on_disconnect(int rc) {
    std::cout << ">> Mqtt_Entity - disconnected from server" << std::endl;
};

void Mqtt_Entity::on_publish(int mid) {
    std::cout << ">> Mqtt_Entity - message " << mid << " published" << std::endl;
};

void Mqtt_Entity::on_message(const struct mosquitto_message *message) {
    char *msg = (char*) message->payload;
    std::cout << ">> Mqtt_Entity - message received " << msg << std::endl;
    maestroSetTarget(9, std::strtoul(msg, NULL, 0));
}

/**
 * Sets the target position of the servo
 * @param fd                    file descriptor describing the serial port
 * @param channel       channel where the servo is connected
 * @param target        target position in quarter microseconds
 * @return returns -1 if error ocured else 0
 */
int Mqtt_Entity::maestroSetTarget(unsigned char channel, unsigned short target)
{ 
	unsigned char command[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
	if (write(this->fd, command, sizeof(command)) == -1)
	{ 
		std::cout << "error writing" << std::endl;
		return -1;
	}
	return 0;
}


