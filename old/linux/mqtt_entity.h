#ifndef SENDER_H
#define SENDER_H

#include <mosquittopp.h>
#include <semaphore.h>

class Mqtt_Entity : public mosqpp::mosquittopp
{
    private:
        const char *host;
        const char *topic;
        int port;
        int keepalive;
        sem_t msg_sem;

        char cmd[20];
		char msg_buffer[100];

        void on_connect(int rc);
        void on_disconnect(int rc);
        void on_publish(int mid);
        void on_message(const struct mosquitto_message *message);
    public:
        Mqtt_Entity(const char* id, const char *topic, const char* host);
        ~Mqtt_Entity();
        bool send_message(const char *message);
        bool my_subscribe(const char *topic);
        void get_cmd(char *buffer, size_t size);
        void wait();

};

#endif /* SENDER_H */
