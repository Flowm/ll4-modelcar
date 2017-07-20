#pragma once

#include <controller_session/controller_session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace  Controller {
	struct Session_client : Genode::Rpc_client<Session> {

		Session_client(Genode::Capability<Session> cap)
			: Genode::Rpc_client<Session>(cap) {}

		int transform_steer(double value) {
			PDBG("Transform steer.");
			return call<Rpc_transform_steer>(value);
		}

        int transform_brake(double value) {
			PDBG("Transform brake.");
			return call<Rpc_transform_brake>(value);
        } 

	};

}
