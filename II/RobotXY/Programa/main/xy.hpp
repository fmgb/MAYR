#ifndef _XY
#define _XY

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include <WSWire.h>
/**********************************************/
//
//            CONSTANTES DEL SISTEMA
//
/**********************************************/

//Milimetros entre los extremos de los ejes.
#define mmX  2185
#define mmY  1783

//-------------------------
//        ENTRADAS Y SALIDAS ARDUINO
//-------------------------

#define O_MARCHA    42   //Enciende los Variadores
#define I_CMARCHA   32   //Motores activados

//Joystick
#define I_JKARR     30   //Arriba Joystick
#define I_JKABA     31   //Abajo Joystick
#define I_JKIZQ     25   //Izquierda Joystick
#define I_JKDER     26   //Derecha Joystick

//-------------------------
//        Eje X
//-------------------------

//Direcciones

#define O_STFX 46   //Avance dcha
#define O_STRX 47   //Avance izda

//Velocidades del Variador

#define O_RLX 37   //Baja
#define O_RMX 36   //Media
#define O_RHX 35   //Alta

//Finales de carrera

#define I_FCX1  23  //Izquierdo mirando de frente
#define I_FCX2  24  //Derecho mirando de frente

//Alarma

#define I_ALARMAX   22

//Comprobacion funcionamiento

#define I_RUNX  33

//Piloto estado eje

#define O_PAX   44

//-------------------------
//        Eje Y
//-------------------------

//Direcciones

#define O_STFY 48   //Arriba
#define O_STRY 49   //Abajo

//Velocidades

#define O_RLY 41   //Baja
#define O_RMY 40   //Media
#define O_RHY 39   //Alta

//Finales de carrera

#define I_FCY1  28  //Arriba
#define I_FCY2  29  //Abajo

//Alarma

#define I_ALARMAY   27

//Comprobacion funcionamiento

#define I_RUNY  34

//Freno
#define O_FRENO 43 //Normalmente cerrado

//Piloto estado eje

#define O_PAY 45

/**********************************************/
//
//      FUNCIONES DE INTERRUPCION
//
/**********************************************/

//Interrupciones de los pines
#define INT_EMERG 18
#define INT_ENX 2
#define INT_ENY 3
#define INT_INTFINC 19

/**********************************************/
//
//            ESTADOS DEL SISTEMA
//
/**********************************************/
#define DIRBACK 0
#define DIRFORWARD 1
#define FRENODES 1 //NORMALMENTE CERRADO
#define FRENOACT 0

int contadorPasosX = 0;
int contadorPasosY = 0;

float stepsX = 0;
float stepsY = 0;

bool dirMotorX = false; // False -> LEFT; True -> RIGHT
bool dirMotorY = false; // False -> DOWN; True -> UP

bool useJoystick = true;
bool freno = false;
int tWaitLedAlarm = 100;
int tWaitJoystick = 100;
int tWait = 3000;
int tWaitFC = 2000;
int maxVelocityPoint = 750;
int midVelocityPoint = 250;
int tiempoRequestMaster = 5000;
#define TWAITI2C 0
#define I2CDIR 8
float THRESHOLDX_1 = 0.1;
float THRESHOLDX_2 = 0.2;
float THRESHOLDY_1 = 0.1;
float THRESHOLDY_2 = 0.2;
bool emergencia = false;

void intEncoderX();
void intEncoderY();
void interruptEmergency();
void intActivateEndstop();
void motorHome(unsigned short motorSelect); // 0 -> X, 1 -> Y
void activeMotors();
void desactiveMotors();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
void stopMotorX();
void stopMotorY();
void activeBrake();
void deactiveBrake();
void controlJostick();
void calibrate(unsigned short motor); // Steps per mm.
void moveXmm(int mm); // + -> Right, - -> Left
void moveYmm(int mm); // + -> Up, - -> Down
unsigned short getStateEndStops();
void adaptVelocityX(int remainingSteps);
void adaptVelocityY(int remainingSteps);
void selectVelocityX(unsigned short velocity);
void selectVelocityY(unsigned short velocity);
void writePosition();



