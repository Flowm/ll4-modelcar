#include "servo.h"
#include "utils.h"

#include <base/printf.h>
#include <base/sleep.h>
#include <base/rpc_server.h>

#include <root/component.h>
#include <terminal_session/connection.h>

Servo::Servo(Terminal::Connection *terminal)
            : _terminal(terminal){
	PDBG("Servo initialized");
}

Servo::~Servo() {
}

int Servo::maestroSetTarget(unsigned char channel, unsigned short target) {
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

int Servo::maestroSetSpeed(unsigned char channel, unsigned short speed) {
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

int Servo::maestroSetAcceleration(unsigned char channel, unsigned short acc) {
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

int Servo::maestroGetPosition(unsigned char channel) {
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

int Servo::maestroGetMovingState() {
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
