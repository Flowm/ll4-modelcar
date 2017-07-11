#ifndef SENDER_H
#define SENDER_H
#include <semaphore.h>

#include <mosquittopp.h>

class Mqtt_Entity : public mosqpp::mosquittopp
{
    private:
        const char *host;
        const char *topic;
        int port;
        int keepalive;

        char cmd[20];

        void on_connect(int rc);
        void on_disconnect(int rc);
        void on_publish(int mid);
        void on_message(const struct mosquitto_message *message);
    public:
        Mqtt_Entity(const char *topic);
        ~Mqtt_Entity();
        bool send_message(const char *message);
        bool my_subscribe(const char *topic);
        void getCmd(char *buffer);
        void setCmd(char *cmd);
        
        sem_t msgSem; 
};

#endif /* SENDER_H */