void setPosition(int value,int axis)
{
  if(axis == 0)
    contadorPasosX = value;
  else
    contadorPasosY = value;
}

void writePosition()
{
    Serial.print(contadorPasosX/(int)stepsX);
    Serial.print(",");
    Serial.print(contadorPasosY/(int)stepsY);
  Serial.print("\n");
}

/*void adaptVelocityX(int remainingSteps)
{
  if(remainingSteps > maxVelocityPoint)
    selectVelocityX(3);
  else if(remainingSteps > midVelocityPoint)
    selectVelocityX(2);
  else
    selectVelocityX(1);
}

void adaptVelocityY(int remainingSteps)
{
  if(remainingSteps > maxVelocityPoint)
    selectVelocityY(3);
  else if(remainingSteps > midVelocityPoint)
    selectVelocityY(2);
  else
    selectVelocityY(1);
}
*/
void moveLeft()
{
  digitalWrite(O_STFX,LOW);
  digitalWrite(O_STRX,HIGH);
  digitalWrite(O_PAX,HIGH);
  dirMotorX = false;
}

void moveRight()
{
  digitalWrite(O_STRX,LOW);
  digitalWrite(O_STFX,HIGH);
  digitalWrite(O_PAX,HIGH);
  dirMotorX = true;
}

//TODO CAMBIAR UP POR DOWN
void moveDown()
{
  deactiveBrake();
  digitalWrite(O_STRY,LOW);
  digitalWrite(O_STFY,HIGH);
  digitalWrite(O_PAY,HIGH);
  dirMotorY = true;
}

void moveUp()
{
  deactiveBrake();
  digitalWrite(O_STFY,LOW);
  digitalWrite(O_STRY,HIGH);
  digitalWrite(O_PAY,HIGH);
  dirMotorY = false;
}

void adaptVelocityYUp(int stepsInicial,int contadorTarget)
{
  int diffSteps = contadorTarget- stepsInicial;

  int threshold1 = diffSteps * THRESHOLDY_1;
  int threshold2 = diffSteps * THRESHOLDY_2;

  if(contadorPasosY < stepsInicial + threshold1 || contadorPasosY > contadorTarget - threshold1)
    selectVelocityY(1);
  else if(contadorPasosY < stepsInicial + threshold2 || contadorPasosY > contadorTarget - threshold2)
    selectVelocityY(2);
  else
    selectVelocityY(3);
}

void adaptVelocityYDown(int stepsInicial, int contadorTarget)
{
  int diffSteps = stepsInicial - contadorTarget;
  
  int threshold1 = diffSteps * THRESHOLDY_1;
  int threshold2 = diffSteps * THRESHOLDY_2;

  if(contadorPasosY > stepsInicial - threshold1 || contadorPasosY < contadorTarget + threshold2)
     selectVelocityY(1);
  else if(contadorPasosY > stepsInicial - threshold2 || contadorPasosY < contadorTarget + threshold2)
     selectVelocityY(2);
  else
    selectVelocityY(3);
  
}

void adaptVelocityXRight(int stepsInicial, int contadorTarget)
{
  int diffSteps = contadorTarget- stepsInicial;

  int threshold1 = diffSteps * THRESHOLDX_1;
  int threshold2 = diffSteps * THRESHOLDX_2;

  if(contadorPasosX < stepsInicial + threshold1 || contadorPasosX > contadorTarget - threshold1)
    selectVelocityX(1);
  else if(contadorPasosX < stepsInicial + threshold2 || contadorPasosX > contadorTarget - threshold2)
    selectVelocityX(2);
  else
    selectVelocityX(3);
}

