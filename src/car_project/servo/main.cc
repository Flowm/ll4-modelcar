#include <base/printf.h>
#include <base/sleep.h>
#include <base/rpc_server.h>

#include <servo_session/servo_session.h>

#include <root/component.h>
#include <cap_session/connection.h>
#include <terminal_session/connection.h>
#include <timer_session/connection.h>

namespace Servo {

    struct Servo_component : Genode::Rpc_object<Session>
    {
        private:
            Terminal::Connection* _terminal = new Terminal::Connection(_terminal);
            const static int SERVO_UPPER_BOUND = 7500;
            const static int SERVO_LOWER_BOUND = 4500;

        public:
            int transform_steer(double value) {
                if (value < -1 || value > 1) {
                    PERR("Invalid steering angle - range is -1 to 1");
                    return -1;
                }

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

            int maestroSetTarget(unsigned char channel, unsigned short target) {
                if (channel > 11) {
                    PERR("Channel does not exist");
                    return -1;
                }

                if (target < 4000 || target > 8000) {
                    PERR("Invalid target position - range is 4000 to 8000");
                    return -1;
                }

                unsigned char command[] = {0x84, channel, (unsigned char)(target & 0x7F), (unsigned char)(target >> 7 & 0x7F)};
                if (_terminal->write(command, sizeof(command)) < sizeof(command)) {
                    PERR("error writing");
                    return -1;
                }
                return 0;
            }

            int maestroSetSpeed(unsigned char channel, unsigned short speed) {
                if (channel > 11) {
                    PERR("Channel does not exist");
                    return -1;
                }

                unsigned char command[] = {0x87, channel, (unsigned char)(speed & 0x7F), (unsigned char)(speed >> 7 & 0x7F)};
                if (_terminal->write(command, sizeof(command)) < sizeof(command)) {
                    PERR("error writing");
                    return -1;
                }
                return 0;
            }

            int maestroSetAcceleration(unsigned char channel, unsigned short acc) {
                if (channel > 11) {
                    PERR("Channel does not exist");
                    return -1;
                }

                if (acc > 255) {
                    PERR("Invalid target acceleration - range is 0 to 255");
                    return -1;
                }

                unsigned char command[] = {0x89, channel, (unsigned char)(acc & 0x7F), (unsigned char)(acc >> 7 & 0x7F)};
                if (_terminal->write(command, sizeof(command)) < sizeof(command)) {
                    PERR("error writing");
                    return -1;
                }
                return 0;
            }

            int maestroGetPosition(unsigned char channel) {
                if (channel > 11) {
                    PERR("Channel does not exist");
                    return -1;
                }

                unsigned char command[] = {0x90, channel};
                if(_terminal->write(command, sizeof(command)) < sizeof(command)) {
                    PERR("error writing");
                    return -1;
                }

                unsigned char response[2];
                if(_terminal->read(response,2) != 2) {
                    PERR("error reading");
                    return -1;
                }

                return response[0] + 256*response[1];
            }

            int maestroGetMovingState() {
                unsigned char command[] = {0x93};
                if(_terminal->write(command, sizeof(command)) < sizeof(command)) {
                    PERR("error writing");
                    return -1;
                }

                unsigned char response[1];
                if(_terminal->read(response, 1) != 1) {
                    PERR("error reading");
                    return -1;
                }

                return response[0];
            }

    };



    class Servo_root : public Genode::Root_component<Servo_component>
    {
        protected:
            Servo_component *_create_session(const char *args) {
                PDBG("Creating Servo session.");
                return new (md_alloc()) Servo_component();
            }

        public:
            Servo_root(Genode::Rpc_entrypoint *ep, Genode::Allocator *allocator)
                : Genode::Root_component<Servo_component>(ep, allocator) {
                    PDBG("Creating root component.");
            }
    };
}

int main(void) {

    using namespace Genode;
    using namespace Servo;

    PDBG("in main before cap");

    /*
     * Get a session for the parent's capability service, so that we
     * are able to create capabilities.
     */
    Cap_connection cap;

    PDBG("in main after cap");

    enum { STACK_SIZE = 4096 };
    static Rpc_entrypoint ep(&cap, STACK_SIZE, "Servo_ep");

    PDBG("in main after rpc entrypoint");

    static Servo_root Servo_root(&ep, env()->heap());

    PDBG("in main after rpc Servo_root");

    /*
     * Announce services
     */
    env()->parent()->announce(ep.manage(&Servo_root));

    /*
     * We are done with this and only act upon client requests now.
     */
    sleep_forever();
    return 0;

}
