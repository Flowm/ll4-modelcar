/*
 * \brief  Main program of the Hello server
 * \author Björn Döbel
 * \date   2008-03-20
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

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

#include "mqtt_entity.h"
#include "controller.h"
#include "utils.h"

#define STEER_CHANNEL "6"
#define BRAKE_LEFT_FRONT_CHANNEL "1"
#define BRAKE_RIGHT_FRONT_CHANNEL "2"
#define BRAKE_REAR_CHANNEL "0"

enum IDs {
    STEER = 0, // [-1;1]
    BRAKE = 1, // [0;1]
    ACCEL = 2  // [0,1]
};

using namespace Genode;

int main(void)
{
  Timer::Connection timer;

  enum { BUF_SIZE = Nic::Packet_allocator::DEFAULT_PACKET_SIZE * 128 };

  Genode::Xml_node network = Genode::config()->xml_node().sub_node("network");

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
    PDBG("done");
  } else {
    PDBG("manual network...");
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
    PDBG("done");
  }

  /* get config */
  Genode::Xml_node mosquitto = Genode::config()->xml_node().sub_node("mosquitto");

  char ip_addr[16] = {0};
  char port[5] = {0};

  mosquitto.attribute("ip-address").value(ip_addr, sizeof(ip_addr));
  mosquitto.attribute("port").value(port, sizeof(port));


    char recv_cmd[50];
    char servo_cmd[10];

    char *split, *id, *target;
    double value;
    int servoVal;

    timer.msleep(10000);
    PDBG("Connecting to MQTT server");

    Mqtt_Entity *mqtt_entity = new Mqtt_Entity("panda", "car-servo", ip_addr);
    Controller *controller = new Controller();

    mqtt_entity->my_subscribe("car-control");

    while (true) {
        sem_wait(&mqtt_entity->msgSem);
        mqtt_entity->getCmd(recv_cmd, sizeof(recv_cmd));

        split = strtok(recv_cmd, ",");
        if (!split) {
            continue;
        }
        id = split;

        target = strtok(NULL, ",");
        if (!target) {
            continue;
        }
        value = atof(target);

        switch (strtoul(id, NULL, 0)) {
            case STEER :
                servoVal = controller->transform_steer(value);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", STEER_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                break;
            case BRAKE :
                servoVal = controller->transform_brake(value);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", BRAKE_LEFT_FRONT_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", BRAKE_RIGHT_FRONT_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                snprintf(servo_cmd, sizeof(servo_cmd), "%s,%d", BRAKE_REAR_CHANNEL, servoVal);
                mqtt_entity->send_message(servo_cmd);
                break;
            case ACCEL :
                //servoVal = controller->transform_accel(value);
                break;
            default :
                servoVal = -1;
                break;
        }
    }

    delete mqtt_entity;
    delete controller;

    sleep_forever();

    return 0;
}
