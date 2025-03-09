#ifndef HALL_SENSOR_HPP
#define HALL_SENSOR_HPP

#include "AS5600.h"
class HallSensor{
  public:
    HallSensor(AS5600& as5600);
    
    void getMeasurements(double& angle, double& angular_velocity);
    void getStatus(
      int& automatedGainControl, 
      bool& detectedMagnet, 
      bool& magnetTooStrong, 
      bool& magnetTooWeak);
  private:
    double previous_angle = 0;
    unsigned long previous_time = 0;
    bool initialized = false;
    bool first_measurement = false;
    bool increasing = false;
    AS5600& as5600;
};

#endif