void adaptVelocityXLeft(int stepsInicial,int contadorTarget)
{
  int diffSteps = stepsInicial - contadorTarget;
  
  int threshold1 = diffSteps * THRESHOLDX_1;
  int threshold2 = diffSteps * THRESHOLDX_1;

  if(contadorPasosX > stepsInicial - threshold1 || contadorPasosX < contadorTarget + threshold2)
     selectVelocityX(1);
  else if(contadorPasosX > stepsInicial - threshold2 || contadorPasosX < contadorTarget + threshold2)
     selectVelocityX(2);
  else
    selectVelocityX(3);

}

void moveXmm(int mm)
{
  stopMotorY();
  stopMotorX();
  int stepsInicial = contadorPasosX;
  int contadorTarget = contadorPasosX + stepsX * mm;

  if(mm > 0)
    {
      
      moveRight();
      
      while(contadorTarget > contadorPasosX && digitalRead(I_FCX2) == LOW && !emergencia) 
      {
        adaptVelocityXRight(stepsInicial,contadorTarget); 
      }
      stopMotorX();
    }
  else if (mm < 0)
    {
      moveLeft();
      while(contadorTarget < contadorPasosX && digitalRead(I_FCX1) == LOW && !emergencia) 
      {
        adaptVelocityXLeft(stepsInicial,contadorTarget);
      };
      stopMotorX();
    }
  else
    Serial.println("Move 0 mm");
}

void moveYmm(int mm)
{
  stopMotorY();
  stopMotorX();
  int stepsInicial = contadorPasosY;
  int contadorTarget = contadorPasosY + stepsY * mm;
  if(mm > 0)
    {
      moveUp();
      
      while(contadorTarget > contadorPasosY && digitalRead(I_FCY1) == LOW && !emergencia) 
      { 
	adaptVelocityYUp(stepsInicial,contadorTarget);
      }
      stopMotorY();
    }
  else if (mm < 0)
    {
      moveDown();
      while(contadorTarget < contadorPasosY && digitalRead(I_FCY2) == LOW && !emergencia) 
      {
	adaptVelocityYDown(stepsInicial, contadorTarget);
//  	adaptVelocityY(contadorPasosY - contadorTarget);
      }
      //Serial.println("1001");
      stopMotorY();
    }
  else
    Serial.println("Move 0 mm");
}


void moveXSteps(int steps)
{
  int stepsInicial = contadorPasosX;
  //Serial.println("Move Steps");
  if (steps > 0)
  {
  int stepsTarget = contadorPasosX + steps;
     moveRight();
     //Serial.println("Derecha");
     //Serial.println(stepsTarget);
     while(stepsTarget > contadorPasosX && digitalRead(I_FCX2) == LOW && !emergencia) 
    {
      //Serial.println(contadorPasosX);
	adaptVelocityXRight(stepsInicial,stepsTarget);
// 	adaptVelocityX(stepsTarget-contadorPasosX);
    };
     stopMotorX();
  }
  else 
  {
    //Serial.println(contadorPasosX);
    int stepsTarget = contadorPasosX + steps;
    //Serial.println("Izquierda");
     moveLeft();
      //Serial.println(stepsTarget);
     while(stepsTarget < contadorPasosX && digitalRead(I_FCX1) == LOW && !emergencia) 
     {
      //Serial.println(contadorPasosX);
	adaptVelocityXLeft(stepsInicial,stepsTarget);
     };
     //Serial.println("Salgo");
     stopMotorX();
  }

}

void moveYSteps(int steps) {

  int stepsInicial = contadorPasosY;
  if (steps > 0)
  {
  int stepsTarget = contadorPasosY + steps;

     moveUp();
     while(stepsTarget > contadorPasosY && digitalRead(I_FCY1) == LOW && !emergencia) 
     {
	adaptVelocityYUp(stepsInicial,stepsTarget);
//	adaptVelocityY(stepsTarget - contadorPasosY);
     };
     stopMotorY();
  }
  else 
  {
   // Serial.println(contadorPasosY);
    int stepsTarget = contadorPasosY + steps;
  //Serial.println("Abajo");
     moveDown();
    // Serial.println(stepsTarget);
     while(stepsTarget < contadorPasosY && digitalRead(I_FCY2) == LOW && !emergencia) 
     {
	adaptVelocityYDown(stepsInicial, stepsTarget);
  //	adaptVelocityY(contadorPasosY - stepsTarget);
     };
     //Serial.println("Salgo");
     stopMotorY();
  }
}

