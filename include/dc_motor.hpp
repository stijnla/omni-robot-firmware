#ifndef DC_MOTOR_HPP
#define DC_MOTOR_HPP

#include "Arduino.h"

class DCMotor{
  public:
    DCMotor(int pwm_pin, int direction_1_pin, int direction_2_pin, double max_angular_velocity);
    
    void setDirection(bool direction);

    void setPWM(int pwm);

    double getMaxAngularVelocity();
    
  private:
    int pwm_pin;
    int direction_1_pin;
    int direction_2_pin;
    double max_angular_velocity;
};
#endif