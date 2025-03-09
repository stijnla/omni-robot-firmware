#ifndef VELOCITY_CONTROLLER_HPP
#define VELOCITY_CONTROLLER_HPP
#include "dc_motor.hpp"
#include "hall_sensor.hpp"

struct VelocityData{
  int control_signal;
  double current_speed;
};

class VelocityControl{
  public:
    VelocityControl(DCMotor& dc_motor, HallSensor& hal_sensor, double k_p, double k_i, double k_d);
    VelocityData setSpeed(double degrees_per_sec);

  private:
    DCMotor dc_motor;
    HallSensor& hal_sensor;
    double desired_speed = 0;
    double P_value;
    double I_value;
    double D_value;
    double error = 0;
    double d_error = 0;
    double i_error = 0;
    double previous_error = 0;
    double current_speed = 0.0;
    double previous_speed = 0.0;
    unsigned long current_time = 0;
    unsigned long previous_time = 0;
    double control_signal = 0;
};
#endif