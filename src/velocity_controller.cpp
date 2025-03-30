#include "velocity_controller.hpp"

VelocityControl::VelocityControl(DCMotor &dc_motor, HallSensor &hal_sensor, double k_p, double k_i, double k_d) : dc_motor(dc_motor), hal_sensor(hal_sensor)
{
    dc_motor.setDirection(0);
    P_value = k_p; // 50.0;
    I_value = k_i; // 3200.0;
    D_value = k_d; // 0.001;
}

VelocityData VelocityControl::setSpeed(double radians_per_sec)
{
    if (desired_speed != radians_per_sec)
    {
        desired_speed = radians_per_sec;
        i_error = 0; // reset integral term if new speed is desired
    }
    current_time = micros();
    double dt = 0.0000001 * (current_time - previous_time);

    // Get error: difference desired and current angular velocity
    double angle;
    hal_sensor.getMeasurements(angle, current_speed);
    if (radians_per_sec < 0)
    {
        dc_motor.setDirection(1);
        error = current_speed - radians_per_sec;
    }
    else
    {
        dc_motor.setDirection(0);
        error = radians_per_sec - current_speed;
    }
    if (dt > 0)
    {
        d_error = (error - previous_error) / dt;
    }
    else
    {
        d_error = 0;
    }

    i_error = i_error + error * dt;
    if (i_error > 1.5)
    {
        // Cap integral term for safety
        i_error = 1.5;
    }
    control_signal = P_value * error + D_value * d_error + I_value * i_error;
    control_signal = control_signal * (255 / dc_motor.getMaxAngularVelocity());

    if (control_signal > 255)
    {
        control_signal = 255;
    }
    if (control_signal < 0)
    {
        control_signal = 0;
    }

    if (radians_per_sec == 0)
    {
        control_signal = 0;
    }
    dc_motor.setPWM((int)control_signal);

    VelocityData data;
    data.control_signal = control_signal;
    data.current_speed = current_speed;
    data.current_angle = angle;
    previous_time = current_time;
    previous_error = error;
    return data;
}
