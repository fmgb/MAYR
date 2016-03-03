#include "xy.hpp"
#include "firmdata.hpp"

void setup() {
  // put your setup code here, to run once:
  initPinModes(9600);
  activeBrake();
  motorHome(0);
  motorHome(1);
  calibrate(0);
  calibrate(1);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
