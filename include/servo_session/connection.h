#include <servo_session/client.h>
#include <base/connection.h>

namespace Servo {
    struct Connection : Genode::Connection<Session>, Session_client {
        Connection()
            : Genode::Connection<Servo::Session>(session("foo, ram_quota=4K")),
            Session_client(cap()) { }
    };
}
