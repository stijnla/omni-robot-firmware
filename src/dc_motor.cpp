#include "Arduino.h"
#include "dc_motor.hpp"

DCMotor::DCMotor(int pwm_pin, int direction_1_pin, int direction_2_pin, double max_angular_velocity){
      this->pwm_pin = pwm_pin;
      this->direction_1_pin = direction_1_pin;
      this->direction_2_pin = direction_2_pin;
      this->max_angular_velocity = max_angular_velocity;

      pinMode(pwm_pin, OUTPUT);
      pinMode(direction_1_pin, OUTPUT);
      pinMode(direction_2_pin, OUTPUT); 
    }
    
void DCMotor::setDirection(bool direction){
    if (direction==1){
        digitalWrite(direction_1_pin, HIGH);
        digitalWrite(direction_2_pin, LOW);
    } else{
        digitalWrite(direction_1_pin, LOW);
        digitalWrite(direction_2_pin, HIGH);
    }
}

void DCMotor::setPWM(int pwm){
    analogWrite(pwm_pin, pwm);
}

double DCMotor::getMaxAngularVelocity(){
    return max_angular_velocity;
}
  
