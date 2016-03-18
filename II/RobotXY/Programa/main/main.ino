#include "xy.hpp"
#include "firmdata.hpp"

String inputString = "";


void setup() {
  // put your setup code here, to run once:
  initPinModes(9600);
  pinMode(13,OUTPUT);
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
//DEBUG
  stepsX = 1;
  stepsY = 1;
    inputString.reserve(200);
}

void loop() {
  // put your main code here, to run repeatedly:
 //checkAlarm();
  //controlJoystick();
  //readSerial();
  controlJoystick();
}

/* void loop()
   {
   checkAlarm();
   readSerial();
   controlJoystick();
 */
