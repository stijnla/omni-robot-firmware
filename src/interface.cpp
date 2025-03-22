#include "interface.hpp"

void Command::readMessage(){
    if (Serial.available() > 0) {
        String received_message = Serial.readStringUntil('\n');
        if (received_message.length() == _message_length){
            decodeMessage(received_message);
        }else{
            Serial.println("!N");
        }
    }
}

int Command::getDesiredSpeedLeft(){
    return _decoded_speed_left;
}

int Command::getDesiredSpeedRight(){
    return _decoded_speed_right;
}

int Command::convertChar(char a){
    return a - '0';
}

void Command::decodeMessage(String message){
    if (message[0] != '!'){
        // check for start character and message integrity
        Serial.println("!N");
        return;
    }

    if (message[1] != '0' && message[1] != '1'){
        // check for start character and message integrity
        Serial.println("!N");
        return;
    }

    if (message[4] != '0' && message[4] != '1'){
        // check for start character and message integrity
        Serial.println("!N");
        return;
    }
    // Decode message, two speeds encoded in four characters, the first
    // character means positive or negative. The three characters after 
    // that are the speed in radians / sec.
    
    if (convertChar(message[1]) == 0){
    _decoded_speed_left = -1* (10*convertChar(message[2]) + convertChar(message[3]));
    } else if (convertChar(message[1]) == 1){
    _decoded_speed_left = 10*convertChar(message[2]) + convertChar(message[3]);
    }
    if (convertChar(message[4]) == 0){
    _decoded_speed_right = -1* (10*convertChar(message[5]) + convertChar(message[6]));
    } else if (convertChar(message[4]) == 1){
    _decoded_speed_right = 10*convertChar(message[5]) + 1*convertChar(message[6]);
    }
    Serial.println("!Y");
}

