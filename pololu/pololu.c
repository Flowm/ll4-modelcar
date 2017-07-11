// Uses POSIX functions to send and receive data from a Maestro.
// NOTE: The Maestro's serial mode must be set to "USB Dual Port".
// NOTE: You must change the 'const char * device' line below.

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#ifdef _WIN32
#define O_NOCTTY 0
#else
#include <termios.h>
#endif

/**
 * Sets the maximum speed of the servo
 * @param fd			file descriptor describing the serial port
 * @param channel	channel where the servo is connected
 * @return returns -1 if error ocured else the current position
 */
int maestroGetPosition(int fd, unsigned char channel)
{
  unsigned char command[] = {0x90, channel};
  if(write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }

  unsigned char response[2];
  if(read(fd,response,2) != 2)
  {
    perror("error reading");
    return -1;
  }

  return response[0] + 256*response[1];
}

/**
 * Returns any servo is still moving
 * @param fd			file descriptor describing the serial port
 * @return returns -1 if error occured, 0 if not moving and 1 if moving
 */
int maestroGetMovingState(int fd)
{
  unsigned char command[] = {0x93};
  if(write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }

  unsigned char response[1];
  if(read(fd,response,1) != 1)
  {
    perror("error reading");
    return -1;
  }

  return response[0];
}

/**
 * Sets the target position of the servo
 * @param fd			file descriptor describing the serial port
 * @param channel	channel where the servo is connected
 * @param target	target position in quarter microseconds
 * @return returns -1 if error ocured else 0
 */
int maestroSetTarget(int fd, unsigned char channel, unsigned short target)
{
  unsigned char command[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
  if (write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }
  return 0;
}

/**
 * Sets the maximum speed of the servo
 * @param fd			file descriptor describing the serial port
 * @param channel	channel where the servo is connected
 * @param speed		maximum speed in quarter milliseconds. 0 means unlimited.
 * @return returns -1 if error ocured else 0
 */
int maestroSetSpeed(int fd, unsigned char channel, unsigned short speed) {
  unsigned char command[] = {0x87, channel, speed & 0x7F, speed >> 7 & 0x7F };
  if (write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }
  return 0;
}

/**
 * Sets the maximum accleration of the servo
 * @param fd			file descriptor describing the serial port
 * @param channel	channel where the servo is connected
 * @param accleration		maximum accleration as value between 1 and 255. 0 means unlimited
 * @return returns -1 if error ocured else 0
 */
int maestroSetAcceleration(int fd, unsigned char channel, unsigned short acc) {
  unsigned char command[] = {0x89, channel, acc & 0x7F, acc >> 7 & 0x7F };
  if (write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }
  return 0;
}

int main()
{
  // Open the Maestro's virtual COM port.
  //const char * device = "\\\\.\\USBSER000";  // Windows, "\\\\.\\COM6" also works
  const char * device = "/dev/ttyACM0";  // Linux
  //const char * device = "/dev/cu.usbmodem00034567";  // Mac OS X
  int fd = open(device, O_RDWR | O_NOCTTY);
  if (fd == -1)
  {
    perror(device);
    return 1;
  }

#ifdef _WIN32
  _setmode(fd, _O_BINARY);
#else
  struct termios options;
  tcgetattr(fd, &options);
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
  options.c_oflag &= ~(ONLCR | OCRNL);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tcsetattr(fd, TCSANOW, &options);
#endif

  int position = maestroGetPosition(fd, 9);
  printf("Current position is %d.\n", position);
  int target = 6000;
  //int target = (position < 6000) ? 7000 : 5000;
  printf("Setting target to %d (%d us).\n", target, target/4);
  maestroSetTarget(fd, 9, target);

  close(fd);
  return 0;
}
