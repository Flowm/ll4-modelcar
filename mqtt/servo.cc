#include "servo.h"
#include "utilities.h"

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

Servo::Servo(const char *device) {
    int fd = open(device, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        printError("Could not open device %s", device);
        return; 
    } 

    // set options for termios
    struct termios options;
    tcgetattr(fd, &options);
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
    options.c_oflag &= ~(ONLCR | OCRNL); 
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tcsetattr(fd, TCSANOW, &options);
}

Servo::~Servo() {
    close(fd);
}

int Servo::maestroSetTarget(unsigned char channel, unsigned short target) {
    if (channel > 11) {
        printError("Channel does not exist");
        return -1;
    }

    if (target < 4000 || target > 8000) {
        printError("Invalid target position - range is 4000 to 8000");
        return -1;
    }

    unsigned char command[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
    if (write(fd, command, sizeof(command)) == -1)
    { 
        printError("error writing");
        return -1;
    }
    return 0;
}

int Servo::maestroSetSpeed(unsigned char channel, unsigned short speed) {
    unsigned char command[] = {0x87, channel, speed & 0x7F, speed >> 7 & 0x7F };
    if (write(fd, command, sizeof(command)) == -1)
    {
        printError("error writing");
        return -1;
    }
    return 0;
}

int Servo::maestroSetAcceleration(unsigned char channel, unsigned short acc) {
    unsigned char command[] = {0x89, channel, acc & 0x7F, acc >> 7 & 0x7F };
    if (write(fd, command, sizeof(command)) == -1)
    {
        ("error writing");
        return -1;
    }
    return 0;
}

int Servo::maestroGetPosition(unsigned char channel) {
    unsigned char command[] = {0x90, channel};
    if(write(fd, command, sizeof(command)) == -1)
    { 
        printError("error writing");
        return -1;
    }

    unsigned char response[2];
    if(read(fd,response,2) != 2)
    { 
        printError("error reading");
        return -1;
    }

    return response[0] + 256*response[1];
}

int Servo::maestroGetMovingState() {
    unsigned char command[] = {0x93};
    if(write(fd, command, sizeof(command)) == -1)
    {
        printError("error writing");
        return -1;
    }

    unsigned char response[1];
    if(read(fd,response,1) != 1)
    {
        printError("error reading");
        return -1;
    }

    return response[0];

}

