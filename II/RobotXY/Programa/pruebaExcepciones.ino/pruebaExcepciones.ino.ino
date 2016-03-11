void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
attachInterrupt(0,intEncoderX,RISING);
  attachInterrupt(1,intEncoderY,RISING);
  attachInterrupt(4,intActivateEndstop,FALLING);
  attachInterrupt(5,interruptEmergency,FALLING);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void intEncoderX()
{
  Serial.println("Se ha pulsado encoderX");
  
}

void intEncoderY()
{
  Serial.println("Se ha pulsado Y");
  
}

void intActivateEndstop()
{
  Serial.println("Endstop");
}

void interruptEmergency()
{
  Serial.println("Emergencia");
}

