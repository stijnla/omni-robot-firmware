#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "Arduino.h"
class Command{
  public:
    Command(){};

    void readMessage();
    int getDesiredSpeedLeft();
    int getDesiredSpeedRight();

  private:
    int _decoded_speed_left = 0;
    int _decoded_speed_right = 0;
    String message;
    static const int _message_length = 7;

    int convertChar(char a);

    void decodeMessage(String message);
};
#endif