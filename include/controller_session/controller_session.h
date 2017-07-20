#pragma once

#include <session/session.h>
#include <base/rpc.h>

namespace Controller {

    struct Session : public Genode::Session {

        static const char *service_name() {
            return "Controller";
        }
        
        /**
         * Transforms steering values to concrete servo values
         */
        virtual int transform_steer(double value) = 0;

        /**
         * Transforms braking values to concrete servo values
         */
        virtual int transform_brake(double value) = 0;

        /* return-type, fn-name, parameters */

        GENODE_RPC(Rpc_transform_steer, int, transform_steer, double);
        GENODE_RPC(Rpc_transform_brake, int, transform_brake, double);

        GENODE_RPC_INTERFACE(Rpc_transform_steer, Rpc_transform_brake);

    };

}
