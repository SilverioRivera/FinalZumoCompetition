#include <ZumoShield.h>     // Zumo Library
#include <SoftwareSerial.h> // Serial Port on Pins

ZumoMotors motors;          // Two of the Zumo Motors
SoftwareSerial bt(2,6);     // RX = PIN2 and TX = PIN6

const int SPEED = 400;      // Front/Back Speed to MAX setting
const int TURN_SPEED = 280; // Spin Speed

void setup(){
  bt.begin(9600);           // Bluetooth speed matched with AVR
  pinMode(13,OUTPUT);       // Yellow LED used for Debugging
}

void loop(){
  if(bt.available()){          // Detecting if the character arrives
    char c = bt.read();        // Reading the incoming signal
    digitalWrite(13,c != 'S'); // LED turns ON for any movement command

    if  (c == 'F') motors.setSpeeds(SPEED, SPEED);                // Forward
    else if (c == 'B') motors.setSpeeds(-SPEED, -SPEED);          // Back
    else if (c == 'L') motors.setSpeeds(-TURN_SPEED, TURN_SPEED); // Left Spin
    else if (c == 'R') motors.setSpeeds(TURN_SPEED, -TURN_SPEED); // Right Spin
    else               motors.setSpeeds(0, 0);                    // STOP

  }
}
