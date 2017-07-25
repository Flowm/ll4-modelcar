#pragma once

#include <mosquittopp.h>
#include <semaphore.h>

class Mqtt_Entity : public mosqpp::mosquittopp
{
    private:
        const char *topic;

        /**
         * private buffer for incoming mqtt messages
         */
        char cmd[20];

        /**
         * semaphore needed for message handling
         */
        sem_t msg_sem;

        /**
         * private callbacks for mqtt
         */
        void on_connect(int rc);
        void on_disconnect(int rc);
        void on_publish(int mid);
        void on_message(const struct mosquitto_message *message);

    public:
        /**
         * Constructor
         *
         * @param id	unique id for mosquitto
         * @param topic	the topic where messages are sent to
         * @param host	ip address of mosquitto server
         */
        Mqtt_Entity(const char* id, const char *topic, const char* host);

        /**
         * Default Destructor
         */
        ~Mqtt_Entity();

        /**
         * send a message to the default topic
         */
        bool send_message(const char *message);

        /**
         * subscribe to a topic
         */
        bool my_subscribe(const char *topic);

        /**
         * get the message from mqtt
         *
         * @param buffer	buffer where the message is written to
         * @param size		size of the buffer
         */
        void get_cmd(char *buffer, size_t size);

        /**
         * wait for the next message (blocks)
         */
        void wait_msg();

};