void checkAlarm()
{
  int alarmaX = digitalRead(I_ALARMAX);
  int alarmaY = digitalRead(I_ALARMAY);
  if(alarmaX || alarmaY)
  {
    stopMotorY();
    stopMotorX();
    activeBrake();
    bool led = HIGH;
    while(alarmaX || alarmaY) 
    {
      alarmaX = digitalRead(I_ALARMAX);
    alarmaY = digitalRead(I_ALARMAY);
      digitalWrite(O_PAX,led);
      digitalWrite(O_PAY,led);
      delay(tWaitLedAlarm);
      led = !led;
     };
  }
}



void stopMotorX()
{
  digitalWrite(O_STFX,LOW);
  digitalWrite(O_STRX,LOW);
  digitalWrite(O_PAX,LOW);
}

void stopMotorY()
{
  digitalWrite(O_STFY,LOW);
  digitalWrite(O_STRY,LOW);
  digitalWrite(O_PAY,LOW);
  activeBrake();
}

void activeBrake()
{

  digitalWrite(O_FRENO, FRENOACT);
  freno = true;
  delay(tWait);  
}

void deactiveBrake()
{
  digitalWrite(O_FRENO, FRENODES);
  freno = false;
  //delay(tWait);
}

void initPinModes(unsigned short baudrate)
{
  Serial.begin(baudrate);
  
  // INPUT
  pinMode(I_CMARCHA, INPUT);
  pinMode(I_JKARR, INPUT);  
  pinMode(I_JKABA,INPUT);
  pinMode(I_JKIZQ,INPUT);
  pinMode(I_JKDER,INPUT);
  pinMode(I_FCX1,INPUT);
  pinMode(I_FCX2,INPUT);
  pinMode(I_ALARMAX,INPUT);
  pinMode(I_RUNX,INPUT);
  pinMode(I_FCY1,INPUT);
  pinMode(I_FCY2,INPUT);
  pinMode(I_ALARMAY,INPUT);
  pinMode(I_RUNY,INPUT);

  
  //OUTPUT
  pinMode(O_MARCHA, OUTPUT);
  pinMode(O_STFX,OUTPUT);
  pinMode(O_STRX,OUTPUT);
  pinMode(O_RLX,OUTPUT);
  pinMode(O_RMX,OUTPUT);
  pinMode(O_RHX,OUTPUT);
  pinMode(O_PAX,OUTPUT);
  pinMode(O_STFY,OUTPUT);
  pinMode(O_STRY,OUTPUT);
  pinMode(O_RLY,OUTPUT);
  pinMode(O_RMY,OUTPUT);
  pinMode(O_RHY,OUTPUT);
  pinMode(O_FRENO,OUTPUT);
  pinMode(O_PAY,OUTPUT);



}

void initInterruptions()
{
    //INTERRUPTS
  //TODO CHANGE THE INTERRUPTION MODE.
  attachInterrupt(0,intEncoderX,RISING);
  attachInterrupt(1,intEncoderY,RISING);
  attachInterrupt(4,intActivateEndstop,FALLING);
  attachInterrupt(5,interruptEmergency,FALLING);
}

void activeMotors()
{
  digitalWrite(O_MARCHA,HIGH);
}

void deactiveMotors()
{
  activeBrake();
  
  digitalWrite(O_MARCHA,LOW);
}

