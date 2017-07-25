#include <base/env.h>
#include <base/printf.h>
#include <base/sleep.h>
#include <cap_session/connection.h>
#include <os/config.h>
#include <root/component.h>
#include <util/xml_node.h>
#include <timer_session/connection.h>
#include <base/rpc_server.h>

extern "C" {
#include <lwip/sockets.h>
}
#include <lwip/genode.h>
#include <nic/packet_allocator.h>

#include <servo_session/client.h>
#include <servo_session/connection.h>
#include "../../mqtt/mqtt_entity.h"


int main(void)
{
    using namespace Genode;

    /*
     * Network configuration
     */
    Genode::Xml_node network = Genode::config()->xml_node().sub_node("network");
    enum { BUF_SIZE = Nic::Packet_allocator::DEFAULT_PACKET_SIZE * 128 };
    if (network.attribute_value<bool>("dhcp", true)) {
        PDBG("DHCP network...");
        if (lwip_nic_init(0,
                    0,
                    0,
                    BUF_SIZE,
                    BUF_SIZE)) {
            PERR("lwip init failed!");
            return 1;
        }
    } else {
        PDBG("Manual network...");
        char ip_addr[16] = {0};
        char subnet[16] = {0};
        char gateway[16] = {0};

        network.attribute("ip-address").value(ip_addr, sizeof(ip_addr));
        network.attribute("subnet-mask").value(subnet, sizeof(subnet));
        network.attribute("default-gateway").value(gateway, sizeof(gateway));

        if (lwip_nic_init(inet_addr(ip_addr),
                    inet_addr(subnet),
                    inet_addr(gateway),
                    BUF_SIZE,
                    BUF_SIZE)) {
            PERR("lwip init failed!");
            return 1;
        }
    }
    PDBG("Network configuration done, waiting for IP");
    Timer::Connection timer;
    timer.msleep(10000);

    /*
     * MQTT setup
     */
    Genode::Xml_node mosquitto = Genode::config()->xml_node().sub_node("mosquitto");

    char ip_addr[16] = {0};
    char port[5] = {0};
    mosquitto.attribute("ip-address").value(ip_addr, sizeof(ip_addr));
    mosquitto.attribute("port").value(port, sizeof(port));

    PDBG("Connecting to MQTT server");
    Mqtt_Entity *mqtt_entity = new Mqtt_Entity("rpi", "car-status", ip_addr);
    mqtt_entity->my_subscribe("car-servo");

    /*
     * Servo connection
     */
    PDBG("Servo init");
    static Servo::Connection servo;

    /*
     * Handle mqtt messages
     */
    char recv_cmd[50];
    char *split, *channel, *target;
    while (true) {
        mqtt_entity->wait_msg();
        mqtt_entity->get_cmd(recv_cmd, sizeof(recv_cmd));

        split = strtok(recv_cmd, ",");
        if (!split) {
            continue;
        }
        channel = split;

        target = strtok(NULL, ",");
        if (!target) {
            continue;
        }

        servo.setTarget(strtoul(channel, NULL, 0), strtoul(target, NULL, 0));
    }

    sleep_forever();

    delete mqtt_entity;
    return 0;
}
