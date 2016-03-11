#include "xy.hpp"
#include "firmdata.hpp"

void setup() {
  // put your setup code here, to run once:
  initPinModes(9600);
  pinMode(13,OUTPUT);

  stepsX = 1;
  stepsY = 1;
  //Serial.println("SETUP");
  activeMotors();
  //moveUp();
    
  /*  activeBrake();
  motorHome(0);
  motorHome(1);
  calibrate(0);
  calibrate(1);
  */
  
   /* motorHome(1);
  cal
  
  selectVelocityX(1);
  moveLeft();
  //moveRight();
  delay(6000);
  stopMotorX();*/
//  motorHome(0);
 // motorHome(1);
  //calibrate(0);
  //calibrate(1);
  //digitalWrite(13,LOW);
   //     stepsX = 1;
  //    stepsY = 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  //checkAlarm();
  //controlJoystick();
  readSerial();
  controlJoystick();
}