//vel = 1 -> LOW, vel = 2 -> MEDIUM, vel = 3 -> HIGH
void selectVelocityX(unsigned short velocity)
{
  digitalWrite(O_RLX,LOW);
  digitalWrite(O_RMX,LOW);
  digitalWrite(O_RHX,LOW);
      
  if(velocity == 1)
    digitalWrite(O_RLX,HIGH);
  else if(velocity == 2)
    digitalWrite(O_RMX,HIGH);
  else if(velocity == 3)
    digitalWrite(O_RHX,HIGH);
  else
    Serial.println("ERROR: Select Velocity > 3");
}

//vel = 1 -> LOW, vel = 2 -> MEDIUM, vel = 3 -> HIGH
void selectVelocityY(unsigned short velocity)
{
  digitalWrite(O_RLY,LOW);
  digitalWrite(O_RMY,LOW);
  digitalWrite(O_RHY,LOW);
      
  if(velocity == 1)
    digitalWrite(O_RLY,HIGH);
  else if(velocity == 2)
    digitalWrite(O_RMY,HIGH);
  else if(velocity == 3)
    digitalWrite(O_RHY,HIGH);
  else
    Serial.println("ERROR: Select Velocity > 3");
}


// motorSelect = 0 -> X ; motorSelect = 1 -> Y
void motorHome(unsigned short motorSelect)
{
 // Serial.print(digitalRead(I_CMARCHA));
 // if(digitalRead(I_CMARCHA) == HIGH)
   // {
      if(motorSelect == 0)
        {//Motor X
          //Serial.println("Homing X");
          selectVelocityX(1);
          stopMotorY();
          moveLeft();
          //delay(500);
          //delay(tWaitFC);
          while(!digitalRead(I_FCX1) && !emergencia){ };

          
          contadorPasosX = 0;
          moveRight();
	  
          //while(digitalRead(I_FCX1)) {};
          delay(tWaitFC);
          stopMotorX();
        }
      else if(motorSelect == 1)
        {// Motor Y
          //Serial.println("Homing Y");
          selectVelocityY(1);
          stopMotorX();
          deactiveBrake();
          delay(50);
          moveDown();
         //delay(tWaitFC);
          while(!digitalRead(I_FCY2) && !emergencia) { };
          
          contadorPasosY = 0;
          moveUp();
//          while(digitalRead(I_FCY2)){ };
  	  delay(tWaitFC);        
          stopMotorY();
        }
      else
        {//UNKNOWN Motor
          //Serial.println("ERROR: motorSelect > 1");
          interruptEmergency();
        }
    
  //else
    //Serial.println("ERROR: The motorSelect is not active");
}

// Steps expresed in mm.
void calibrate(unsigned short motor)
{
  //Start [0,0]
  if(motor == 0)
    {
      selectVelocityX(2);
      moveRight();
      while(!digitalRead(I_FCX2) && !emergencia) { };
      
      int stepsXAux = contadorPasosX;
     /* Serial.print("\nSteps motor X per");
      Serial.print(mmX);
      Serial.print(": ");
      Serial.println(stepsXAux);
      *///Serial.println(contador
      moveLeft();
      while(digitalRead(I_FCX2) && !emergencia){};
      stopMotorX();
      
      stepsX = (stepsXAux)/((float)mmX);
      //Serial.printlnln(stepsX);
    }
  else if(motor == 1)
    {
      selectVelocityX(2);
      moveUp();
      while(!digitalRead(I_FCY1) && !emergencia) { };
      
      int stepsYAux = contadorPasosY;
      //Serial.println("\nSteps per");
      //Serial.println(mmY);
      //Serial.println(": ");
      //Serial.printlnln(stepsYAux);
      moveDown();
      while(digitalRead(I_FCY1) && !emergencia){};
      
      stopMotorY();
      
      stepsY = (stepsYAux)/((float) mmY);
      //Serial.printlnln(stepsY);
    }
  else
    Serial.println("ERROR: Calibrate motor");
}

void intEncoderX()
{
  if(dirMotorX == DIRBACK)
    contadorPasosX--;
  else
    contadorPasosX++;
}

