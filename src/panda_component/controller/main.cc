#include <base/printf.h>
#include <base/sleep.h>
#include <base/rpc_server.h>

#include <controller_session/controller_session.h>

#include <root/component.h>
#include <cap_session/connection.h>
#include <terminal_session/connection.h>
#include <timer_session/connection.h>

namespace Controller {

    struct Controller_component : Genode::Rpc_object<Session>
    {
        private:
            const static int SERVO_UPPER_BOUND = 7500;
            const static int SERVO_LOWER_BOUND = 4500;

        public:
            int transform_steer(double value) {
                if (value < -1 || value > 1) {
                    PERR("Invalid steering angle - range is -1 to 1");
                    return -1;
                }

                // Invert value as SpeedDreams thinks -1 is right
                value = -value;

                value = (value + 1)/2;
                return (SERVO_UPPER_BOUND - SERVO_LOWER_BOUND) * value + SERVO_LOWER_BOUND;
            }

            int transform_brake(double value) {
                if (value < 0 || value > 1) {
                    PERR("Invalid target brake position - range is 0 to 1");
                    return -1;
                }

                return (SERVO_UPPER_BOUND - SERVO_LOWER_BOUND) * value + SERVO_LOWER_BOUND;
            }
    };



    class Controller_root : public Genode::Root_component<Controller_component>
    {
        protected:
            Controller_component *_create_session(const char *args) {
                PDBG("Creating controller session.");
                return new (md_alloc()) Controller_component();
            }

        public:
            Controller_root(Genode::Rpc_entrypoint *ep, Genode::Allocator *allocator)
                : Genode::Root_component<Controller_component>(ep, allocator) {
                    PDBG("Creating root component.");
            }
    };
}

int main(void) {

    using namespace Genode;
    using namespace Controller;
    
    /*
     * Get a session for the parent's capability service, so that we
     * are able to create capabilities.
     */
    Cap_connection cap;
    
    enum { STACK_SIZE = 4096 };
    static Rpc_entrypoint ep(&cap, STACK_SIZE, "controller_ep");
    
    static Controller_root controller_root(&ep, env()->heap());

    /*
     * Announce services
     */
    env()->parent()->announce(ep.manage(&controller_root));

    /*
     * We are done with this and only act upon client requests now.
     */
    sleep_forever();
    return 0;

}
