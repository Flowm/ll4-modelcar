#pragma once

#include <servo_session/servo_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace Servo {
    struct Session_client : Genode::Rpc_client<Session> {

        Session_client(Genode::Capability<Session> cap)
            : Genode::Rpc_client<Session>(cap) {}

        /**
         * Sets the target position of the servo
         * @param channel   channel where the servo is connected
         * @param target    target position in quarter microseconds
         * @return  returns -1 if error ocured else 0
         */
        int setTarget(unsigned char channel, unsigned short target){
            return call<Rpc_setTarget>(channel, target);
        }

        /**
         * Sets the maximum speed of the servo
         * @param channel   channel where the servo is connected
         * @param speed     maximum speed in quarter milliseconds. 0 means unlimited.
         * @return  returns -1 if error ocured else 0
         */
        int setSpeed(unsigned char channel, unsigned short speed){
            return call<Rpc_setSpeed>(channel, speed);
        }

        /**
         * Sets the maximum accleration of the servo
         * @param channel       channel where the servo is connected
         * @param accleration   maximum accleration as value between 1 and 255. 0 means unlimited
         * @return  returns -1 if error ocured else 0
         */
        int setAcceleration(unsigned char channel, unsigned short acc){
            return call<Rpc_setAcceleration>(channel, acc);
        }

        /**
         * Gets the current position of the servo
         * @param channel   channel where the servo is connected
         * @return  returns -1 if error ocured else the current position
         */
        int getPosition(unsigned char channel){
            return call<Rpc_getPosition>(channel);
        }

        /**
         * Returns if any servo is still moving
         * @return  returns -1 if error occured, 0 if none is moving and 1 if any moving
         */
        int getMovingState(){
            return call<Rpc_getMovingState>();
        }

    };

}
