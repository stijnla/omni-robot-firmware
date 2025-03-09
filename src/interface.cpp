#include "interface.hpp"

void Command::readMessage(){
    if (Serial.available() > 0) {
    String received_message = Serial.readStringUntil('e');
    if (received_message.length() == _message_length){
        decodeMessage(received_message);
    }
    }
}

int Command::getDesiredSpeedLeft(){
    return _decoded_speed_left;
}

int Command::getDesiredSpeedRight(){
    return _decoded_speed_right;
}

int _decoded_speed_left = 0;
int _decoded_speed_right = 0;
String message;
static const int _message_length = 9;

int Command::convertChar(char a){
    return a - '0';
}

void Command::decodeMessage(String message){
    if (message[0] != 's'){
    // check for start character and message integrity
    return;
    }

    if (message[1] != '0' && message[1] != '1'){
    // check for start character and message integrity
    return;
    }

    if (message[5] != '0' && message[5] != '1'){
    // check for start character and message integrity
    return;
    }
    // Decode message, two speeds encoded in four characters, the first
    // character means positive or negative. The three characters after 
    // that are the speed in radians / sec.
    
    if (convertChar(message[1]) == 0){
    _decoded_speed_left = -1* (100*convertChar(message[2]) + 10*convertChar(message[3]) + convertChar(message[4]));
    } else if (convertChar(message[1]) == 1){
    _decoded_speed_left = 100*convertChar(message[2]) + 10*convertChar(message[3]) + convertChar(message[4]);
    }
    if (convertChar(message[5]) == 0){
    _decoded_speed_right = -1* (100*convertChar(message[6]) + 10*convertChar(message[7]) + convertChar(message[8]));
    } else if (convertChar(message[5]) == 1){
    _decoded_speed_right = 100*convertChar(message[6]) + 10*convertChar(message[7]) + convertChar(message[8]);
    }
}