void intEncoderY()
{
  if(dirMotorY != DIRBACK)
    contadorPasosY--;
  else
    contadorPasosY++;
}

void intActivateEndstop()
{
  //Serial.printlnln("ENDSTOP");
  if(digitalRead(I_FCX1) || digitalRead(I_FCX2))
    {
      stopMotorX();
    }
  else if (digitalRead(I_FCY1) || digitalRead(I_FCX2))
    {
      stopMotorY();
    }
  else
    {
      stopMotorY();
      stopMotorX();
    }
}

void rearme()
{
  emergencia = false;
  selectVelocityX(1);
  selectVelocityY(1);
    activeMotors();
}

void interruptEmergency()
{
  //Serial.println("EMERGENCIA");
  emergencia = true;
  stopMotorX();
  stopMotorY();
  deactiveMotors();
  delayMicroseconds(10000);
  //TODO Comprobar si es LOW o HIGH en el de emergencia.
  while(digitalRead(INT_EMERG) == LOW) { };
  //TODO
  //rearme();
}

unsigned short getStateEndStops()
{
  short endStopAct = 0000;
  //Serial.println("ENDSTOPS");
  if(digitalRead(I_FCX1) == HIGH)
    endStopAct += 1000;
  if(digitalRead(I_FCX2) == HIGH)
    endStopAct += 100;
  if(digitalRead(I_FCY1) == HIGH)
    endStopAct += 10;
  if(digitalRead(I_FCY2) == HIGH)
    endStopAct += 1;
    Serial.print("Endstops:");
  Serial.print(endStopAct);
  Serial.print("\n");
  return endStopAct;
}

// TODO contolar maximo y minimo con finales de carrera con joystick
// Inicio de velocidad adaptativo
// Activar motores tras emergencia.
// Cambiar la velocidad por serial
// REdundancia
// Monitorizacion serial
// 
void controlJoystick()
{
  if(useJoystick)
  {
    selectVelocityX(2);
    selectVelocityY(2);
   // Serial.println("JOYSTICK");
      if(digitalRead(I_JKIZQ) == HIGH && !digitalRead(I_FCX1))
      //if(digitalRead(I_JKIZQ) == HIGH)
        {
         //  Serial.println("JOYSTICK1");
          delay(tWaitJoystick); //Tiempo para evitar falsas lecturas.
          moveLeft();
//          while(digitalRead(I_JKIZQ) == HIGH && digitalRead(I_FCX1) == LOW) {  };
//          stopMotorX();
        }
      if(digitalRead(I_JKDER) == HIGH && !digitalRead(I_FCX2))
        {
         //  Serial.println("JOYSTICK2");
          delay(tWaitJoystick);
          moveRight();
//          while(digitalRead(I_JKDER) == HIGH && digitalRead(I_FCX2) == LOW) {  };
//          stopMotorX();
        }
      if(digitalRead(I_JKARR) == HIGH && !digitalRead(I_FCY1))
        {
           //Serial.println("JOYSTICK3");
          delay(tWaitJoystick);
          moveUp();
//          while(digitalRead(I_JKARR) == HIGH && digitalRead(I_FCY1) == LOW) {  };
//          stopMotorY();
        }
      if(digitalRead(I_JKABA) == HIGH&& !digitalRead(I_FCY2))
        {
           //Serial.println("JOYSTICK4");
          delay(tWaitJoystick);
          moveDown();
//          while(digitalRead(I_JKABA) == HIGH && digitalRead(I_FCY2) == LOW) {  };
//          stopMotorY();
        }
      if(digitalRead(I_JKDER) == LOW && digitalRead(I_JKIZQ) == LOW && digitalRead(I_JKABA) == 
LOW && digitalRead(I_JKARR) == LOW)
        {
	        stopMotorX();
	        stopMotorY();
        }
  }
}



// 4149 X
// 3223 Y

