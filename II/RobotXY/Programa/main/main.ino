#include "xy.hpp"
#include "firmdata.hpp"

String inputString = "";


#define NONE 2
#define MASTER 1
#define SLAVE 0
#define ROL NONE 

 bool isAlive = true;




void setup() {
  // put your setup code here, to run once:
  initPinModes(9600);
  if (ROL == SLAVE)
  {
    initInterruptions();
    initSlave();
  
  }else if (ROL == MASTER){
    initMaster();
  }
  else if(ROL == NONE)
    initInterruptions();
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
  
  if(ROL == MASTER && isAlive){
    isAlive = checkAliveMaster();
    if(!isAlive)
      initInterruptions();
  }
  if(!isAlive && ROL == MASTER)
  {
    //digitalWrite(13,HIGH);
    checkAlarm();
    controlJoystick();
  }else if(ROL == SLAVE || ROL == NONE)
  {
    //digitalWrite(13,HIGH);
    checkAlarm();
    if(!endstopsActivate)
    controlJoystick();
  }
  endstopsActivate = false;
}

/* void loop()
   {
   checkAlarm();
   readSerial();
   controlJoystick();
 */


 // TODO TEST
 /*
  * Probar Joystick en los finales de carrera
  * Probar aplicacion Android
  * todos los movimientosla 
  * Boton de emergencia y rearme
  * Velocidades
  * checkAlarms
  * Endstops
  */


  //TODO
  /* 
   *  Controlar endstops en el joystick ** Solved
   *  POner emergencia en todos los while de movimientos **Solved
   *  POner el setPosition de la aplicacion android  ** TODO Comprobar el estado de los textview de la posicion actual.
   *  Quitar los SerialPrint **Solved
   *  Seleccionar la velocidad desde la app
   */
