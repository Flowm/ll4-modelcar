#include "servo.h"
#include "utils.h"

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

#include <iostream>

Servo::Servo(const char *device) {
	fd = open(device, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		char buffer[50];
		sprintf(buffer, "Could not open device %s", device);
		print_error(buffer);
		return;
	}

	// set options for termios
	struct termios options;
	tcgetattr(fd, &options);
	options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
	options.c_oflag &= ~(ONLCR | OCRNL);
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	options.c_ispeed = 115200;
	options.c_ospeed = 115200;
	tcsetattr(fd, TCSANOW, &options);

	std::cout << "Servo initialized" << std::endl;
}

Servo::~Servo() {
	close(fd);
}

int Servo::maestroSetTarget(unsigned char channel, unsigned short target) {
	if (channel > 11) {
		print_error("Channel does not exist");
		return -1;
	}

	if (target < 4000 || target > 8000) {
		print_error("Invalid target position - range is 4000 to 8000");
		return -1;
	}

	unsigned char command[] = {0x84, channel, (unsigned char)(target & 0x7F), (unsigned char)(target >> 7 & 0x7F)};
	if (write(fd, command, sizeof(command)) == -1) {
		print_error("error writing");
		return -1;
	}
	return 0;
}

int Servo::maestroSetSpeed(unsigned char channel, unsigned short speed) {
	if (channel > 11) {
		print_error("Channel does not exist");
		return -1;
	}

	unsigned char command[] = {0x87, channel, (unsigned char)(speed & 0x7F), (unsigned char)(speed >> 7 & 0x7F)};
	if (write(fd, command, sizeof(command)) == -1) {
		print_error("error writing");
		return -1;
	}
	return 0;
}

int Servo::maestroSetAcceleration(unsigned char channel, unsigned short acc) {
	if (channel > 11) {
		print_error("Channel does not exist");
		return -1;
	}

	if (acc > 255) {
		print_error("Invalid target acceleration - range is 0 to 255");
		return -1;
	}

	unsigned char command[] = {0x89, channel, (unsigned char)(acc & 0x7F), (unsigned char)(acc >> 7 & 0x7F)};
	if (write(fd, command, sizeof(command)) == -1) {
		print_error("error writing");
		return -1;
	}
	return 0;
}

int Servo::maestroGetPosition(unsigned char channel) {
	if (channel > 11) {
		print_error("Channel does not exist");
		return -1;
	}

	unsigned char command[] = {0x90, channel};
	if(write(fd, command, sizeof(command)) == -1) {
		print_error("error writing");
		return -1;
	}

	unsigned char response[2];
	if(read(fd,response,2) != 2) {
		print_error("error reading");
		return -1;
	}

	return response[0] + 256*response[1];
}

int Servo::maestroGetMovingState() {
	unsigned char command[] = {0x93};
	if(write(fd, command, sizeof(command)) == -1) {
		print_error("error writing");
		return -1;
	}

	unsigned char response[1];
	if(read(fd, response, 1) != 1) {
		print_error("error reading");
		return -1;
	}

	return response[0];

}
