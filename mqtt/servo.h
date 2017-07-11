#ifndef SERVO_H
#define SERVO_H

class Servo
{
    private:
        int fd;
    public:
        Servo(const char *device);
        ~Servo();
        int maestroSetTarget(unsigned char channel, unsigned short target);
        int maestroSetSpeed(unsigned char channel, unsigned short speed);
        int maestroSetAcceleration(unsigned char channel, unsigned short acc);
        int maestroGetPosition(unsigned char channel);


        /**
         * Returns any servo is still moving
         * @return returns -1 if error occured, 0 if not moving and 1 if moving
         */
        int maestroGetMovingState();
};

#endif
