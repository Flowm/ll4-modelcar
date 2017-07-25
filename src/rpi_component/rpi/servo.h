// TODO: Remove
#ifndef SERVO_H
#define SERVO_H

class Servo
{
    private:
        Terminal::Connection *_terminal;

    public:
        /**
         * Constructor of the Servo class
         * @param device    the unix device where the servocontroller is conected to
         */
        Servo(Terminal::Connection *terminal);
        ~Servo();

        /**
         * Sets the target position of the servo
         * @param channel	channel where the servo is connected
         * @param target	target position in quarter microseconds
         * @return  returns -1 if error ocured else 0
         */
        int maestroSetTarget(unsigned char channel, unsigned short target);

        /**
         * Sets the maximum speed of the servo
         * @param channel	channel where the servo is connected
         * @param speed		maximum speed in quarter milliseconds. 0 means unlimited.
         * @return  returns -1 if error ocured else 0
         */
        int maestroSetSpeed(unsigned char channel, unsigned short speed);

        /**
         * Sets the maximum accleration of the servo
         * @param channel	    channel where the servo is connected
         * @param accleration	maximum accleration as value between 1 and 255. 0 means unlimited
         * @return  returns -1 if error ocured else 0
         */
        int maestroSetAcceleration(unsigned char channel, unsigned short acc);

        /**
         * Gets the current position of the servo
         * @param channel	channel where the servo is connected
         * @return  returns -1 if error ocured else the current position
         */
        int maestroGetPosition(unsigned char channel);

        /**
         * Returns if any servo is still moving
         * @return  returns -1 if error occured, 0 if none is moving and 1 if any moving
         */
        int maestroGetMovingState();
};

#endif /* SERVO_H */
