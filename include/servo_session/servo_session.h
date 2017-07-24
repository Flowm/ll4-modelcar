#pragma once

#include <session/session.h>
#include <base/rpc.h>

namespace Servo {

    struct Session : public Genode::Session {

        static const char *service_name() {
            return "Servo";
        }

        /**
         * Sets the target position of the servo
         * @param channel	channel where the servo is connected
         * @param target	target position in quarter microseconds
         * @return  returns -1 if error ocured else 0
         */
        virtual int setTarget(unsigned char channel, unsigned short target) = 0;

        /**
         * Sets the maximum speed of the servo
         * @param channel	channel where the servo is connected
         * @param speed		maximum speed in quarter milliseconds. 0 means unlimited.
         * @return  returns -1 if error ocured else 0
         */
        virtual int setSpeed(unsigned char channel, unsigned short speed) = 0;

        /**
         * Sets the maximum accleration of the servo
         * @param channel	    channel where the servo is connected
         * @param accleration	maximum accleration as value between 1 and 255. 0 means unlimited
         * @return  returns -1 if error ocured else 0
         */
        virtual int setAcceleration(unsigned char channel, unsigned short acc) = 0;

        /**
         * Gets the current position of the servo
         * @param channel	channel where the servo is connected
         * @return  returns -1 if error ocured else the current position
         */
        virtual int getPosition(unsigned char channel) = 0;

        /**
         * Returns if any servo is still moving
         * @return  returns -1 if error occured, 0 if none is moving and 1 if any moving
         */
        virtual int getMovingState() = 0;

        /* return-type, fn-name, parameters */

        GENODE_RPC(Rpc_setTarget, int, setTarget, unsigned char, unsigned short);
        GENODE_RPC(Rpc_setSpeed, int, setSpeed, unsigned char, unsigned short);
        GENODE_RPC(Rpc_setAcceleration, int, setAcceleration, unsigned char, unsigned short);
        GENODE_RPC(Rpc_getPosition, int, getPosition, unsigned char);
        GENODE_RPC(Rpc_getMovingState, int, getMovingState);

        GENODE_RPC_INTERFACE(Rpc_setTarget, Rpc_setSpeed, Rpc_setAcceleration, Rpc_getPosition, Rpc_getMovingState);

    };

}
