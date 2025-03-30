#include "TCA9548.h"
#include "FastIMU.h"
#include "hall_sensor.hpp"
#include "dc_motor.hpp"
#include "interface.hpp"
#include "velocity_controller.hpp"

#define IMU_ADDRESS 0x68
MPU6500 IMU;

TCA9548 MP(0x70);
AS5600 as5600_left;
AS5600 as5600_right;

uint32_t FAST_MODE = 400000; // 400 kHz

int motor_right_pwm = 9;
int motor_right_dir1 = 8;
int motor_right_dir2 = 7;
double motor_right_max_angular_velocity = 600; // mean max speed = 625

int motor_left_dir1 = 5;
int motor_left_dir2 = 4;
int motor_left_pwm = 3;
double motor_left_max_angular_velocity = 650; // mean max speed = 670

HallSensor hal_sensor_left(as5600_left);
HallSensor hal_sensor_right(as5600_right);

DCMotor dc_motor_left(motor_left_pwm, motor_left_dir1, motor_left_dir2, motor_left_max_angular_velocity);
DCMotor dc_motor_right(motor_right_pwm, motor_right_dir1, motor_right_dir2, motor_right_max_angular_velocity);

VelocityControl velocity_control_left(dc_motor_left, hal_sensor_left, 50.0, 3200.0, 0.001);
VelocityControl velocity_control_right(dc_motor_right, hal_sensor_right, 50.0, 1600.0, 0.01);

Command current_command;

// IMU calibration data
double acceleration_calibration[3] = {0.00, 0.00, 0.00};
double gyro_calibration[3] = {0.00, 0.00, 0.00};
calData calib = {
    1,
}; // Calibration data

AccelData accelData; // Sensor data
GyroData gyroData;

// Function to switch to the desired bus on the multiplexer
void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70); // TCA9548A address is 0x70
  Wire.write(1 << bus);         // send byte to select bus
  Wire.endTransmission();
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(1); // on average it takes 0.785 milliseconds to read the full message
  Wire.begin();
  Wire.setClock(1000000);
  // Initialize multiplexer, do not continue if it fails
  if (MP.begin() == false)
  {
    while (true)
    {
      Serial.println("Could not connect to TCA9548 multiplexer.");
    }
  }
  // Initialize IMU, do not continue if it fails
  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0)
  {
    while (true)
    {
      Serial.print("Error initializing IMU: ");
      Serial.println(err);
    }
  }
  if (true)
  { // false if calibrating not necessary
    // Serial.println("Calibrating... Keep IMU level.");
    // delay(5000);
    // IMU.calibrateAccelGyro(&calib);
    // Serial.println("Calibration done!");
    calib.accelBias[0] = acceleration_calibration[0];
    calib.accelBias[1] = acceleration_calibration[1];
    calib.accelBias[2] = acceleration_calibration[2];
    calib.gyroBias[0] = gyro_calibration[0];
    calib.gyroBias[1] = gyro_calibration[1];
    calib.gyroBias[2] = gyro_calibration[2];
    Serial.println("Accel biases X/Y/Z: ");
    Serial.print(calib.accelBias[0]);
    Serial.print(", ");
    Serial.print(calib.accelBias[1]);
    Serial.print(", ");
    Serial.println(calib.accelBias[2]);
    Serial.println("Gyro biases X/Y/Z: ");
    Serial.print(calib.gyroBias[0]);
    Serial.print(", ");
    Serial.print(calib.gyroBias[1]);
    Serial.print(", ");
    Serial.println(calib.gyroBias[2]);
    IMU.init(calib, IMU_ADDRESS);
  }

  // ranges: 250, 500, 1000, 2000 degrees / s for gyro, 2, 4, 8, 16 g for accel
  err = IMU.setGyroRange(250); // USE THESE TO SET THE gyro RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  err = IMU.setAccelRange(2);  // THESE TWO SET THE accel RANGE TO ±500 DPS AND THE ACCELEROMETER RANGE TO ±2g
  if (err != 0)
  {
    while (true)
    {
      Serial.print("Error Setting range: ");
      Serial.println(err);
    }
  }
}

void loop()
{
  current_command.readMessage(); // read incoming message
  int desired_speed_left = current_command.getDesiredSpeedLeft();
  int desired_speed_right = current_command.getDesiredSpeedRight();

  // Collect data from hal sensors, control motors according to incoming message
  TCA9548A(3);
  VelocityData s1 = velocity_control_left.setSpeed(desired_speed_left);
  TCA9548A(2);
  VelocityData s2 = velocity_control_right.setSpeed(desired_speed_right);

  // Read IMU data
  IMU.update();
  IMU.getAccel(&accelData);
  IMU.getGyro(&gyroData);

  // Sent data back to master
  String data = "!" + String(accelData.accelX) + "," + String(accelData.accelY) + "," + String(accelData.accelZ) + "," +
                String(gyroData.gyroX) + "," + String(gyroData.gyroY) + "," + String(gyroData.gyroZ) + "," +
                String(IMU.getTemp()) + "," + String(s1.current_angle) + "," + String(s1.current_speed) + "," +
                String(s1.control_signal) + "," + String(s2.current_angle) + "," + String(s2.current_speed) + "," +
                String(s2.control_signal);
  // Calculate checksum
  uint8_t checksum = 0;
  for (size_t i = 0; i < data.length(); i++)
  {
    checksum ^= data[i];
  }
  data += "," + String(checksum);
  Serial.println(data);
}
