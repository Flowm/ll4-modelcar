#ifndef SENDER_H
#define SENDER_H

#include <mosquittopp.h>

class Mqtt_Sender : public mosqpp::mosquittopp
{
    private:
        const char *topic;
        const char *host;
        int port;
        int keepalive;

        void on_connect(int rc);
        void on_disconnect(int rc);
        void on_publish(int mid);
    public:
        Mqtt_Sender(const char *id, const char *topic, const char *host, int port);
        ~Mqtt_Sender();
        bool send_message(const char *message);
};

#endif /* SENDER_H */
