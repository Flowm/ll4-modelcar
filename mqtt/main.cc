#include <iostream>
#include "sender.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << "Specify a message" << std::endl;
        return -1;
    }

    Mqtt_Sender *mqtt_sender = new Mqtt_Sender("999", "topic/test", "localhost", 1883);

    mqtt_sender->send_message(argv[1]);
    mqtt_sender->send_message("second message");

    delete mqtt_sender;

    return 0;
}
