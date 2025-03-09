#include "hall_sensor.hpp"

double pi = 3.14159265359;

HallSensor::HallSensor(AS5600& as5600): as5600(as5600){
}

void HallSensor::getMeasurements(double& angle, double& angular_velocity){
    // gets angle and angular velocity. Angle is in radians, angular_velocity is in radians per sec
    unsigned long current_time = micros();
    angle = AS5600_RAW_TO_RADIANS * as5600.rawAngle();
    if (initialized){
        double dAngle = angle - previous_angle;
        if (dAngle > pi/2){
            // Serial.print(dAngle);
            // Serial.print("\t");
            dAngle = dAngle - 2*pi;
            // Serial.println(dAngle);
        }
        if (dAngle < -pi/2){
            // Serial.print(dAngle);
            // Serial.print("\t");
            dAngle = dAngle + 2*pi;
            // Serial.println(dAngle);
        }
        angular_velocity = 1000000 * dAngle / double(current_time - previous_time);
    }
    
    previous_angle = angle;
    previous_time = current_time;
    initialized = true;
}
void HallSensor::getStatus(
        int& automatedGainControl, 
        bool& detectedMagnet, 
        bool& magnetTooStrong, 
        bool& magnetTooWeak){
    automatedGainControl = as5600.readAGC();
    detectedMagnet = as5600.detectMagnet();
    magnetTooStrong = as5600.magnetTooStrong();
    magnetTooWeak = as5600.magnetTooWeak();
}