void serialEvent()
{
  while (Serial.available()) 
  {
    int action = Serial.parseInt();
    int axis = Serial.parseInt();
    int value = Serial.parseInt();
   /*Serial.print("actIon:");
    Serial.print(action);
    Serial.print("Axis:");
    Serial.print(axis);
    Serial.print("Value");
    Serial.println(value);*/
    if (Serial.read() == '\n') {
      if (!useJoystick)
      {
  	switch(action)
	{
	  case 0:
      //Serial.println("Entrado en 0");
	    if(axis == 0)
	       moveXmm(value);
	    else
	       moveYmm(value);
	  break;
 	  case 1:
    //Serial.println("Entrado en 1");
	    if(axis == 0)
	       moveXmm(value*-1);
	    else
	       moveYmm(value*-1);
	  break;
	  case 2:
   //Serial.println("Entrado en 2");
	    if(axis == 0)
	       moveXSteps(value);
	    else
	       moveYSteps(value);
	  break;
	  case 3:
   //Serial.println("Entrado en 3");
	    if(axis == 0)
	       moveXSteps(value*-1);
	    else
	       moveYSteps(value*-1);
	  break;
	  case 4:
   //Serial.println("Entrado en 4");
	    calibrate(axis);
	  break;
	  case 5:
   //Serial.println("Entrado en 5");
	    motorHome(axis);
	  break;
	  case 6:
   //Serial.println("Entrado en 6");
      if (!value)
      {
        stopMotorX();
        stopMotorY();
      }
	    useJoystick = value;
	  break;
	  case 7:
   //Serial.println("Entrado en 7");
	    writePosition();
	  break;
    case 8:
    //Serial.println("Entrado en 8");
      setPosition(value,axis);
    break;
    case 9:
    //Serial.println("Entrado en 9");
      rearme();
    break;
    case 10:
    //Serial.println("Entrado en 10");
      deactiveMotors();
    break;
    case 11:
    //Serial.println("Entrado en 11");
    //Get endstops
    getStateEndStops();
      //Serial.println(getStateEndStops());
    break;
    case 12:
    //Serial.println("Entrado en 12");
      interruptEmergency();
      break;
	  default:
	    Serial.print("He recibido");
	    Serial.println(action);
     break;
	}
      }
	else
	{
  //Serial.println("Joystick");
	  if(action == 6)
	    useJoystick = value;
    else if(action == 7)
      writePosition();
    else if(action == 9)
        rearme();
    else if(action == 10)
        deactiveMotors();
    else if(action == 12)
        interruptEmergency();
    else if(action == 11)
        getStateEndStops();
        
	}
    }
  }


}


bool checkAliveMaster()
{
  if((millis() - tiempoRequestMaster) > TWAITI2C)
  {
  unsigned long time;
  unsigned long cTime;

  char buffer[20] = "";
  Wire.requestFrom(I2CDIR, 2);
  time = millis();


 // Serial.println("Request done"); //Borrar esto
  bool isAlive = false;


  cTime = time;
  int cBuffer = 0;

  while (!isAlive && (cTime - time) < 5000) {
    while (Wire.available() && !isAlive ){//&& (cTime - time) < 5000) {
      buffer[cBuffer] = Wire.read();
      
      ++cBuffer;

      if (strcmp(buffer, "OK")) {
        isAlive = true;
      //  Serial.println("OK received"); //Borrar esto
      }

      cTime = millis();
      
    }
    cTime = millis();
    
  }

 /*   if (!isAlive)
    Serial.println("Tengo el control"); //Borrar esto
  else
    Serial.println("Esclavo responde"); //Borrar esto*/
tiempoRequestMaster = millis();
  return isAlive;

  }
  else
    return true;
   // delay(TWAITI2C);
}


void initMaster()
{
  Wire.begin();                // join i2c bus 
}

void requestEvent() {   // Cuando el maestro se lo solicite, el esclavo respondera OK
  Wire.write("OK");
}

void initSlave()
{
  Wire.begin(I2CDIR);                // join i2c bus 
  Wire.onRequest(requestEvent);       // registrar evento
}

#endif
