#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
    private:
        const static int SERVO_UPPER_BOUND = 7500;
        const static int SERVO_LOWER_BOUND = 4500;

    public:

        /**
         * Transforms steering values to concrete servo values
         */
        int transform_steer(double value);
        
        /**
         * Transforms braking values to concrete servo values
         */
        int transform_brake(double value);
        
};

#endif
