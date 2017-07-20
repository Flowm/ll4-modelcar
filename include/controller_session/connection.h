#pragma once

#include <controller_session/client.h>
#include <base/connection.h>

namespace Controller {
	struct Connection : Genode::Connection<Session>, Session_client {
		Connection()
			: Genode::Connection<Controller::Session>(session("foo, ram_quota=4K")),
			Session_client(cap()) { }
	};
}
